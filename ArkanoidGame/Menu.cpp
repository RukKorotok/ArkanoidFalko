#include "Menu.h"

namespace Arkanoid
{
	//RecordItem
	//-----------------------------------------------------------------------------------------------------------
	RecordItem::RecordItem(std::string name, int score) : MenuItem(name)
	{
		m_score = score;
	}
	//-----------------------------------------------------------------------------------------------------------
	void RecordItem::Visualize(sf::RenderWindow& window, Vector2D tabIndex)
	{
		Draw(m_string + "  " + std::to_string(m_score), sf::Color::White, window, tabIndex);
	}
	//-----------------------------------------------------------------------------------------------------------
	int RecordItem::GetScore()
	{
		return m_score;
	}
	//MenuItem
	//-----------------------------------------------------------------------------------------------------------
	MenuItemWithAction::MenuItemWithAction(std::string text, std::string action) : MenuItem(text)
	{
		m_action = action;
	}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItemWithAction::Visualize(sf::RenderWindow& window, Vector2D tabIndex)
	{
		sf::Color color;
		if (m_action == "")
		{
			color = sf::Color::White;
		}
		else
		{
			if (m_selected)
			{
				color = sf::Color::Green;
			}
			else
			{
				color = sf::Color::Yellow;
			}
		}
		Draw(m_string, color, window, tabIndex);
	}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItemWithAction::ActionHandle()
	{
		std::vector<std::shared_ptr<Menu>> menu = UI::GetInstance().GetMenu();
		menu[menu.size() - 1]->SetMenuPointer(1);

		if (m_action == " ><|")
		{
			menu[menu.size() - 1]->RemoveMenuStackItem();
		}
		else
		{
			menu[menu.size() - 1]->AddMenuItems(m_action, UI::GetInstance().GetReader());
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItemWithAction::SetSelection(bool selected)
	{
		m_selected = selected;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::string MenuItemWithAction::GetAction()
	{
		return m_action;
	}
	bool MenuItemWithAction::GetSelected()
	{
		return m_selected;
	}
	//MenuItemToOpenSubmenu
	//-----------------------------------------------------------------------------------------------------------
	MenuItemToOpenSubmenu::MenuItemToOpenSubmenu(std::string text, std::string action) : MenuItemWithAction(text, action) {}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItemToOpenSubmenu::ActionHandle()
	{
		UI::GetInstance().CreateMenu(m_action);
	}
	//MenuItemToSetState
	//-----------------------------------------------------------------------------------------------------------
	MenuItemToSetState::MenuItemToSetState(std::string text, std::string mainAction, std::string secondAction, std::string attribute)
		: MenuItemWithAction(text, mainAction)
	{

		m_atribute = attribute;
		if (attribute != "")
		{
			if (attribute == " |_|")
			{
				m_isHaveCheckBox = true;
			}
			if (attribute == " =A ")
			{
				m_isOpenSubMenu = true;
			}
			m_secondAction = secondAction;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItemToSetState::ActionHandle()
	{
		std::vector<std::shared_ptr<Menu>> menu = UI::GetInstance().GetMenu();

		if (m_SETTINGS_MAP.count(m_action) > 0)
		{
			//set settings
			Game::GetInstance().SetGameSettings(m_SETTINGS_MAP.find(m_action)->second);
		}
		else if (m_DIFFICULTY_MAP.count(m_action) > 0)
		{
			//set diff
			Game::GetInstance().SetGameDifficulty(m_DIFFICULTY_MAP.find(m_action)->second);
		}
		else if (m_STATE_ACTIONS_MAP.count(m_action) > 0)
		{
			if (m_isOpenSubMenu)
			{
				UI::GetInstance().CreateMenu(m_secondAction);
			}
			else
			{
				switch (m_STATE_ACTIONS_MAP.find(m_action)->second)
				{
				case State::ResumeGame:
				{
					Game::GetInstance().RemoveGameState();
					break;
				}
				case State::ExitGame:
				{
					Game::GetInstance().ExitFromGame();
					break;
				}
				case State::GameInProgress:
				{
					Game::GetInstance().AddGameState(m_STATE_ACTIONS_MAP.find(m_action)->second, Game::GetInstance().GetCurrentGameState()->GetScore());
					break;
				}
				case State::GameOver:
				{
					Game::GetInstance().AddGameState(m_STATE_ACTIONS_MAP.find(m_action)->second, Game::GetInstance().GetCurrentGameState()->GetScore());
					break;
				}
				case State::Main:
				{
					UI::GetInstance().RemoveMenu();
					Game::GetInstance().ResetGame();

					break;
				}
				}
			}
		}
		else
		{
			std::cerr << "Action not found" << std::endl;
		}

		if (m_atribute == " ><|" || m_atribute == " <>|")
		{
			menu[menu.size() - 1]->RemoveMenuStackItem();
		}
	}
	void MenuItemToSetState::Visualize(sf::RenderWindow& window, Vector2D tabIndex)
	{
		MenuItemWithAction::Visualize(window, tabIndex);

		if (m_isHaveCheckBox)
		{
			UI::GetInstance().InitShape(m_checkBoxFrame, m_CHECKBOX_SIZE, m_CHECKBOX_SIZE, 1.1f, sf::Color::White, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * tabIndex.x, m_MENU_CHECKBOX_INDETATION.y + 50.0f * tabIndex.y);
			window.draw(m_checkBoxFrame);
			if (m_SETTINGS_MAP.count(m_action) > 0)
			{
				if (!(static_cast<uint32_t>(m_SETTINGS_MAP.find(m_action)->second) & Game::GetInstance().GetSetings()))
				{
					UI::GetInstance().InitShape(m_checkBox, m_CHECKBOX_SIZE, m_CHECKBOX_SIZE, 1.0f, sf::Color::Green, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * tabIndex.x, m_MENU_CHECKBOX_INDETATION.y + 50.0f * tabIndex.y);
					window.draw(m_checkBox);
				}
			}
			else if (m_DIFFICULTY_MAP.count(m_action) > 0)
			{
				if (m_DIFFICULTY_MAP.find(m_action)->second == Game::GetInstance().GetDifficultyLevel())
				{
					UI::GetInstance().InitShape(m_checkBox, m_CHECKBOX_SIZE, m_CHECKBOX_SIZE, 1.0f, sf::Color::Green, m_MENU_CHECKBOX_INDETATION.x - MENU_INDENTATION_OFFSET * tabIndex.x, m_MENU_CHECKBOX_INDETATION.y + 50.0f * tabIndex.y);
					window.draw(m_checkBox);
				}
			}
		}
	}
	//Menu
	//-----------------------------------------------------------------------------------------------------------
	Menu::Menu(const std::string& section, const INIReader& reader, int index)
	{
		AddMenuItems(section, reader);
		m_index = index;
	}
	//-----------------------------------------------------------------------------------------------------------
	Menu::~Menu()
	{
		while (!m_menuStack.empty())
		{
			m_menuStack.pop();
		}
	}
	void Menu::DrawMenu(sf::RenderWindow& window, Vector2D tabMenu)
	{
		float i = 0.0f;
		for (auto mapItem : m_menuStack.top())
		{
			mapItem.second->Visualize(window, { tabMenu.y + m_index, tabMenu.x + i });
			i = i + 1.0f;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::AddMenuItems(const std::string& section, const INIReader& reader)
	{
		m_menuStack.push(LoadMenuItems(section, reader));
	}
	//-----------------------------------------------------------------------------------------------------------
	std::map<int, std::shared_ptr<MenuItemWithAction>> Menu::LoadMenuItems(const std::string& section, const INIReader& reader)
	{
		std::vector<std::shared_ptr<Menu>> menuList;
		const size_t simbol_size = 4;
		std::map<int, std::shared_ptr<MenuItemWithAction>> menuItems;
		size_t mainActionPos = 0;
		size_t secondActionPos = 0;
		int id = 0;
		std::string value = "";
		std::string mainAction = "";
		std::string secondAction = "";
		std::string attribute = "";
		std::map<int, MenuItem*> menu;
		std::map<std::string, std::string> sectionData;

		auto proccess_actions = [&]()
			{
				mainAction = value.substr(mainActionPos + simbol_size, secondActionPos - mainActionPos - simbol_size);
				attribute = value.substr(secondActionPos, simbol_size);
				secondAction = value.substr(secondActionPos + simbol_size);
				value = value.substr(0, mainActionPos);
			};

		for (std::string key : reader.Keys(section))
		{
			sectionData[key] = reader.Get(section, key, "error");
		}
		for (const auto& pair : sectionData)
		{
			try
			{
				id = std::stoi(pair.first);
				value = pair.second;
				if (id == 0)
				{
					//Create menu title
					mainAction = "";
					menuItems[id] = std::make_shared <MenuItemWithAction>(value, mainAction);
				}
				else
				{
					//Create simple item
					mainActionPos = value.find(" -> ");
					if (mainActionPos != std::string::npos)
					{
						mainAction = value.substr(mainActionPos + simbol_size);
						value = value.substr(0, mainActionPos);
						menuItems[id] = std::make_shared <MenuItemWithAction>(value, mainAction);
					}

					//Create submenu action item
					mainActionPos = value.find(" =| ");
					if (mainActionPos != std::string::npos)
					{
						mainAction = value.substr(mainActionPos + simbol_size);
						value = value.substr(0, mainActionPos);
						menuItems[id] = std::make_shared<MenuItemToOpenSubmenu>(value, mainAction);
					}

					//Create state chenge item
					mainActionPos = value.find(" => ");
					if (mainActionPos != std::string::npos)
					{
						//whith checkBox
						secondActionPos = value.find(" |_|");
						if (secondActionPos != std::string::npos)
						{
							proccess_actions();
						}
						//open accept menu
						secondActionPos = value.find(" =A ");
						if (secondActionPos != std::string::npos)
						{
							proccess_actions();
						}
						//close item
						secondActionPos = value.find(" ><|");
						if (secondActionPos != std::string::npos)
						{
							proccess_actions();
						}
						menuItems[id] = std::make_shared <MenuItemToSetState>(value, mainAction, secondAction, attribute);
					}

					//close item and make action
					mainActionPos = value.find(" <>|");
					if (mainActionPos != std::string::npos)
					{
						attribute = value.substr(mainActionPos, simbol_size);
						value = value.substr(0, mainActionPos);
						menuList = UI::GetInstance().GetMenu();
						menuItems[id] = std::make_shared <MenuItemToSetState>(value, menuList[menuList.size() - 1]->GetStack().top().find(menuList[menuList.size() - 1]->GetMenuPointer())->second->GetAction(), "", attribute);
					}

					//close action
					mainActionPos = value.find(" ><|");
					if (mainActionPos != std::string::npos)
					{
						mainAction = value.substr(mainActionPos);
						value = value.substr(0, mainActionPos);
						menuItems[id] = std::make_shared <MenuItemWithAction>(value, mainAction);
					}
				}
			}
			catch (const std::invalid_argument&) {}
			//Set selected item
			if (id == m_menuPointer)
			{
				menuItems.find(id)->second->SetSelection(true);
			}
		}
		return menuItems;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::SwitchMenuItem(bool down)
	{
		int i = 0;
		std::map<int, std::shared_ptr<MenuItemWithAction>> currentItems = m_menuStack.top();

		if (down == true && m_menuPointer < currentItems.size() - 1)
		{
			m_menuPointer++;
		}
		else if (down == false && m_menuPointer > 1)
		{
			m_menuPointer--;
		}
		for (auto item : currentItems)
		{
			if (m_menuPointer == i)
			{
				item.second->SetSelection(true);
			}
			else
			{
				item.second->SetSelection(false);
			}
			i++;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	std::stack<std::map<int, std::shared_ptr<MenuItemWithAction>>> Menu::GetStack()
	{
		return m_menuStack;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::AddMenuStackItem(std::map<int, std::shared_ptr<MenuItemWithAction>> menuItem)
	{
		m_menuStack.push(menuItem);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::RemoveMenuStackItem()
	{
		int i = 0;
		//remove items
		m_menuStack.pop();
		//remove menu
		if (m_menuStack.empty())
		{
			UI::GetInstance().RemoveMenu(*this);
		}
		else
		{
			for (auto item : m_menuStack.top())
			{
				if (item.second->GetSelected())
				{
					m_menuPointer = i;
					continue;
				}
				i++;
			}
		}
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
	std::map<int, std::shared_ptr<MenuItemWithAction>> Menu::GetMenuActiveItemList()
	{
		return m_menuStack.top();
	}
	//RecordsMenu
	//-----------------------------------------------------------------------------------------------------------
	void RecordsMenu::DrawMenu(sf::RenderWindow& window, Vector2D tabMenu)
	{
		std::vector<std::shared_ptr<RecordItem>> score = Game::GetInstance().ReadRecordsList();

		float i = 0.0f;
		bool isScoreDrawed = false;

		for (const auto mapItem : m_menuStack.top())
		{
			//Draw title and active items
			mapItem.second->Visualize(window, { tabMenu.y + m_index, tabMenu.x + i });
			i = i + 1.0f;
			if (!isScoreDrawed)
			{
				isScoreDrawed = true;

				//Draw records items
				for (const auto scoreItem : score)
				{
					scoreItem->Visualize(window, { tabMenu.y + m_index, tabMenu.x + i });
					i = i + 1.0f;
				}
			}
		}
	}
}