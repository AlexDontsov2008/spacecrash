#ifndef BOOK_Planet_HPP
#define BOOK_Planet_HPP

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
        
        using Ptr = std::unique_ptr<Planet>;
        
	public:
								Planet(Planet::Type type, const TextureHolder& textures);


		virtual unsigned int	getCategory() const;
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		float					getMaxSpeed() const;

		virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual std::vector<sf::FloatRect>	getBoundingRect() const;
		virtual bool			isMarkedForRemoval() const;

	private:
		void					updateMovementPattern(sf::Time dt);
		void					addRect(sf::FloatRect& bounds, float addTop, float addLeft, float width, float height) const;
		void					initialSattelites();
		void					updateSattelites(sf::Time dt);

	private:
		Type					mType;
		sf::Sprite				mSprite;
		float					mTravelledDistance;
		std::size_t				mDirectionIndex;
		std::vector<Sattelite*>	mSattelites;
		const TextureHolder&	mTextures;
		bool					mIsMarkedForRemoval;
};

#endif
