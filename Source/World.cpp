#include <Book/World.hpp>
#include <Book/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>


bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
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


World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mFonts()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 4000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerSpaceship(nullptr)
, mPlanetSpawnPoints()
, mBackgroundSprite(nullptr)
, mHealthDisplay(nullptr)
, mPointsDisplay(nullptr)
, mPoints(0)
, mOldWorldCoordY(0.f)
{
	mFonts.load(Fonts::Main, "Media/Sansation.ttf");
	loadTextures();
	buildScene();
	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
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
			
			player.damage(10);
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
	mHealthDisplay->setColor(sf::Color(12, 75, 118));
	mHealthDisplay->setTextSize(25);
	mHealthDisplay->setTextStyle(sf::Text::Bold);
}

void World::updatePointsDisplay(sf::Time dt)
{
	mPointsDisplay->setString("Points: " + toString((int)mPoints));
	mPointsDisplay->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2 - 125,
								mWorldView.getCenter().y - mWorldView.getSize().y / 2 + 55);
	mPointsDisplay->setColor(sf::Color(12, 75, 118));
	mPointsDisplay->setTextSize(25);
	mPointsDisplay->setTextStyle(sf::Text::Bold);

	mPoints += abs(mScrollSpeed) * dt.asSeconds();
}

void World::updateText(sf::Time dt)
{
	updatePointsDisplay(dt);
	updateHealthDisplay();
}

void World::update(sf::Time dt)
{
	std::cout << mWorldView.getCenter().y << std::endl;
	// Scroll the world, reset player velocity
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());	
	mPlayerSpaceship->setVelocity(0.f, 0.f);

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();

	handleCollisions();
	spawnEnemies();
	updateWorldBounds();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt);
	adaptPlayerPosition();

	updateText(dt);
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
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

	initialText();

	addPlanets();
}


//	
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
	mWorldBounds.top -= 4000.f;

	sf::IntRect textureRect(mWorldBounds.left, mWorldBounds.top, mWorldBounds.width, mWorldBounds.height);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
}

// Check for adding new World texture.
bool World::isNeedNewWorld()
{
	if (abs(mWorldBounds.top - mWorldView.getCenter().y) <= 1000.f)
		return true;
	else
		return false;
}

// Check for delete old World texture.
bool World::isNeedDeleteOldWorld()
{
	if (mWorldView.getCenter().y - mOldWorldCoordY <= -1000.f)
		return true;
	else
		return false;
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
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
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::spawnEnemies()
{
	while (!mPlanetSpawnPoints.empty()
		&& mPlanetSpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mPlanetSpawnPoints.back();

		std::unique_ptr<Planet> planet(new Planet(spawn.type, mTextures));
		planet->setPosition(spawn.x, spawn.y);

		mSceneLayers[Space]->attachChild(std::move(planet));

		mPlanetSpawnPoints.pop_back();
	}
}

void World::addPlanets()
{
	addPlanet(Planet::Earth, 0.f, 500.f);
	addPlanet(Planet::Earth, -70.f, 1600.f);
	addPlanet(Planet::Pluton, +70.f, 2500.f);

	std::sort(mPlanetSpawnPoints.begin(), mPlanetSpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::addPlanet(Planet::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mPlanetSpawnPoints.push_back(spawn);
}
