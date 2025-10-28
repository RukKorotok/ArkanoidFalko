#include "Game.h"
#include <bitset>


namespace Arkanoid
{
	//-----------------------------------------------------------------------------------------------------------
	Game::Game()
	{
		m_settings = 0b11;
		m_state = State::InMenu;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::StartGame()
	{
		m_currentScore = 0;
		m_scoreAdded = int(m_difficulty) * 2;
		m_state = State::InProgress;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::UpdateGame(float deltaTime)
	{
		
	}
	//-----------------------------------------------------------------------------------------------------------
	State Game::GetState()
	{
		return m_state;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::SetState(State state)
	{
		m_state = state;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::SetGameOver()
	{
		m_state = State::GameOver;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::ExitFromGame(sf::RenderWindow& window)
	{
		window.close();
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::DrawGame(sf::RenderWindow& window)
	{
		window.clear();
		Ball::GetInstance().DrawBall(window);
		UI::GetInstance().DrawGameUI(GetInstance(), window);
		Base::GetInstance().DrawBase(window);

		window.display();
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::ChangeGameSettings(SettingsMode settingMode)
	{

		// Set game settings
		if (m_settings & static_cast<uint32_t>(settingMode) )
		{
			m_settings &= ~static_cast<uint32_t>(settingMode);
		}
		else
		{
			m_settings |= static_cast<uint32_t>(settingMode);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::ChangeGameDifficulty(GameDifficultyLevel difficulty)
	{
		m_difficulty = difficulty;
	}
	int Game::GetScore()
	{
		return m_currentScore;
	}
	//-----------------------------------------------------------------------------------------------------------
	uint32_t Game::GetSetings()
	{
		return m_settings;
	}
	//-----------------------------------------------------------------------------------------------------------
	GameDifficultyLevel Game::GetDifficultyLevel()
	{
		return m_difficulty;
	}
}
