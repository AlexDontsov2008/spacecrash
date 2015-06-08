#include <Book/Sattelite.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Utility.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

Sattelite::Sattelite(Type type, const TextureHolder& textures, sf::Vector2f planetCenterPoint, float speed, float radius, float initialAngle)
: mType(type)
, mSprite(textures.get(Textures::Moon))
, mPlanetCenterPoint(planetCenterPoint)
, mSpeed(speed)
, mRadius(radius)
, mAngle(initialAngle)
{
	mSprite.setPosition(sf::Vector2f(radius * cos(degreeToRadians(mAngle)), radius * sin(degreeToRadians(mAngle))));
}

unsigned int Sattelite::getCategory() const
{
	return Category::Planet;
}

void Sattelite::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Sattelite::updateCurrent(sf::Vector2f planetCenterPosition, sf::Time dt)
{
	mPlanetCenterPoint = planetCenterPosition;
	mAngle = mAngle + mSpeed * dt.asSeconds();

	if (mAngle >= 360.f)
		mAngle = 0.f;

	mSprite.setPosition(sf::Vector2f(planetCenterPosition.y + mRadius * cos(degreeToRadians(mAngle)),
									 planetCenterPosition.x + mRadius * sin(degreeToRadians(mAngle))));
}

std::vector<sf::FloatRect> Sattelite::getBoundingRect() const
{
	std::vector<sf::FloatRect> rects;
	rects.push_back(getWorldTransform().transformRect(mSprite.getGlobalBounds()));
	return rects;
}