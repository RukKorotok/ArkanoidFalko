#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <vector>
#include "SFML/Audio.hpp"
#include "Config.h"
#include "Math.h"
#include "UI.h"
#include "Base.h"
#include "Ball.h"
#include "Block.h"
#include "Rock.h"
#include "Sound.h"
#include "InputHandlers.h"
#include "GridManager.h"

namespace Arkanoid
{
	enum State
	{
		Undefined = 0,
		InMenu,
		InProgress,
		Paused,
		GameOver,
		ExitGame
	};

	enum SettingsMode
	{
		SoundOn = 1 << 0,
		MusicOn = 1 << 1,
	};

	class  Game
	{
	public:

		static Game& GetInstance()
		{
			static Game game;
			return game;
		}

		void StartGame();
		void UpdateGame(float deltaTime);
		State GetState();
		void SetState(State state);
		void SetGameOver();
		void ExitFromGame(sf::RenderWindow& window);
		void DrawGame(sf::RenderWindow& window);
		void ChangeGameSettings(SettingsMode settingMode);
		void ChangeGameDifficulty(GameDifficultyLevel difficulty);
		int GetScore();
		uint32_t GetSetings();
		GameDifficultyLevel GetDifficultyLevel();

	private:
		Game();
		~Game() {}

		Game(Game const&) = delete;
		Game& operator = (Game const&) = delete;

		int m_currentScore = 0;
		float m_tartTime = 0.0f;
		int m_scoreAdded = 1;
		State m_state = State::InMenu;
		uint32_t m_settings = 0b11;
		GameDifficultyLevel m_difficulty = Medium;
	};
}