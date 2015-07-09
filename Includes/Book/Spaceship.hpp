#ifndef BOOK_AIRCRAFT_HPP
#define BOOK_AIRCRAFT_HPP

#include <Book/Entity.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/TextNode.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <vector>


class Spaceship : public Entity
{
	public:
		enum Type
		{
			Rocket,
			TypeCount,
		};


	public:
								Spaceship(Type type, const TextureHolder& textures);

		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual unsigned int	getCategory() const;
		virtual bool			isMarkedForRemoval() const;
		

		virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);

		virtual std::vector<sf::FloatRect>	getBoundingRect() const;


	private:
		void					updateAnimation(sf::Time dt);

	private:
		Type					mType;
		sf::Sprite				mSprite;
		bool					mFlag;
		const TextureHolder&	mTextures;
		bool					mIsMarkedForRemoval;
};

#endif // BOOK_AIRCRAFT_HPP
