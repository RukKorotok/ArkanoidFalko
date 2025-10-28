#include "UI.h"
#include "Game.h"

namespace Arkanoid
{
	//-----------------------------------------------------------------------------------------------------------
	Menu::Menu(MenuType type)
	{
		MenuStates currentState = MenuStates::MainMenu;
		MenuItemsList passiveItems;
		std::map<int, ParticipantString> recordList;
		std::vector<std::string > currentPassiveItems;

		assert(m_font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));
		switch (type)
		{
		case MenuType::StartMenu:
		{
			currentState = MenuStates::MainMenu;

			m_menuActiveItemsList[MenuStates::MainMenu] = { {"Mad balL"},
				{
					{MenuEvents::GameStart, "Start game", false },
					{MenuEvents::GoToDifficultyLevel, "Set difficulty level", false },
					{MenuEvents::OpenRecordList, "Records list", false},
					{MenuEvents::GoToSettings, "Settings", false},
					{MenuEvents::CloseGame, "Exit", false}
				}
			};
			m_menuPassiveItemsList[MenuStates::MainMenu] = {};

			m_menuActiveItemsList[MenuStates::RecordsList] = {{"Settings"},
				{
					{MenuEvents::GoBack, "Back", false}
				}
			};
			m_menuPassiveItemsList[MenuStates::RecordsList] = {};

			m_menuActiveItemsList[MenuStates::DifficultyLevel] = {{"Difficulty level"},
				{
					{MenuEvents::SetEasyDifficulty, "Easy", true},
					{MenuEvents::SetNotEasyDifficulty, "Heavier than simple", true},
					{MenuEvents::SetMediumDifficulty, "Medium", true},
					{MenuEvents::SetNotMediumDifficulty, "Heavier than middle", true},
					{MenuEvents::SetHardDifficulty, "Hard", true},
					{MenuEvents::GoBack, "Back", false}
				}
			};
			m_menuPassiveItemsList[MenuStates::DifficultyLevel] = {};

			m_menuActiveItemsList[MenuStates::Settings] = { {"Difficulty level"},
				{
					{MenuEvents::SetSound, "Sound", true},
					{MenuEvents::SetMusic, "Music", true},
					{MenuEvents::GoBack, "Back", false}
				}
			};
			m_menuPassiveItemsList[MenuStates::Settings] = {};

			break;
		}
		case MenuType::PauseMenu:
		{
			currentState = MenuStates::Pause;

			m_menuActiveItemsList[MenuStates::Pause] = {{"Pause"},
				{
					{MenuEvents::GoBack, "resume game", false },
					{MenuEvents::InMainMenu, "In main menu", false },
				}
			};
			m_menuPassiveItemsList[MenuStates::Pause] = {};

			break;
		}
		case MenuType::AfterDeathMenu:
		{
			currentState = MenuStates::Death;

			m_menuActiveItemsList[MenuStates::Death] = {{"Game Over"},
				{
					{MenuEvents::InMainMenu, "In main menu", false},
					{MenuEvents::CloseGame, "Exit", false}
				}
			};
			m_menuPassiveItemsList[MenuStates::Death] = {};

			break;
		}
		case MenuType::RecordListMenu:
		{
			currentState = MenuStates::RecordsList;

			m_menuActiveItemsList[MenuStates::RecordsList] = { {"Records list"},
				{
					{MenuEvents::GoBack, "Back", false}
				}
			};

			recordList = UI::GetInstance().GetRecordList();
			for (std::pair<int, ParticipantString> currentItem : recordList)
			{
				currentPassiveItems.push_back(std::to_string(currentItem.first) + " - " + currentItem.second.participant + " " + std::to_string(currentItem.second.score));
			}
			m_menuPassiveItemsList[currentState] = currentPassiveItems;
			break;
		}
		case MenuType::AcceptMenu:
		{
			currentState = MenuStates::Accept;

			m_menuActiveItemsList[MenuStates::Accept] = { {"Are you sure"},
				{
					{MenuEvents::AcceptSubMenu, "Yes", false},
					{MenuEvents::GoBack, "No", false}
				}
			};
			m_menuPassiveItemsList[MenuStates::Accept] = {};

			break;
		}
		}
		m_menuStack.push({ currentState, m_menuActiveItemsList.find(currentState)->second, m_menuPassiveItemsList.find(currentState)->second,
			int(m_menuActiveItemsList.find(currentState)->second.items.size()), 0 });
		m_menuPointer = 0;
	}
	//-----------------------------------------------------------------------------------------------------------
	Menu::~Menu()
	{
		m_menuItemsText.clear();
		m_menuItems.clear();
		m_menuActiveItemsList.clear();
		m_menuPassiveItemsList.clear();
		while (!m_menuStack.empty())
		{
			m_menuStack.pop();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::SwitchMenuItem(bool down)
	{
		if (down == true && m_menuPointer < m_menuStack.top().pointsCount - 1)
		{
			m_menuPointer++;
		}
		else if (down == false && m_menuPointer > 0)
		{
			m_menuPointer--;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::GoBack()
	{
		if (m_menuStack.size() > 1)
		{
			m_menuPointer = m_menuStack.top().previewPointer;
			m_menuStack.pop();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	std::stack<MenuStateInfo> Menu::GetStack()
	{
		return m_menuStack;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::AddMenuStackItem(MenuStateInfo menuItem)
	{
		m_menuStack.push(menuItem);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::RemoveMenuStackItem()
	{
		m_menuStack.pop();
	}
	//-----------------------------------------------------------------------------------------------------------
	int Menu::GetMenuPointer()
	{
		return m_menuPointer;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::SetMenuPointer(int pointer)
	{
		m_menuPointer = pointer;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::map<MenuStates, MenuItemsList> Menu::GetMenuActiveItemList()
	{
		return m_menuActiveItemsList;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::map<MenuStates, std::vector<std::string>> Menu::GetMenuPassiveItemList()
	{
		return m_menuPassiveItemsList;
	}
	//-----------------------------------------------------------------------------------------------------------
	UI::UI()
	{
		Menu* menu = new Menu(MenuType::StartMenu);
		m_currentMenuList.push_back(menu);

		assert(m_mainMenueTexture.loadFromFile(RESOURCES_PATH + "\\GameStart.png"));
		assert(m_font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		int i = 0;
		float mainMenueSpriteSize = SCREEN_HEIGHT * 0.9f;
		float menuIndentation = SCREEN_WIDTH - 250.0f;

		// Init UI sprites
		InitSprite(m_mainMenuSprite, m_mainMenueTexture, 0.38f, 0.35f);

		// Init UI text
		InitText(m_gameText, m_font, SCORE_TITLE, sf::Color::Yellow, SCREEN_WIDTH - 100.0f, 5.0f);
		InitText(m_nameInputText, m_font, INPUT_TITLE, sf::Color::Yellow, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f);
	}
	//-----------------------------------------------------------------------------------------------------------
	UI::~UI()
	{
		m_currentMenuList.clear();
		m_recordListText.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::DrawMenu(sf::RenderWindow& window)
	{
		sf::Text itemText;
		sf::RectangleShape checkboxFraim;
		sf::RectangleShape checkbox;
		sf::Color currentColor = sf::Color::White;
		Menu* currentMenu;
		std::stack<MenuStateInfo> currentMenuStack;
		int menuPointsCount = 0;
		int passiveIttemIndex = 0;
		int activeItenIndex = 0;
		int menuIndex = 0;

		float maxYPos = 0;
		float currentYPos = 0;

		window.clear();

		for (menuIndex = 0; menuIndex < m_currentMenuList.size(); menuIndex++)
		{
			currentMenu = m_currentMenuList[menuIndex];
			currentMenuStack = currentMenu->GetStack();
			// check menu state
			switch (currentMenuStack.top().state)
			{
			case MenuStates::MainMenu:
			{
				window.draw(m_mainMenuSprite);
				break;
			}
			case MenuStates::DifficultyLevel:
			{

				break;
			}
			case MenuStates::RecordsList:
			{

				break;
			}
			case MenuStates::Settings:
			{

				break;
			}
			}

			if (currentMenuStack.top().menuPassiveItems.size() > 0)
			{
				for (passiveIttemIndex = 0; passiveIttemIndex < currentMenuStack.top().menuPassiveItems.size() - 1; passiveIttemIndex++)
				{
					UI::GetInstance().InitText(itemText, m_font, currentMenuStack.top().menuPassiveItems[passiveIttemIndex], currentColor, m_MENU_ITEMS_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_ITEMS_INDETATION.y + 50.0f * passiveIttemIndex);
					window.draw(itemText);
				}
			}

			// Draw menu
			for (activeItenIndex = 0; activeItenIndex < currentMenuStack.top().menuActiveItems.items.size(); activeItenIndex++)
			{
				if (currentMenu->GetMenuPointer() == activeItenIndex)
				{
					currentColor = sf::Color::Green;
				}
				else
				{
					currentColor = sf::Color::Yellow;
				}
				UI::GetInstance().InitText(itemText, m_font, currentMenuStack.top().menuActiveItems.items[activeItenIndex].menuPointTitle, currentColor, m_MENU_ITEMS_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_ITEMS_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex) );
				window.draw(itemText);

				if (currentMenuStack.top().menuActiveItems.items[activeItenIndex].isHaveCheckBox)
				{
					UI::GetInstance().InitShape(checkboxFraim, CHECKBOX_SIZE, CHECKBOX_SIZE, 1.1f, sf::Color::White, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_CHECKBOX_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex));
					window.draw(checkboxFraim);

					switch (currentMenuStack.top().menuActiveItems.items[activeItenIndex].event)
					{
					case MenuEvents::SetSound:
					{

						if (!(Game::GetInstance().GetSetings() & static_cast<uint32_t>(SettingsMode::SoundOn)))
						{
							UI::GetInstance().InitShape(checkbox, CHECKBOX_SIZE, CHECKBOX_SIZE, 1.0f, sf::Color::Black, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_CHECKBOX_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex));
							window.draw(checkbox);
						}
						break;
					}
					case MenuEvents::SetMusic:
					{
						if (!(Game::GetInstance().GetSetings() & static_cast<uint32_t>(SettingsMode::MusicOn)))
						{
							UI::GetInstance().InitShape(checkbox, CHECKBOX_SIZE, CHECKBOX_SIZE, 1.0f, sf::Color::Black, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_CHECKBOX_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex));
							window.draw(checkbox);
						}
						break;
					}
					case MenuEvents::SetEasyDifficulty:
					{
						if (Game::GetInstance().GetDifficultyLevel() == Easy)
						{
							UI::GetInstance().InitShape(checkbox, CHECKBOX_SIZE, CHECKBOX_SIZE, 1.0f, sf::Color::Black, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_CHECKBOX_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex));
							window.draw(checkbox);
						}
						break;
					}
					case MenuEvents::SetNotEasyDifficulty:
					{
						if (Game::GetInstance().GetDifficultyLevel() == NotEasy)
						{
							UI::GetInstance().InitShape(checkbox, CHECKBOX_SIZE, CHECKBOX_SIZE, 1.0f, sf::Color::Black, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_CHECKBOX_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex));
							window.draw(checkbox);
						}
						break;
					}
					case MenuEvents::SetMediumDifficulty:
					{
						if (Game::GetInstance().GetDifficultyLevel() == Medium)
						{
							UI::GetInstance().InitShape(checkbox, CHECKBOX_SIZE, CHECKBOX_SIZE, 1.0f, sf::Color::Black, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_CHECKBOX_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex));
							window.draw(checkbox);
						}
						break;
					}
					case MenuEvents::SetNotMediumDifficulty:
					{
						if (Game::GetInstance().GetDifficultyLevel() == NotMedium)
						{
							UI::GetInstance().InitShape(checkbox, CHECKBOX_SIZE, CHECKBOX_SIZE, 1.0f, sf::Color::Black, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_CHECKBOX_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex));
							window.draw(checkbox);
						}
						break;
					}
					case MenuEvents::SetHardDifficulty:
					{
						if (Game::GetInstance().GetDifficultyLevel() == Hard)
						{
							UI::GetInstance().InitShape(checkbox, CHECKBOX_SIZE, CHECKBOX_SIZE, 1.0f, sf::Color::Black, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * menuIndex, m_MENU_CHECKBOX_INDETATION.y + 50.0f * (activeItenIndex + passiveIttemIndex));
							window.draw(checkbox);
						}
						break;
					}
					}
				}
			}
			window.display();
			////draw record list and input text
	//if (game.ui.drawPopUpMenu)
	//{
	//	window.draw(game.ui.nameInputText);
	//}
	//else
	//{
	//	i = 0;
	//	for (const std::pair<int, ParticipantString> pair : game.ui.recordListText)
	//	{
	//		if (i < 5)
	//		{
	//			window.draw(pair.second.listText);
	//		}
	//		i++;
	//	}
	//}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::InitSprite(sf::Sprite& sprite, const sf::Texture& texture, float heightMulty, float widthMulty)
	{
		float mainMenueSpriteSize = SCREEN_HEIGHT * 0.9f;
		float textXIndentation = SCREEN_WIDTH - 400.0f;

		sprite.setTexture(texture);
		Math::GetInstance().SetSpriteSize(sprite, mainMenueSpriteSize, mainMenueSpriteSize);
		Math::GetInstance().SetSpriteRelativeOrigin(sprite, 0.5f, 0.5f);
		sprite.setPosition(SCREEN_HEIGHT * heightMulty, SCREEN_WIDTH * widthMulty);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::InitShape(sf::RectangleShape& shape, float shapeSizeX, float shapeSizeY, float sizeMulty, const sf::Color& color, float height, float width)
	{
		float shapeSize = shapeSizeX * sizeMulty;

		shape.setOrigin(shapeSizeX * 0.5f, shapeSizeY * 0.5f);
		shape.setSize({ shapeSizeX * sizeMulty, shapeSizeY * sizeMulty });
		shape.setPosition(height, width);
		shape.setFillColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::InitText(sf::Text& text, const sf::Font& font, const std::string title, const sf::Color& color, const float heigh, const float width)
	{
		text.setFont(font);
		text.setCharacterSize(TEXT_SIZE - 5);
		text.setString(title);
		text.setFillColor(color);
		text.setPosition(heigh, width);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::InitRecordsList()
	{
		int score = 0;
		ParticipantString participantString;
		const int recordListLenght = sizeof(OPPONENTS_NAMES_ITEMS) / sizeof(OPPONENTS_NAMES_ITEMS[0]);
		std::pair<std::string, int> tempList[recordListLenght];

		//Generate opponents score
		for (int i = 0; i < recordListLenght; ++i)
		{
			score = rand() % MAX_OPPONENTS_SCORE + 1;
			tempList[i] = { OPPONENTS_NAMES_ITEMS[i], score };
		}

		//Sort
		std::sort(tempList, tempList + recordListLenght, [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b)
			{
				return a.second > b.second;
			});

		//Add list map
		for (int i = 0; i < recordListLenght; i++)
		{
			participantString.participant = tempList[i].first;
			participantString.score = tempList[i].second;

			m_recordListText[i] = participantString;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::RefreshRecordList(Game& game)
	{
		const int recordList = int(m_recordListText.size()) + 1;
		int i = 0;
		int j = 0;
		std::vector<ParticipantString> tempList;
		tempList.resize(recordList);
		ParticipantString insertValue;

		// Get values
		for (const std::pair<int, ParticipantString> pair : m_recordListText)
		{
			tempList[i] = pair.second;
			++i;
		}
		insertValue.participant = PLAYER_NAME;
		insertValue.score = game.GetScore();
		tempList[i] = insertValue;

		m_recordListText.clear();

		//Insertion sort
		for (i = 1; i < recordList; ++i)
		{
			insertValue = tempList[i];
			j = i - 1;

			while (j >= 0 && tempList[j].score < insertValue.score)
			{
				tempList[j + 1] = tempList[j];
				j = j - 1;
			}
			tempList[j + 1] = insertValue;
		}

		// Record list filling
		for (i = 0; i < recordList; ++i)
		{
			m_recordListText[i] = tempList[i];
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	std::map<int, ParticipantString> UI::GetRecordList()
	{
		return m_recordListText;
	}
	//-----------------------------------------------------------------------------------------------------------
	bool UI::IsSetNewRecord(Game& game)
	{
		return game.GetScore() > m_recordListText.find(0)->second.score;
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::DrawGameUI(Game& game, sf::RenderWindow& window)
	{
		m_gameText.setString(SCORE_TITLE + std::to_string(game.GetScore()));
		window.draw(m_gameText);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::SetStringInText(sf::Text& text, std::string string)
	{
		text.setString(string);
	}
	//-----------------------------------------------------------------------------------------------------------
	void SetShapeColor(sf::RectangleShape& button, const sf::Color& color)
	{
		button.setFillColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	void SetTextColor(sf::Text& text, const sf::Color& color)
	{
		text.setFillColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::CreateMenu(MenuType menuType)
	{
		Menu* menu = new Menu(menuType);
		m_currentMenuList.push_back(menu);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::RemoveMenu(Menu* menu)
	{
		int i = 0;

		for (i = 0; i < m_currentMenuList.size(); i++)
		{		
			if (m_currentMenuList[i] == menu)
			{
				delete m_currentMenuList[i];
				m_currentMenuList.erase(m_currentMenuList.begin() + i);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<Menu*> UI::GetMenu()
	{
		return m_currentMenuList;
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Font UI::GetFont()
	{
		return m_font;
	}
}
