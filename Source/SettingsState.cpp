#include <Book/SettingsState.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	texture.setRepeated(true);
	mBackgroundSprite.setTexture(texture);
	
	// Build key binding buttons and labels
	addButtonLabel(Player::RotateLeft,  250.f, "Rotate Left", context);
	addButtonLabel(Player::RotateRight, 340.f, "Rotate Right", context);
	addButtonLabel(Player::MoveUp,    430.f, "Move Up", context);
	addButtonLabel(Player::MoveDown,  520.f, "Move Down", context);

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(GUI::Button::ExitButton, *context.fonts, *context.textures);
	backButton->setPosition(340.f, 610.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));
	backButton->setTextSize(25);
	backButton->setScale(0.8f, 0.8f);

	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;
	
	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < Player::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
		updateLabels();
	else
		mGUIContainer.handleEvent(event);

	return false;
}

void SettingsState::updateLabels()
{
	Player& player = *getContext().player;

	for (std::size_t i = 0; i < Player::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		mBindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{
	mBindingButtons[action] = std::make_shared<GUI::Button>(GUI::Button::DefaultButton, *context.fonts, *context.textures);
	mBindingButtons[action]->setPosition(340.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setTextSize(25);
	mBindingButtons[action]->setToggle(true);
	mBindingButtons[action]->setScale(0.8f, 0.8f);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabels[action]->setPosition(620.f, y + 15.f);
	mBindingLabels[action]->setTextSizeAndColor(35, sf::Color(12, 75, 118));

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}
