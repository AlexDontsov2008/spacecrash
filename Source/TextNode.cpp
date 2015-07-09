#include <Book/TextNode.hpp>
#include <Book/Utility.hpp>

#include <SFML/Graphics/RenderTarget.hpp>


TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
	mText.setFont(fonts.get(Fonts::Main));
	mText.setCharacterSize(20);
	setString(text);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}

void TextNode::setString(const std::string& text)
{
	mText.setString(text);
	centerOrigin(mText);
}


void TextNode::setColor(const sf::Color& color)
{
	mText.setColor(color);
}


void TextNode::setTextSize(size_t size)
{
	mText.setCharacterSize(size);
}

void TextNode::setTextStyle(uint32_t style)
{
	mText.setStyle(style);
}