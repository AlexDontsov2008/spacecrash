#include <Book/Button.hpp>
#include <Book/Utility.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

Textures::ID toTextureNormalID(Button::Type type)
{
	switch (type)
	{
		case Button::PlayButton:
			return Textures::ButtonStartNormal;

		case Button::OptionButton:
			return Textures::ButtonOptionNormal;

		case Button::ExitButton:
			return Textures::ButtonExitNormal;

		default:
			return Textures::ButtonNormal;
	}
}

Textures::ID toTextureSelectedID(Button::Type type)
{
	switch (type)
	{
	case Button::PlayButton:
		return Textures::ButtonStartSelected;

	case Button::OptionButton:
		return Textures::ButtonOptionSelected;

	case Button::ExitButton:
		return Textures::ButtonExitSelected;

	default:
		return Textures::ButtonSelected;
	}
}

Textures::ID toTextureActiveID(Button::Type type)
{
	switch (type)
	{
	case Button::PlayButton:
		return Textures::ButtonStartActive;

	case Button::OptionButton:
		return Textures::ButtonOptionActive;

	case Button::ExitButton:
		return Textures::ButtonExitActive;

	default:
		return Textures::ButtonActive;
	}
}

Button::Button(Button::Type type, const FontHolder& fonts, const TextureHolder& textures)
: mType(type)
, mCallback()
, mNormalTexture(textures.get(toTextureNormalID(type)))
, mSelectedTexture(textures.get(toTextureSelectedID(type)))
, mPressedTexture(textures.get(toTextureActiveID(type)))
, mSprite()
, mText("", fonts.get(Fonts::Main), 16)
, mIsToggle(false)
{
	mSprite.setTexture(mNormalTexture);

	sf::FloatRect bounds = mSprite.getLocalBounds();
	mText.setPosition(bounds.width / 2.f, bounds.height / 2.5f);
}

void Button::setCallback(Callback callback)
{
	mCallback = std::move(callback);
}

void Button::setText(const std::string& text)
{
	mText.setString(text);
	centerOrigin(mText);
}

void Button::setTextSize(size_t size)
{
	mText.setCharacterSize(size);
}


void Button::setSpriteScale(float scale)
{
	mSprite.setScale(scale, scale);
}

void Button::setToggle(bool flag)
{
	mIsToggle = flag;
}

bool Button::isSelectable() const
{
    return true;
}

void Button::select()
{
	Component::select();

	mSprite.setTexture(mSelectedTexture);
}

void Button::deselect()
{
	Component::deselect();

	mSprite.setTexture(mNormalTexture);
}

void Button::activate()
{
	Component::activate();

    // If we are toggle then we should show that the button is pressed and thus "toggled".
	if (mIsToggle)
		mSprite.setTexture(mPressedTexture);

	if (mCallback)
		mCallback();

    // If we are not a toggle then deactivate the button since we are just momentarily activated.
	if (!mIsToggle)
		deactivate();
}

void Button::deactivate()
{
	Component::deactivate();

	if (mIsToggle)
	{
        // Reset texture to right one depending on if we are selected or not.
		if (isSelected())
			mSprite.setTexture(mSelectedTexture);
		else
			mSprite.setTexture(mNormalTexture);
	}
}

void Button::handleEvent(const sf::Event&)
{
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}

}
