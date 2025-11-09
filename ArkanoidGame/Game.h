#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <fstream>
#include "SFML/Audio.hpp"
#include "Config.h"
#include "Math.h"
#include "UI.h"
#include "Base.h"
#include "Ball.h"
#include "Block.h"
#include "Sound.h"
#include "InputHandlers.h"
#include "Objects.h"

namespace Arkanoid
{
	class RecordItem;

	class GameState
	{
	public:
		GameState(State state, int score);
		~GameState();

		virtual void UpdateGame(sf::RenderWindow& window, float deltaTime);
		virtual void RefreshMousePosition(float position) {}
		State GetState();
		void AddScore(int score);
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
		void AddPrimaryBall(Vector2D ballSize, Vector2D ballPosition, Vector2D ballDirection);
		void RemoveObject(GameObject& object);
		void AddAdditionalBall(std::shared_ptr<Ball> ball);
		void RemoveAdditionalBall(Ball& ball);
		void SetPoison();
		void SetDesorient();
		Base* GetBase();
		std::vector<std::shared_ptr<Block>> GetBlocks();

	private:

		void LoadLevel(const std::string& fileName);
		void RemoveObjectByAdress(std::vector<std::shared_ptr<GameObject>> objects, GameObject& adress);
		Base* m_base = nullptr;
		std::vector<std::shared_ptr<MainBall>> m_primaryBalls;
		std::vector<std::shared_ptr<Ball>> m_additionalBalls;
		std::vector<std::shared_ptr<Block>> m_blocks;
		bool m_isPoisoned = false;
		bool m_isDisoriented = false;
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
		std::string GetLevelPath(int index);

	private:

		Game();
		~Game() {}

		Game(Game const&) = delete;
		Game& operator = (Game const&) = delete;
		static std::shared_ptr<GameState> CreateGameState(State state, int score);

		std::string m_LEVELS_PATH = "../Levels";
		std::vector<std::string> m_levelsPaths;
		DifficultyLevel m_difficulty;
		uint32_t m_settings;
		std::vector<std::shared_ptr<GameState>> m_gameStates;
		std::vector<std::shared_ptr<RecordItem>> m_recordList;
		const int m_MAX_SCORE = 10;
		sf::RenderWindow* m_window = nullptr;
	};
}