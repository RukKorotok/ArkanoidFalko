#include "Game.h"
#include <bitset>


namespace Arkanoid
{
	//GameState
	//-----------------------------------------------------------------------------------------------------------
	GameState::GameState(State state, int score)
	{
		switch (state)
		{
		case State::Main:
		{
			m_menu = UI::GetInstance().CreateMenu("Main");
			break;
		}
		case State::ResumeGame:
		{
			m_menu = UI::GetInstance().CreateMenu("PauseMenu");
			break;
		}
		case State::GameOver:
		{
			m_menu = UI::GetInstance().CreateMenu("GameOverMenu");
			break;
		}
		}
		m_state = state;
		m_score = score;
	}
	//-----------------------------------------------------------------------------------------------------------
	GameState::~GameState()
	{
		UI::GetInstance().RemoveMenu(*m_menu);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameState::UpdateGame(sf::RenderWindow& window, float deltaTime)
	{
		window.clear();
		UI::GetInstance().DrawMenu(window);
		window.display();
	}
	//-----------------------------------------------------------------------------------------------------------
	State GameState::GetState()
	{
		return m_state;
	}
	void GameState::SetScore(int score)
	{
		m_score = score;
	}
	//-----------------------------------------------------------------------------------------------------------
	int GameState::GetScore()
	{
		return m_score;
	}
	//GameStateInRuntime
	//-----------------------------------------------------------------------------------------------------------
	GameStateInRuntime::GameStateInRuntime(State state, int score) : GameState(state, score)
	{
		m_ball = new Ball();
		m_base = new Base();
	}
	//-----------------------------------------------------------------------------------------------------------
	GameStateInRuntime::~GameStateInRuntime()
	{
		delete m_ball;
		delete m_base;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::UpdateGame(sf::RenderWindow& window, float deltaTime)
	{
		window.clear();
		CheckBallCollitions();
		m_ball->UpdateBall(deltaTime);
		m_ball->DrawBall(window);
		m_base->DrawBase(window);
		UI::GetInstance().DrawGameUI(Game::GetInstance(), window);
		window.display();
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::RefreshMousePosition(float position)
	{
		m_base->UpdateBase(position);
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::CheckBallCollitions()
	{
		Position2D ballPosition = m_ball->GetPosition();
		Position2D basePosition = m_base->GetPosition();
		Position2D baseSize = m_base->GetBaseSize();

		if (Math::GetInstance().IsCicleRectangleCollition(ballPosition, BALL_SIZE * 0.5f, basePosition, baseSize))
		{
			m_ball->SetVectorSpeed(Math::GetInstance().CalculateReboundSpeedByBase(ballPosition.x, INITIAL_BALL_SPEED, basePosition.x, baseSize.x, BASE_REBOUND_MAX_ANGLE) );
			m_ball->SetPosition({ ballPosition.x, basePosition.y - (BALL_SIZE * 0.5f + BASE_SEGMENT_SIZE) } );
		}
	}
	//Game
	//-----------------------------------------------------------------------------------------------------------
	void Game::SetGameSettings(SettingsMode settingMode)
	{
		// Set game settings
		if (m_settings & static_cast<uint32_t>(settingMode))
		{
			m_settings &= ~static_cast<uint32_t>(settingMode);
		}
		else
		{
			m_settings |= static_cast<uint32_t>(settingMode);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::SetGameDifficulty(DifficultyLevel difficulty)
	{
		m_difficulty = difficulty;
	}
	//-----------------------------------------------------------------------------------------------------------
	uint32_t Game::GetSetings()
	{
		return m_settings;
	}
	//-----------------------------------------------------------------------------------------------------------
	DifficultyLevel Game::GetDifficultyLevel()
	{
		return m_difficulty;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<GameState> Game::GetCurrentGameState()
	{
		return m_gameStates[m_gameStates.size() - 1];
	}

	//-----------------------------------------------------------------------------------------------------------
	void Game::ExitFromGame()
	{
		m_window->close();
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::AddGameState(State state, int score)
	{
		m_gameStates.push_back(CreateGameState(state, score) );
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::RemoveGameState()
	{
		if (!m_gameStates.empty())
		{
			m_gameStates.pop_back();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::ResetGame()
	{
		m_gameStates.clear();
		AddGameState(State::Main, 0);
	}
	//-----------------------------------------------------------------------------------------------------------
	Game::Game()
	{
		AddGameState(State::Main, 0);
	}
	std::shared_ptr<GameState> Game::CreateGameState(State state, int score)
	{
		if (state == State::GameInProgress)
		{
			return std::make_shared<GameStateInRuntime>(state, score);
		}
		else
		{
			return std::make_shared<GameState>(state, score);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<RecordItem>> Game::ReadRecordsList()
	{
		std::string name = "";
		std::vector<std::shared_ptr<RecordItem>> list;
		std::ifstream file("../Saves/RecordList.txt");
		int score = 0;

		if (file.is_open())
		{
			while (file >> name >> score)
			{
				list.push_back(std::make_shared<RecordItem>(name, score) );
			}
			file.close();
		}
		return list;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::WriteRecordList(const std::vector<std::shared_ptr<RecordItem>> recordList)
	{
		std::ofstream file("../Saves/RecordList.txt");
		if (file.is_open())
		{
			for (const auto& entry : recordList) 
			{
				file << entry->GetName() << " " << entry->GetName() << std::endl;
			}
			file.close();
		}
		else 
		{
			std::cerr << "File dont open, ERROR!!!" << std::endl;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::AddRecordScore(const std::string& name, int newScore)
	{
		std::vector<std::shared_ptr<RecordItem>> scores = ReadRecordsList();
		std::shared_ptr<RecordItem> insertValue;
		std::vector<std::shared_ptr<RecordItem>> tempList = scores;
		scores.push_back(std::make_shared<RecordItem>(name, newScore) );
		int i = 0;
		int j = 0;

		for (i = 0; i < scores.size(); ++i)
		{
			insertValue = tempList[i];
			j = i - 1;

			while (j >= 0 && tempList[j]->GetScore() < insertValue->GetScore() )
			{
				tempList[j + 1] = tempList[j];
				j = j - 1;
			}
			tempList[j + 1] = insertValue;
		}
		scores = tempList;

		if (scores.size() > m_MAX_SCORE)
		{
			scores.resize(m_MAX_SCORE);
		}
		WriteRecordList(scores);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::SetWindowRef(sf::RenderWindow* window)
	{
		m_window = window;
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::RenderWindow* Game::GetWindow()
	{
		return m_window;
	}
}
