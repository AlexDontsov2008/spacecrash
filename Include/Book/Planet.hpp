#ifndef BOOK_PLANET_HPP
#define BOOK_PLANET_HPP

#include <Book/Entity.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/Sattelite.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <vector>

class Planet : public Entity
{
	public:
		enum Type
		{
			Earth,
			Pluton,
			AsteroidGreate,
			AsteroidAverage,
			AsteroidLittle,
			TypeCount,
		};


	public:
								Planet(Planet::Type type, const TextureHolder& textures);


		virtual unsigned int	getCategory() const;
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		float					getMaxSpeed() const;

		virtual void			updateCurrent(sf::Time dt);
		virtual std::vector<sf::FloatRect>	getBoundingRect() const;

	private:
		void					updateMovementPattern(sf::Time dt);
		void					addRect(sf::FloatRect& bounds, float addTop, float addLeft, float width, float height) const;
		void					initialSattelites();


	private:
		Type					mType;
		sf::Sprite				mSprite;
		float					mTravelledDistance;
		std::size_t				mDirectionIndex;
		std::vector<Sattelite*>	mSattelites;
		const TextureHolder&	mTextures;
};

#endif