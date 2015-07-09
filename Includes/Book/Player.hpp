#ifndef BOOK_PLAYER_HPP
#define BOOK_PLAYER_HPP

#include <Book/Command.hpp>

#include <SFML/Window/Event.hpp>

#include <map>


class CommandQueue;

class Player
{
	public:
		enum Action
		{
			RotateLeft,
			RotateRight,
			MoveUp,
			MoveDown,
			ActionCount
		};

		enum MissionStatus
		{
			MissionRunning,
			MissionFailure,
		};


	public:
								Player();

		void					handleEvent(const sf::Event& event, CommandQueue& commands);
		void					handleRealtimeInput(CommandQueue& commands);

		void					assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key		getAssignedKey(Action action) const;

		void					setMissionStatus(MissionStatus status);
		MissionStatus			getMissionStatus() const;

		float					getPoints() const;
		void					setPoints(float points);

	private:
		void					initializeActions();
		static bool				isRealtimeAction(Action action);


	private:
		std::map<sf::Keyboard::Key, Action>		mKeyBinding;
		std::map<Action, Command>				mActionBinding;
		MissionStatus							mCurrentMissionStatus;
		float									mPoints;
};

#endif // BOOK_PLAYER_HPP
