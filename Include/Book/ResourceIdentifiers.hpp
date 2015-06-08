#ifndef BOOK_RESOURCEIDENTIFIERS_HPP
#define BOOK_RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID
	{
		Background,
		TitleScreen,
		SpaceshipHighSpeed,
		SpaceshipMaximumSpeed,
		SpaceshipMiddleSpeed,
		SpaceshipMinimumSpeed,
		Earth,
		Moon,
		Pluton,
		ButtonNormal,
		ButtonSelected,
		ButtonActive,
		ButtonStartNormal,
		ButtonStartSelected,
		ButtonStartActive,
		ButtonOptionNormal,
		ButtonOptionSelected,
		ButtonOptionActive,
		ButtonExitNormal,
		ButtonExitSelected,
		ButtonExitActive,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>			FontHolder;

#endif // BOOK_RESOURCEIDENTIFIERS_HPP
