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
	State currentState = State::ExitGame;
	int seed = (int)time(nullptr);
	srand(seed);
	sf::Event event;

	// Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Arkanoid game!");

	Game::GetInstance().SetWindowRef(&window);
	// Game initialization

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

		if (Game::GetInstance().GetCurrentGameState()->GetState() == State::GameInProgress)
		{
			InputHandler::GetInstance().HandleInputInGame(float(sf::Mouse::getPosition(window).x));
			Game::GetInstance().GetCurrentGameState()->UpdateGame(window, deltaTime);
		}
		else
		{
			if (InputHandler::GetInstance().HandlerInputMenu(isKeysPressed))
			{
				Game::GetInstance().GetCurrentGameState()->UpdateGame(window, deltaTime);
				isKeysPressed = true;
			}
			if (currentState != Game::GetInstance().GetCurrentGameState()->GetState())
			{
				Game::GetInstance().GetCurrentGameState()->UpdateGame(window, deltaTime);
				currentState = Game::GetInstance().Game::GetInstance().GetCurrentGameState()->GetState();
			}
		}
		}
	return 0;
}
