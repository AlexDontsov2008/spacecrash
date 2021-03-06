#ifndef BOOK_SATTELITE_HPP
#define BOOK_SATTELITE_HPP

#include <Book/Entity.hpp>
#include <Book/ResourceIdentifiers.hpp>


#include <SFML/Graphics/Sprite.hpp>

class Sattelite : public Entity
{
	public:
		enum Type
		{
			Moon,
		};


	public:
		Sattelite(Type type, const TextureHolder& textures, sf::Vector2f planetCenterPoint, float speed, float radius, float startAngle);

		virtual unsigned int				getCategory() const;
		virtual void						drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual	void						updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual std::vector<sf::FloatRect>	getBoundingRect() const;

	private:
		Type					mType;
		sf::Sprite				mSprite;
		sf::Vector2f			mPlanetCenterPoint;
		const float				mSpeed;
		const float				mRadius;
		float					mAngle;
};



#endif //BOOK_SATTELITE_HPP
