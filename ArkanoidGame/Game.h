#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <fstream>
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

namespace Arkanoid
{
	class Game;

	class GameState
	{
	public:
		GameState(State state, int score);
		~GameState();

		virtual void UpdateGame(sf::RenderWindow& window, float deltaTime);
		virtual void RefreshMousePosition(float position) {}
		State GetState();
		void SetScore(int score);
		int GetScore();

	protected:

		State m_state = State::Main;
		int m_score;
		std::shared_ptr<Menu> m_menu = nullptr;
	};

	class GameStateInRuntime final : public GameState
	{
	public:
		GameStateInRuntime(State state, int score);
		~GameStateInRuntime();

		void UpdateGame(sf::RenderWindow& window, float deltaTime) override;
		void RefreshMousePosition(float position) override;

	private:

		void CheckBallCollitions();

		Base* m_base = nullptr;
		Ball* m_ball = nullptr;
	};


	class  Game
	{
	public:

		static Game& GetInstance()
		{
			static Game game;
			return game;
		}

		void SetGameSettings(SettingsMode settingMode);
		void SetGameDifficulty(DifficultyLevel difficulty);
		uint32_t GetSetings();
		DifficultyLevel GetDifficultyLevel();
		std::shared_ptr<GameState> GetCurrentGameState();
		void ExitFromGame();
		void AddGameState(State state, int score);
		void RemoveGameState();
		void ResetGame();
		std::vector<std::shared_ptr<RecordItem>> ReadRecordsList();
		void WriteRecordList(const std::vector<std::shared_ptr<RecordItem>> recordList);
		void AddRecordScore(const std::string& name, int newScore);
		void SetWindowRef(sf::RenderWindow* window);
		sf::RenderWindow* GetWindow();
	private:
		Game();
		~Game() {}

		Game(Game const&) = delete;
		Game& operator = (Game const&) = delete;
		static std::shared_ptr<GameState> CreateGameState(State state, int score);
		DifficultyLevel m_difficulty;
		uint32_t m_settings;
		std::vector<std::shared_ptr<GameState>> m_gameStates;
		std::vector<std::shared_ptr<RecordItem>> m_recordList;
		const int m_MAX_SCORE = 10;
		sf::RenderWindow* m_window = nullptr;
	};
}