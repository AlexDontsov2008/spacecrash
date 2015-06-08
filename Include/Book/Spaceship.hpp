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

		void					repair(int points);
		void					damage(int points);
		void					destroy();

		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual unsigned int	getCategory() const;
		int						getHitpoints() const;
		

		virtual void			updateCurrent(sf::Time dt);

		virtual std::vector<sf::FloatRect>	getBoundingRect() const;

	private:
		Type					mType;
		sf::Sprite				mSprite;
		bool					mFlag;
		const TextureHolder&	mTextures;
		int						mHitpoints;
};

#endif // BOOK_AIRCRAFT_HPP
