#include "Game.h"
#include "Menu.h"
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
		case State::Win:
		{
			m_menu = UI::GetInstance().CreateMenu("WinMenu");
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
	//-----------------------------------------------------------------------------------------------------------
	void GameState::AddScore(int score)
	{
		m_score = m_score + score;
	}
	//-----------------------------------------------------------------------------------------------------------
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
		Vector2D ballSize = { BALL_SIZE, BALL_SIZE };
		Vector2D ballPosition = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
		Vector2D ballDirection = { 0.0f, -1.0f };
		LoadLevel(Game::GetInstance().GetLevelPath(0));
		m_base = new Base();
		AddPrimaryBall(ballSize, ballPosition, ballDirection);
	}
	//-----------------------------------------------------------------------------------------------------------
	GameStateInRuntime::~GameStateInRuntime()
	{
		m_primaryBalls.clear();
		delete m_base;
		m_additionalBalls.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::UpdateGame(sf::RenderWindow& window, float deltaTime)
	{
		window.clear();
		for (const auto block : m_blocks)
		{
			block->Visualize(window);
		}
		for (const auto ball : m_primaryBalls)
		{
			ball->UpdateBall(deltaTime, window);
		}
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
	void GameStateInRuntime::AddPrimaryBall(Vector2D ballSize, Vector2D ballPosition, Vector2D ballDirection)
	{
		m_primaryBalls.push_back(std::make_shared<MainBall>(ballSize, ballPosition, ballDirection));
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::RemoveObject(GameObject& object)
	{
		int i = 0;
		GameObject* objectAdress = &object;
		for (i = 0; i < m_additionalBalls.size(); i++)
		{
			if (m_additionalBalls[i].get() == objectAdress)
			{
				m_additionalBalls.erase(m_additionalBalls.begin() + i);
				return;
			}
		}

		for (i = 0; i < m_primaryBalls.size(); i++)
		{
			if (m_primaryBalls[i].get() == objectAdress)
			{
				m_primaryBalls.erase(m_primaryBalls.begin() + i);

				if (m_primaryBalls.empty())
				{
					Game::GetInstance().AddGameState(GameOver, Game::GetInstance().GetCurrentGameState()->GetScore());
				}
				return;
			}
		}

		for (i = 0; i < m_blocks.size(); i++)
		{
			if (m_blocks[i].get() == objectAdress)
			{
				m_blocks.erase(m_blocks.begin() + i);

				if (m_blocks.empty())
				{
					Game::GetInstance().AddGameState(Win, Game::GetInstance().GetCurrentGameState()->GetScore());
				}
				return;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::AddAdditionalBall(std::shared_ptr<Ball> ball)
	{
		m_additionalBalls.push_back(ball);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::SetPoison()
	{
		m_isPoisoned = true;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::SetDesorient()
	{
		m_isDisoriented = true;
	}
	//-----------------------------------------------------------------------------------------------------------
	Base* GameStateInRuntime::GetBase()
	{
		return m_base;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<Block>> GameStateInRuntime::GetBlocks()
	{
		return m_blocks;
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::LoadLevel(const std::string& fileName)
	{
		const Vector2D size = { BASE_SEGMENT_SIZE * 2, BASE_SEGMENT_SIZE };
		std::ifstream file(fileName);
		std::string line;
		int row = 0;
		Vector2D position;
		char symbol;

		//Open file
		if (!file.is_open())
		{
			std::cerr << "Error, not open level: " << fileName << std::endl;
			return;
		}
		//Get line
		while (std::getline(file, line))
		{
			for (int c = 0; c < line.length(); c++)
			{
				symbol = line[c];
				//Check symbols
				if (symbol != '#')
				{
					position = { c * BASE_SEGMENT_SIZE * 2 + BASE_SEGMENT_SIZE, row * BASE_SEGMENT_SIZE + BASE_SEGMENT_SIZE * 0.5f };

					if (symbol == '1')
					{
						m_blocks.emplace_back(std::make_shared <Block>(1, size, position));
					}
				}
			}
			row++;
		}
		// Close file
		file.close();
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::RemoveObjectByAdress(std::vector<std::shared_ptr<GameObject>> objects, GameObject& adress)
	{
		int i = 0;
		for (i = 0; i < objects.size(); i++)
		{
			if (objects[i].get() == &adress)
			{
				objects.erase(objects.begin() + i);
				return;
			}
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
				file << entry->GetString() << " " << entry->GetString() << std::endl;
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
	//-----------------------------------------------------------------------------------------------------------
	std::string Game::GetLevelPath(int index)
	{
		if (m_levelsPaths.size() > index)
		{
			return m_levelsPaths[index];
		}
		std::cerr << "Level not found: " << m_LEVELS_PATH << std::endl;
		return "";
	}
	//-----------------------------------------------------------------------------------------------------------
	Game::Game()
	{
		//Find all paths for levels files
		try
		{
			std::filesystem::path folderPath(m_LEVELS_PATH);

			if (std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath))
			{
				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(folderPath))
				{
					if (std::filesystem::is_regular_file(entry.status()))
					{
						// Added path in paths list
						m_levelsPaths.push_back(entry.path().string());
					}
				}
			}
			else
			{
				std::cerr << "Folder not found: " << m_LEVELS_PATH << std::endl;
			}
		}
		catch (const std::filesystem::filesystem_error& error)
		{
			std::cerr << "File system error: " << error.what() << std::endl;
		}
		//Create main menu
		AddGameState(State::Main, 0);
	}
	//-----------------------------------------------------------------------------------------------------------
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
}
