#pragma once

namespace Arkanoid
{

	class InputHandler
	{
	public:

		static InputHandler& GetInstance()
		{
			static InputHandler inputHandler;
			return inputHandler;
		}

		void HandleInputInGame(float position);
		bool HandlerInputMenu(bool isAllKeysRealised);

	private:
		InputHandler() {}
		~InputHandler() {}

		InputHandler(InputHandler const&) = delete;
		InputHandler& operator = (InputHandler const&) = delete;
	};
}
