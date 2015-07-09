#ifndef BOOK_TEXTNODE_HPP
#define BOOK_TEXTNODE_HPP

#include <Book/ResourceHolder.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/SceneNode.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextNode : public SceneNode
{
public:
	explicit		TextNode(const FontHolder& fonts, const std::string&  text);
	void			setString(const std::string& text);

	void			setColor(const sf::Color& color);
	void			setTextSize(size_t size);
	void			setTextStyle(uint32_t style);


private:
	virtual void	drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Text		mText;
};

#endif