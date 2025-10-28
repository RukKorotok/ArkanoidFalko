#include "InputHandlers.h"
#include "Math.h"
#include "Game.h"

namespace Arkanoid
{
	//------------------------------------------------------------------------------------------------------------
	void InputHandler::HandleInputInGame(float position)
	{
		// Change player direction
		if (position > 0 && position < SCREEN_WIDTH)
		{
			Base::GetInstance().RefreshBasePosition(position);
		}
		if (sf::Keyboard::isKeyPressed(PAUSE))
		{
			UI::GetInstance().CreateMenu(MenuType::PauseMenu);
			Game::GetInstance().SetState(State::Paused);
		}
	}
	//------------------------------------------------------------------------------------------------------------
	bool HandleInputOnGameEnded(Game& game, bool isAllKeysRealised, sf::Event event)
	{
		/*ParticipantString nameText;

		if (!isAllKeysRealised)
		{
			if (sf::Keyboard::isKeyPressed(DOWN_DIRECTION) )
			{
				UI::GetInstance().GetMenu()->SwitchMenuItem(true);
				return true;
			}
			else if (sf::Keyboard::isKeyPressed(UP_DIRECTION) )
			{
				UI::GetInstance().GetMenu()->SwitchMenuItem(false);
				return true;
			}
			else if (sf::Keyboard::isKeyPressed(ENTER))
			{
				switch (game.ui.menuStack.top().menuItems[game.ui.menuPointer].second)
				{
				case InMainMenu:
				{
					game.ui.menuPointer = 0;
					game.ui.menuStack.pop();
					game.ui.inputString.clear();
					game.state = State::MainMenu;
					break;
				}
				case CloseGame:
				{
					game.state = State::ExitGame;
					break;
				}
				case AcceptSubMenu:
				{
					if (!game.ui.drawPopUpMenu)
					{
						game.ui.drawPopUpMenu = true;
						game.ui.menuPointer = 0;
					}
					else
					{
						game.ui.drawPopUpMenu = false;
						game.ui.menuPointer = 0;
						game.ui.menuStack.pop();
						game.ui.menuStack.push({ GameOver, GAME_OVER_ITEMS, sizeof(GAME_OVER_ITEMS) / sizeof(GAME_OVER_ITEMS[0]), 0 });
						nameText = game.ui.recordListText.find(0)->second;
						nameText.participant = game.ui.inputString;
						nameText.score = game.score;
						SetStringInText(nameText.listText, std::to_string(1) + "  " + nameText.participant + " --- " + std::to_string(nameText.score) );
						game.ui.recordListText[0] = nameText;
					}
					break;
				}
				case GoBack:
				{
					game.ui.drawPopUpMenu = false;
					game.ui.menuPointer = 0;
					game.ui.menuStack.pop();
					game.ui.menuStack.push({ GameOver, GAME_OVER_ITEMS, sizeof(GAME_OVER_ITEMS) / sizeof(GAME_OVER_ITEMS[0]), 0 });
					break;
				}
				}
				return true;;
			}

			else if (event.type == sf::Event::TextEntered && event.text.unicode < 128 && game.ui.drawPopUpMenu)
			{
				game.ui.inputString += static_cast<char>(event.text.unicode);
				game.ui.nameInputText.setString(INPUT_TITLE + game.ui.inputString);
				return true;
			}
		}
		return false;*/
		return false;
	}
	//------------------------------------------------------------------------------------------------------------
	bool InputHandler::HandlerInputMenu(bool isAllKeysRealised)
	{
		Menu* currentMenu = UI::GetInstance().GetMenu()[UI::GetInstance().GetMenu().size() - 1];

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
				currentMenu->GoBack();
				return true;
			}
			else if (sf::Keyboard::isKeyPressed(ENTER))
			{
				switch (currentMenu->GetStack().top().menuActiveItems.items[currentMenu->GetMenuPointer()].event)
				{
				case GameStart:
				{
					Game::GetInstance().StartGame();
					Audio::GetInstance().StopSound(SoundFile::IntroSound);
					break;
				}
				case CloseGame:
				{
					UI::GetInstance().CreateMenu(MenuType::AcceptMenu);
					break;
				}
				case GoToDifficultyLevel:
				{
					currentMenu->AddMenuStackItem({ MenuStates::DifficultyLevel, currentMenu->GetMenuActiveItemList().find(MenuStates::DifficultyLevel)->second, currentMenu->GetMenuPassiveItemList().find(MenuStates::DifficultyLevel)->second, 
						int(currentMenu->GetMenuActiveItemList().find(MenuStates::DifficultyLevel)->second.items.size()),  currentMenu->GetMenuPointer() });

					currentMenu->SetMenuPointer(0);
					break;
				}
				case OpenRecordList:
				{
					UI::GetInstance().CreateMenu(MenuType::RecordListMenu);
					break;
				}
				case GoToSettings:
				{
					currentMenu->AddMenuStackItem({ currentMenu->GetStack().top().state, currentMenu->GetMenuActiveItemList().find(MenuStates::Settings)->second, currentMenu->GetMenuPassiveItemList().find(MenuStates::Settings)->second, 
						int(currentMenu->GetMenuActiveItemList().find(MenuStates::Settings)->second.items.size()), currentMenu->GetMenuPointer() });

					currentMenu->SetMenuPointer(0);
					break;
				}
				case SetSound:
				{
					Game::GetInstance().ChangeGameSettings(SettingsMode::SoundOn);
					break;
				}
				case SetMusic:
				{
					Game::GetInstance().ChangeGameSettings(SettingsMode::MusicOn);
					break;
				}
				case SetEasyDifficulty:
				{
					Game::GetInstance().ChangeGameDifficulty(GameDifficultyLevel::Easy);
					break;
				}
				case SetNotEasyDifficulty:
				{
					Game::GetInstance().ChangeGameDifficulty(GameDifficultyLevel::NotEasy);
					break;
				}
				case SetMediumDifficulty:
				{
					Game::GetInstance().ChangeGameDifficulty(GameDifficultyLevel::Medium);
					break;
				}
				case SetNotMediumDifficulty:
				{
					Game::GetInstance().ChangeGameDifficulty(GameDifficultyLevel::NotMedium);
					break;
				}
				case SetHardDifficulty:
				{
					Game::GetInstance().ChangeGameDifficulty(GameDifficultyLevel::Hard);
					break;
				}
				case AcceptSubMenu:
				{
					Menu* menu = UI::GetInstance().GetMenu()[0];
					switch (UI::GetInstance().GetMenu()[0]->GetStack().top().menuActiveItems.items[UI::GetInstance().GetMenu()[0]->GetMenuPointer()].event)
					{
					case CloseGame:
					{
						Game::GetInstance().SetState(State::ExitGame);
					}
					}
					break;
				}
				case GoBack:
				{
					if (currentMenu->GetStack().size() > 1)
					{
						currentMenu->SetMenuPointer(currentMenu->GetStack().top().previewPointer);
						currentMenu->RemoveMenuStackItem();
					}
					else
					{
						UI::GetInstance().RemoveMenu(currentMenu);
					}
					break;
				}
				}
				return true;
			}
		}
		return false;
	}
}
