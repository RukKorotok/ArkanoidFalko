#pragma once

#include <string>
#include <map>
#include <cassert>
#include "SFML/Graphics.hpp"

namespace Arkanoid
{	
	enum MenuStates
	{
		MainMenu = 0,
		DifficultyLevel,
		RecordsList,
		Settings,
		Pause,
		SetName,
		Death,
		Accept
	};

	enum MenuEvents
	{
		GameStart = 0,
		GoBack,
		InMainMenu,
		CloseGame,
		GoToMainManu,
		GoToDifficultyLevel,
		GoToSettings,
		SetSound,
		SetMusic,
		SetEasyDifficulty,
		SetNotEasyDifficulty,
		SetMediumDifficulty,
		SetNotMediumDifficulty,
		SetHardDifficulty,
		AcceptSubMenu,
		OpenSubMenu,
		OpenRecordList,
	};

	enum GameDifficultyLevel
	{
		Easy = 1,
		NotEasy,
		Medium,
		NotMedium,
		Hard
	};

	struct MenuItem 
	{
		MenuEvents event;
		sf::String menuPointTitle;
		bool isHaveCheckBox;
	};

	struct MenuItemsList
	{
		std::string title;
		std::vector <MenuItem> items;
	};
	// Resources path
	const std::string RESOURCES_PATH = "Resources/";

	// Titles
	const int MENU_POINTS_MAX_SIZE = 6;
	const int POP_MENU_POINTS_MAX_SIZE = 2;
	const std::string SCORE_TITLE = "Score: ";
	const std::string INPUT_TITLE = "Inter name: ";

	const std::string OPPONENTS_NAMES_ITEMS[] = { "Dave", "Carol", "Bob", "Alice" };
	const std::string PLAYER_NAME = "Player";
	
	// Game config
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const float INITIAL_BALL_SPEED = 400.0f; 
	const float SPETIAL_APPLE_TIME = 20.0f;
	const float BALL_SIZE = 35.0f;
	const float BASE_SEGMENT_SIZE = 35.0f;
	const float BASE_REBOUND_MAX_ANGLE = 80.0f;
	const float CELL_SIZE = 20.0f;
	const int TEXT_SIZE = 24;
	const int MAX_OPPONENTS_SCORE = 50;
	const int BASE_START_SIZE = 3;
	const int PENALTY_STEPS = 20;
	const float START_DELAY = 3.0f;

	// Inputs config
	const sf::Keyboard::Key UP_DIRECTION = sf::Keyboard::Key::W;
	const sf::Keyboard::Key DOWN_DIRECTION = sf::Keyboard::Key::S;
	const sf::Keyboard::Key LEFT_DIRECTION = sf::Keyboard::Key::A;
	const sf::Keyboard::Key RIGHT_DIRECTION = sf::Keyboard::Key::D;
	const sf::Keyboard::Key PAUSE = sf::Keyboard::Key::P;
	const sf::Keyboard::Key ENTER = sf::Keyboard::Key::Enter;
	const sf::Keyboard::Key GO_BACK = sf::Keyboard::Key::B;
}
