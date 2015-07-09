#include <Book/World.hpp>
#include <Book/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>

static const float WorldLeght = 3000.f;
static const float ScrollSpeedAccelerate = 3.f;

static bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

static void	setTextProperties(TextNode* text)
{
	text->setColor(sf::Color(12, 75, 118));
	text->setTextSize(25);
	text->setTextStyle(sf::Text::Bold);

	return;
}


World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mFonts()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, WorldLeght)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 3.f)
, mScrollSpeed(-200.f)
, mPlayerSpaceship(nullptr)
, mPlanetSpawnPoints()
, mBackgroundSprite(nullptr)
, mHealthDisplay(nullptr)
, mPointsDisplay(nullptr)
, mPoints(0)
, mOldWorldCoordY(0.f)
, mDistanceForSpawnNewEnemies(WorldLeght)
{

	mFonts.load(Fonts::Main, "Media/Sansation.ttf");
	loadTextures();
	buildScene();	

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition.x, mWorldBounds.height - mWorldView.getSize().y / 2.f);

}


void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
	for (SceneNode::Pair pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerSpaceship, Category::Planet))
		{
			auto& player = static_cast<Spaceship&>(*pair.first);
			auto& planet = static_cast<Planet&>(*pair.second);
			
			player.destroy();
		}
	}
}

void World::initialHealthDisplay()
{
	std::unique_ptr<TextNode> healthDisplay(new TextNode(mFonts, ""));
	mHealthDisplay = healthDisplay.get();
	mSceneLayers[Space]->attachChild(std::move(healthDisplay));
	updateHealthDisplay();
}

void World::initialPointsDisplay()
{
	std::unique_ptr<TextNode> pointsDisplay(new TextNode(mFonts, ""));
	mPointsDisplay = pointsDisplay.get();
	mSceneLayers[Space]->attachChild(std::move(pointsDisplay));
	updatePointsDisplay(sf::Time::Zero);
}

void World::initialText()
{
	initialHealthDisplay();
	initialPointsDisplay();
}

void World::updateHealthDisplay()
{
	mHealthDisplay->setString("Health: " + toString(mPlayerSpaceship->getHitpoints()) + "%");
	mHealthDisplay->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2 - 100,
								mWorldView.getCenter().y - mWorldView.getSize().y / 2 + 25);
	setTextProperties(mHealthDisplay);
}

void World::updatePointsDisplay(sf::Time dt)
{
	mPointsDisplay->setString("Points: " + toString((int)mPoints));
	mPointsDisplay->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2 - 125,
								mWorldView.getCenter().y - mWorldView.getSize().y / 2 + 55);
	setTextProperties(mPointsDisplay);
	mPoints += abs(mScrollSpeed) * dt.asSeconds();
}

void World::updateText(sf::Time dt)
{
	updatePointsDisplay(dt);
	updateHealthDisplay();
}

void World::update(sf::Time dt)
{
	// Scroll the world, reset player velocity
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());	
	mPlayerSpaceship->setVelocity(0.f, 0.f);

	// Check for delete enemies
	destroyEntitiesOutsideView();


	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();
	// Handle our collision.
	handleCollisions();

	// Remove all destroyed entities, and create new.
	mSceneGraph.removeWrecks();
	// Templates for add new Enemies.
	addNewEnemies(dt);
	spawnEnemies();
	// Check, for edding new world texture.
	updateWorldBounds();


	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();
	// Update Points and Health.
	updateText(dt);

	mBattlifieldBounds.setPosition(getBattlefieldBounds().left, getBattlefieldBounds().top);
	// Update ScrollSpeed
	mScrollSpeed -= ScrollSpeedAccelerate * dt.asSeconds();
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
	mWindow.draw(mBattlifieldBounds);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::loadTextures()
{
	mTextures.load(Textures::Background, "Media/Textures/Background.png");

	mTextures.load(Textures::SpaceshipHighSpeed, "Media/Textures/SpaceshipHighSpeed.png");
	mTextures.load(Textures::SpaceshipMaximumSpeed, "Media/Textures/SpaceshipMaximumSpeed.png");
	mTextures.load(Textures::SpaceshipMiddleSpeed, "Media/Textures/SpaceshipMiddleSpeed.png");
	mTextures.load(Textures::SpaceshipMinimumSpeed, "Media/Textures/SpaceshipMinimumSpeed.png");

	mTextures.load(Textures::Earth, "Media/Textures/Earth.png");
	mTextures.load(Textures::Moon, "Media/Textures/Moon.png");
	mTextures.load(Textures::Pluton, "Media/Textures/Pluton.png");

	mTextures.load(Textures::AsteroidGreate, "Media/Textures/AsteroidGreate.png");
	mTextures.load(Textures::AsteroidAverage, "Media/Textures/AsteroidAverage.png");
	mTextures.load(Textures::AsteroidLittle, "Media/Textures/AsteroidLittle.png");

	for (size_t textureCount = Textures::SpaceshipHighSpeed; textureCount <= Textures::Pluton; ++textureCount)
	{
		mTextures.smoothOn(static_cast<Textures::ID>(textureCount));
	}
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& texture = mTextures.get(Textures::Background);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mBackgroundSprite = backgroundSprite.get();
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));


	// Add player's spaceship
	std::unique_ptr<Spaceship> leader(new Spaceship(Spaceship::Rocket, mTextures));
	mPlayerSpaceship = leader.get();
	mPlayerSpaceship->setPosition(mSpawnPosition);
	mSceneLayers[Space]->attachChild(std::move(leader));

	mBattlifieldBounds.setSize(sf::Vector2f(getBattlefieldBounds().width, getBattlefieldBounds().height));
	mBattlifieldBounds.setPosition(getBattlefieldBounds().left, getBattlefieldBounds().top);
	mBattlifieldBounds.setFillColor(sf::Color::Transparent);
	mBattlifieldBounds.setOutlineThickness(2.f);
	mBattlifieldBounds.setOutlineColor(sf::Color::Green);

	initialText();
}


