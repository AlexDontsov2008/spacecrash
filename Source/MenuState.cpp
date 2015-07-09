#include <Book/MenuState.hpp>
#include <Book/Button.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	texture.setRepeated(true);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(GUI::Button::PlayButton, *context.fonts, *context.textures);
	playButton->setPosition(370, 300);
	playButton->setText("Play");
	playButton->setTextSize(25);
	playButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto settingsButton = std::make_shared<GUI::Button>(GUI::Button::OptionButton, *context.fonts, *context.textures);
	settingsButton->setPosition(370, 420);
	settingsButton->setText("Settings");
	settingsButton->setTextSize(25);
	settingsButton->setCallback([this] ()
	{
		requestStackPush(States::Settings);
	});

	auto exitButton = std::make_shared<GUI::Button>(GUI::Button::ExitButton, *context.fonts, *context.textures);
	exitButton->setPosition(370, 540);
	exitButton->setText("Exit");
	exitButton->setTextSize(25);
	exitButton->setCallback([this] ()
	{
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
