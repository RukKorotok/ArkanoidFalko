#include "InputHandlers.h"
#include "Menu.h"
#include "Game.h"

namespace Arkanoid
{
	//------------------------------------------------------------------------------------------------------------
	void InputHandler::HandleInputInGame(float position)
	{
		// Change player direction
		if (position > 0 && position < SCREEN_WIDTH)
		{
			Game::GetInstance().GetCurrentGameState()->RefreshMousePosition(position);
		}
		if (sf::Keyboard::isKeyPressed(PAUSE))
		{
			Game::GetInstance().AddGameState(State::ResumeGame, Game::GetInstance().GetCurrentGameState()->GetScore());
		}
	}
	//------------------------------------------------------------------------------------------------------------
	bool InputHandler::HandlerInputMenu(bool isAllKeysRealised)
	{
		std::shared_ptr<Menu> currentMenu = UI::GetInstance().GetMenu()[UI::GetInstance().GetMenu().size() - 1];

		if (!isAllKeysRealised)
		{
			if (sf::Keyboard::isKeyPressed(DOWN_DIRECTION) )
			{
				currentMenu->SwitchMenuItem(true);
				return true;
			}
			else if (sf::Keyboard::isKeyPressed(UP_DIRECTION) )
			{
				currentMenu->SwitchMenuItem(false);
				return true;
			}
			else if (sf::Keyboard::isKeyPressed(GO_BACK) )
			{
				currentMenu->RemoveMenuStackItem();
				return true;
			}
			else if (sf::Keyboard::isKeyPressed(ENTER))
			{
				currentMenu->GetStack().top().find(currentMenu->GetMenuPointer())->second->ActionHandle();
				return true;
			}
		}
		return false;
	}
}