void World::updateWorldBounds()
{
	if (isNeedNewWorld())
		addNewWorldArea();

	if (isNeedDeleteOldWorld())
	{
		if (mSceneLayers[Background]->getLastChild() == nullptr)
			return;
		mSceneLayers[Background]->detachChild(*mBackgroundSprite);
		mBackgroundSprite = static_cast<SpriteNode*>(mSceneLayers[Background]->getLastChild());
		mOldWorldCoordY = mWorldBounds.top;
	}
}

// Add new world Texture bounds.
void World::addNewWorldArea()
{
	sf::Texture& texture = mTextures.get(Textures::Background);
	mWorldBounds.top -= WorldLeght;

	sf::IntRect textureRect(mWorldBounds.left, mWorldBounds.top, mWorldBounds.width, mWorldBounds.height);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
}

// Check for adding new World texture.
bool World::isNeedNewWorld() const
{
	if (abs(mWorldBounds.top - mWorldView.getCenter().y) <= mWorldView.getSize().y)
		return true;
	else
		return false;
}

// Check for delete old World texture.
bool World::isNeedDeleteOldWorld() const
{
	if (mWorldView.getCenter().y - mOldWorldCoordY <= -mWorldView.getSize().y)
		return true;
	else
		return false;
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least ds.width - borderDistance);
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 80.f;

	sf::Vector2f position = mPlayerSpaceship->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerSpaceship->setPosition(position);
}


void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerSpaceship->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerSpaceship->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	float xCoord = sin(degreeToRadians(mPlayerSpaceship->getRotation()));
	float yCoord = cos(degreeToRadians(mPlayerSpaceship->getRotation()));
	mPlayerSpaceship->accelerate(-mScrollSpeed * xCoord, +mScrollSpeed * yCoord);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 1000.f;
	bounds.height += 1000.f;
	bounds.left -= 500.f;
	bounds.width += 1000.f;

	return bounds;
}

void World::spawnEnemies()
{
	while (!mPlanetSpawnPoints.empty()
		&& mPlanetSpawnPoints.back().y > getBattlefieldBounds().top + 300)
	{
		SpawnPoint spawn = mPlanetSpawnPoints.back();

		std::unique_ptr<Planet> planet(new Planet(spawn.type, mTextures));
		planet->setPosition(spawn.x, spawn.y);
		mSceneLayers[Space]->attachChild(std::move(planet));
		mPlanetSpawnPoints.pop_back();
	}
}

void World::addPlanets(int enemiesCase)
{
    addPlanet(Planet::AsteroidAverage, -200.f, 800);
	addPlanet(Planet::Earth, 0.f, 900.f);
	addPlanet(Planet::AsteroidAverage, -200.f, 800);
	addPlanet(Planet::AsteroidLittle, +350.f, 900);
	addPlanet(Planet::AsteroidGreate, +300.f, 1100);
	addPlanet(Planet::Pluton, -100.f, 1500);
	addPlanet(Planet::AsteroidAverage, -250.f, 1300);
	addPlanet(Planet::Earth, -100.f, 2200);
	addPlanet(Planet::AsteroidLittle, +280.f, 1700);
	addPlanet(Planet::AsteroidGreate, +355.f, 2000);
	addPlanet(Planet::Pluton, +180.f, 2900);
	addPlanet(Planet::AsteroidAverage, -500.f, 2500);
	addPlanet(Planet::AsteroidLittle, +280.f, 3200);
	addPlanet(Planet::AsteroidLittle, +350.f, 3350);
 
	std::sort(mPlanetSpawnPoints.begin(), mPlanetSpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::addPlanet(Planet::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mWorldView.getCenter().x + relX, mWorldView.getCenter().y - relY);
	mPlanetSpawnPoints.push_back(spawn);
}

sf::View World::getWorldView() const
{
	return mWorldView;
}


void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Planet | Category::Sattelite;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		sf::FloatRect boundingRect = e.getBoundingRect()[0];
		bool isNeedDelete = true;

		if (!getBattlefieldBounds().intersects(boundingRect) && e.getHitpoints() > 0)
		{
			const std::vector<SceneNode::Ptr>& sattelites = e.getChilds();

			for (int satteliteCounter = 0; satteliteCounter < sattelites.size(); ++satteliteCounter)
			{
				if (getBattlefieldBounds().intersects(sattelites[satteliteCounter]->getBoundingRect()[0]))
				{
					isNeedDelete = false;
				}
			}
			if (isNeedDelete)
			{
                e.destroy();
			}
		}
	});

	mCommandQueue.push(command);
}

void World::addNewEnemies(sf::Time dt)
{
	if (isNeedNewEnemies())
	{
		std::srand(std::time(NULL));

		int enemiesCase = rand() % 3 + 1;
		addPlanets(enemiesCase);
	}
	else
	{
		mDistanceForSpawnNewEnemies -= mScrollSpeed * dt.asSeconds();
	}

}

bool World::isNeedNewEnemies()
{
	if (mDistanceForSpawnNewEnemies > WorldLeght)
	{
		mDistanceForSpawnNewEnemies = 0.f;
		return true;
	}
	else
		return false;
}

bool World::hasAlivePlayer()
{
	return !mPlayerSpaceship->isMarkedForRemoval();
}

float World::getPoints() const
{
	return mPoints;
}
