#pragma once
#include "SFML/Graphics.hpp"

namespace Arkanoid
{
	class Game;

	class InputHandler
	{
	public:

		static InputHandler& GetInstance()
		{
			static InputHandler inputHandler;
			return inputHandler;
		}

		void HandleInputInGame(float position);
		bool HandleInputOnGameEnded(Game& game, bool isAllKeysRealised, sf::Event event);
		bool HandlerInputMenu(bool isAllKeysRealised);

	private:
		InputHandler() {}
		~InputHandler() {}

		InputHandler(InputHandler const&) = delete;
		InputHandler& operator = (InputHandler const&) = delete;
	};
}
