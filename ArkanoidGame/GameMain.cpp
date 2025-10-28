#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Config.h"
#include "Game.h"
#include <memory>

//-----------------------------------------------------------------------------------------------------------
int main()
{
	using namespace Arkanoid;
	bool isKeysPressed = false;
	sf::Keyboard::Key pressedKey = sf::Keyboard::Unknown;
	State currentState = State::Undefined;
	int seed = (int)time(nullptr);
	srand(seed);
	sf::Event event;

	// Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake game!");

	// Game initialization
	UI::GetInstance().InitRecordsList();
	Audio::GetInstance().InitAudio();
	
	// Init game clocks
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	// Main loop
	while (window.isOpen())
	{
		// Reduce framerate to not spam CPU and GPU
		sf::sleep(sf::milliseconds(16));

		// Calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Read events
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				pressedKey = event.key.code;
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == pressedKey)
			{
				isKeysPressed = false;

			}
		}

		// Check game states
		switch (Game::GetInstance().GetState())
		{
		case State::InMenu:
		{
			if (currentState != Game::GetInstance().GetState())
			{
				UI::GetInstance().DrawMenu(window);
				Audio::GetInstance().PlaySound(SoundFile::IntroSound);
				currentState = Game::GetInstance().GetState();
			}
			if (InputHandler::GetInstance().HandlerInputMenu(isKeysPressed))
			{
				UI::GetInstance().DrawMenu(window);
				isKeysPressed = true;
			}

			break;
		}
		case State::InProgress:
		{
			currentState = Game::GetInstance().GetState();
			InputHandler::GetInstance().HandleInputInGame(float(sf::Mouse::getPosition(window).x) );
			Game::GetInstance().UpdateGame(deltaTime);
			Ball::GetInstance().UpdateBall(deltaTime);
			Game::GetInstance().DrawGame(window);
			break;
		}
		case State::Paused:
		{
			if (currentState != Game::GetInstance().GetState())
			{
				Audio::GetInstance().PlaySound(SoundFile::PauseSound);
				currentState = Game::GetInstance().GetState();
			}
			if (InputHandler::GetInstance().HandlerInputMenu(isKeysPressed))
			{
				isKeysPressed = true;
				Game::GetInstance().DrawGame(window);
			}
			break;
		}
		case State::GameOver:
		{
			GridManager::GetInstance().CleareEventMap();

			if (currentState != Game::GetInstance().GetState())
			{
				if (UI::GetInstance().IsSetNewRecord(Game::GetInstance()))
				{
					UI::GetInstance().RefreshRecordList(Game::GetInstance());
				}
				else
				{

				}
				Audio::GetInstance().PlaySound(SoundFile::DeathSound);
				currentState = Game::GetInstance().GetState();
			}
			if (InputHandler::GetInstance().HandlerInputMenu(isKeysPressed))
			{
				isKeysPressed = true;
			}
			break;
		}
		case State::ExitGame:
		{
			Game::GetInstance().ExitFromGame(window);
			break;
		}
		}
	}
	return 0;
}
