#include <Book/Planet.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/DataTables.hpp>
#include <Book/Utility.hpp>
#include <Book/Sattelite.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <ctime>
#include <memory>

namespace
{
	const std::vector<PlanetData> Table = initializePlanetData();
}

Planet::Planet(Planet::Type type, const TextureHolder& textures)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texutre))
, mSattelites()
, mTextures(textures)
, mIsMarkedForRemoval(false)
, mDirectionIndex(0)
{
	centerOrigin(mSprite);
	
	if (mType == Earth || mType == Pluton)
		initialSattelites();
}

void Planet::initialSattelites()
{

 	std::srand(time(NULL));
	
	size_t sattelitesCount = rand() % 3 + 2;
	size_t sattelitesRadius = rand() % 50 + 200;
	for (size_t counter = 1; counter <= sattelitesCount; ++counter)
	{
		std::unique_ptr<Sattelite> sattelite(new Sattelite(Sattelite::Moon, 
														   mTextures,
														   mSprite.getPosition(),
														   (rand() % 25 + 35) / counter,
														   counter * sattelitesRadius / 1.5,
														   rand() % 360 * 1.f));
		mSattelites.push_back(sattelite.get());
		this->attachChild(std::move(sattelite));
	}

}

void Planet::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}


void Planet::updateMovementPattern(sf::Time dt)
{
	
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		float distanceToTravel = directions[mDirectionIndex].distance;

		if (mTravelledDistance > distanceToTravel)
		{
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}

		float radians = degreeToRadians(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);
		setVelocity(vx, vy);
		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

float Planet::getMaxSpeed() const
{
	return Table[mType].speed;
}

/*
void Planet::updateSattelites(sf::Time dt)
{
	for (auto sattelite : mSattelites)
	{
		sattelite->updateCurrent(mSprite.getPosition(), dt);
	}
}
*/

void Planet::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isDestroyed())
	{
		mIsMarkedForRemoval = true;
		return;
	}

	updateMovementPattern(dt);
	mSprite.rotate(15 * dt.asSeconds());
    //updateSattelites(dt);
	Entity::updateCurrent(dt, commands);

}


void Planet::addRect(sf::FloatRect& bounds, float addTop, float addLeft, float width, float height) const
{
	bounds.top += addTop;
	bounds.left += addLeft;
	bounds.width = width;
	bounds.height = height;
}


std::vector<sf::FloatRect> Planet::getBoundingRect() const
{
	std::vector<sf::FloatRect> rects;
	if (mType == Earth || mType == Pluton)
	{
		sf::FloatRect bounds = mSprite.getGlobalBounds();

		// Because center of the origin in (100.f, 100.f)
		bounds.left = -mSprite.getTextureRect().width / 2.f;
		bounds.top = -mSprite.getTextureRect().height / 2.f;

		addRect(bounds, 4.f, 60.f, 80.f, 30.f);
		rects.push_back(getWorldTransform().transformRect(bounds));
		addRect(bounds, 30.f, -35.f, 150.f, 30.f);
		rects.push_back(getWorldTransform().transformRect(bounds));
		addRect(bounds, 30.f, -22.f, 194.f, 30.f);
		rects.push_back(getWorldTransform().transformRect(bounds));
		addRect(bounds, 30.f, 0.f, 194.f, 30.f);
		rects.push_back(getWorldTransform().transformRect(bounds));
		addRect(bounds, 30.f, 10.f, 175.f, 30.f);
		rects.push_back(getWorldTransform().transformRect(bounds));
		addRect(bounds, 30.f, 15.f, 145.f, 15.f);
		rects.push_back(getWorldTransform().transformRect(bounds));
		addRect(bounds, 15.f, 15.f, 115.f, 15.f);
		rects.push_back(getWorldTransform().transformRect(bounds));
		addRect(bounds, 16.f, 20.f, 74.f, 12.f);
		rects.push_back(getWorldTransform().transformRect(bounds));
	}
	else
	{
		static sf::FloatRect bounds;

		bounds.left = -mSprite.getTextureRect().width / 2.f;
		bounds.top = -mSprite.getTextureRect().height / 2.f;
		bounds.width = mSprite.getTextureRect().width * 1.f;
		bounds.height = mSprite.getTextureRect().height * 1.f;

		rects.push_back(getWorldTransform().transformRect(bounds));
	}

	return rects;
}

unsigned int Planet::getCategory() const
{
	return Category::Sattelite;
}

bool Planet::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}


