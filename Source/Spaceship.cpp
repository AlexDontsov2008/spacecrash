#include <Book/Spaceship.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Utility.hpp>
#include <Book/DataTables.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


namespace
{
	const std::vector<SpaceshipData> Table = initializeSpaceshipData();
}


Spaceship::Spaceship(Spaceship::Type type, const TextureHolder& textures)
: mType(type)
, mSprite(textures.get(Table[type].texture))
, mFlag(true)
, mTextures(textures)
, mHitpoints(Table[type].hitpoints)
, Entity()
{
	centerOrigin(mSprite);
}

void Spaceship::repair(int points)
{
	mHitpoints += points;
}

void Spaceship::damage(int points)
{
	mHitpoints -= points;
}

void Spaceship::destroy()
{
	mHitpoints = 0;
}

void Spaceship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Spaceship::getCategory() const
{
	switch (mType)
	{
		case Rocket:
			return Category::PlayerSpaceship;
	}
}

void Spaceship::updateCurrent(sf::Time dt)
{
	Entity::updateCurrent(dt);

	static sf::Time time = sf::Time::Zero;

	if (time.asSeconds() >= 0.f && time.asSeconds() < 0.1f)
	{
		mSprite.setTexture(mTextures.get(Textures::SpaceshipMinimumSpeed));
	}
	else if (time.asSeconds() >= 0.1f && time.asSeconds() < 0.2f)
	{
		mSprite.setTexture(mTextures.get(Textures::SpaceshipMiddleSpeed));
	}
	else if (time.asSeconds() >= 0.2f && time.asSeconds() < 0.3f)
	{
		mSprite.setTexture(mTextures.get(Textures::SpaceshipHighSpeed));
	}
	else if (time.asSeconds() >= 0.3f && time.asSeconds() < 0.4f)
	{
		mSprite.setTexture(mTextures.get(Textures::SpaceshipMaximumSpeed));
		time = sf::Time::Zero;
	}
	time += dt;
}

int Spaceship::getHitpoints() const
{
	return mHitpoints;
}

std::vector<sf::FloatRect> Spaceship::getBoundingRect() const
{
	std::vector<sf::FloatRect> rects;


	sf::FloatRect bounds = mSprite.getGlobalBounds();

	bounds.top += 32.f;
	bounds.left += 20.f;
	bounds.height = 10.f;
	bounds.width = 20.f;

	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 12.f;
	bounds.left -= 20.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.left += 40.f;
	bounds.top -= 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	bounds.left -= 5.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.left -= 30.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.top += 10.f;
	bounds.left += 5.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.left += 25.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.left -= 5.f;
	bounds.top += 10.f;
	rects.push_back(getWorldTransform().transformRect(bounds));
	bounds.left -= 22.f;
	rects.push_back(getWorldTransform().transformRect(bounds));

	return rects;
}
