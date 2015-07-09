#include <Book/Sattelite.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Utility.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

static const float SCALE = 0.7f;

float getCoord(float radius, float angle, double(*func)(double))
{
	return radius * func(degreeToRadians(angle));
}

Sattelite::Sattelite(Type type, const TextureHolder& textures, sf::Vector2f planetCenterPoint, float speed, float radius, float initialAngle)
: Entity(1)
, mType(type)
, mSprite(textures.get(Textures::Moon))
, mPlanetCenterPoint(planetCenterPoint)
, mSpeed(speed)
, mRadius(radius)
, mAngle(initialAngle)
{
	centerOrigin(mSprite);
	mSprite.setScale(SCALE, SCALE);
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

void Sattelite::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	mAngle += mSpeed * dt.asSeconds();

	if (mAngle >= 360.f)
		mAngle = 0.f;

	mSprite.setPosition(getCoord(mRadius, mAngle, cos), getCoord(mRadius, mAngle, sin));
	mSprite.rotate(30  * dt.asSeconds());
}


std::vector<sf::FloatRect> Sattelite::getBoundingRect() const
{
	std::vector<sf::FloatRect> rects;
	static sf::FloatRect bounds = mSprite.getGlobalBounds();
	bounds.left = getWorldPosition().x + getCoord(mRadius, mAngle, cos) - mSprite.getTextureRect().width / 2.f * SCALE;
	bounds.top = getWorldPosition().y + getCoord(mRadius, mAngle, sin) - mSprite.getTextureRect().height / 2.f * SCALE;
	rects.push_back(bounds);
	return rects;
}

