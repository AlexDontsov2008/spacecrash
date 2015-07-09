#include <Book/Player.hpp>
#include <Book/CommandQueue.hpp>
#include <Book/Spaceship.hpp>
#include <Book/Utility.hpp>

#include <map>
#include <string>
#include <algorithm>
#include <cmath>


struct SpaceshipMover
{
	SpaceshipMover(float angle, float radius)
	: mAngle(angle)
	, mRadius(radius)
	{
	}

	void operator() (Spaceship& spaceship, sf::Time dt) const
	{
		spaceship.rotate(mAngle * dt.asSeconds());

		float xCoord = sin(degreeToRadians(spaceship.getRotation()));
		float yCoord = cos(degreeToRadians(spaceship.getRotation()));

		spaceship.accelerate(-mRadius * xCoord, +mRadius * yCoord);
	}

	float mAngle;
	float mRadius;
};



Player::Player()
: mCurrentMissionStatus(MissionRunning)
, mPoints(0.f)
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = RotateLeft;
	mKeyBinding[sf::Keyboard::Right] = RotateRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;

	// Set initial action bindings
	initializeActions();	

	// Assign all categories to player's aircraft
	for(auto& pair : mActionBinding)
		pair.second.category = Category::PlayerSpaceship;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for(auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for(auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	const float angle = 100.f;
	const float radius = 250.f;

	mActionBinding[RotateLeft].action	= derivedAction<Spaceship>(SpaceshipMover(-angle, 0.f));
	mActionBinding[RotateRight].action	= derivedAction<Spaceship>(SpaceshipMover(+angle, 0.f));
	mActionBinding[MoveUp].action		= derivedAction<Spaceship>(SpaceshipMover(0.f, -radius));
	mActionBinding[MoveDown].action		= derivedAction<Spaceship>(SpaceshipMover(0.f, +radius));
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case RotateLeft:
		case RotateRight:
		case MoveDown:
		case MoveUp:
			return true;

		default:
			return false;
	}
}

void Player::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

float Player::getPoints() const
{
	return mPoints;
}


void Player::setPoints(float points)
{
	mPoints = points;
}
