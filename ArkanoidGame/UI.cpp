#include "UI.h"
#include "Game.h"

namespace Arkanoid
{	
	//RecordItem
	//-----------------------------------------------------------------------------------------------------------
	RecordItem::RecordItem(std::string name, int score)
	{
		m_name = name;
		m_score = score;
	}
	//-----------------------------------------------------------------------------------------------------------
	void RecordItem::Draw(sf::RenderWindow& window, Position2D tabIndex)
	{
		UI::GetInstance().InitText(m_text, m_name + " " + std::to_string(m_score), sf::Color::White, m_MENU_ITEMS_INDETATION.x - MENU_INDENTATION_OFFSET * tabIndex.x, m_MENU_ITEMS_INDETATION.y + 50.0f * tabIndex.y);
		window.draw(m_text);
	}
	//-----------------------------------------------------------------------------------------------------------
	std::string RecordItem::GetName()
	{
		return m_name;
	}
	//-----------------------------------------------------------------------------------------------------------
	int RecordItem::GetScore()
	{
		return m_score;
	}
	//MenuItem
	//-----------------------------------------------------------------------------------------------------------
	MenuItem::MenuItem(std::string text, std::string action)
	{
		m_string = text;
		m_action = action;
	}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItem::Draw(sf::RenderWindow& window, Position2D tabIndex)
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

		UI::GetInstance().InitText(m_text, m_string, color, m_MENU_ITEMS_INDETATION.x - MENU_INDENTATION_OFFSET * tabIndex.x, m_MENU_ITEMS_INDETATION.y + 50.0f * tabIndex.y);
		window.draw(m_text);
	}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItem::ActionHandle()
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
	void MenuItem::SetSelection(bool selected)
	{
		m_selected = selected;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::string MenuItem::GetAction()
	{
		return m_action;
	}
	bool MenuItem::GetSelected()
	{
		return m_selected;
	}
	//MenuItemToOpenSubmenu
	//-----------------------------------------------------------------------------------------------------------
	MenuItemToOpenSubmenu::MenuItemToOpenSubmenu(std::string text, std::string action)
		: MenuItem(text, action){}
	//-----------------------------------------------------------------------------------------------------------
	void MenuItemToOpenSubmenu::ActionHandle()
	{
		UI::GetInstance().CreateMenu(m_action);
	}
	//MenuItemToSetState
	//-----------------------------------------------------------------------------------------------------------
	MenuItemToSetState::MenuItemToSetState(std::string text, std::string mainAction, std::string secondAction, std::string attribute)
		: MenuItem(text, mainAction)
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

		if (m_SETTINGS_MAP.count(m_action) > 0 )
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
	void MenuItemToSetState::Draw(sf::RenderWindow& window, Position2D tabIndex)
	{
		MenuItem::Draw(window, tabIndex);

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
	void Menu::DrawMenu(sf::RenderWindow& window, Position2D tabMenu)
	{
		float i = 0.0f;
		for (auto mapItem : m_menuStack.top())
		{
			mapItem.second->Draw(window, { tabMenu.y + m_index, tabMenu.x + i } );
			i = i + 1.0f;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::AddMenuItems(const std::string& section, const INIReader& reader)
	{
		m_menuStack.push(LoadMenuItems(section, reader));
	}
	//-----------------------------------------------------------------------------------------------------------
	std::map<int, std::shared_ptr<MenuItem>> Menu::LoadMenuItems(const std::string& section, const INIReader& reader)
	{
		std::vector<std::shared_ptr<Menu>> menuList;
		const size_t simbol_size = 4;
		std::map<int, std::shared_ptr<MenuItem>> menuItems;
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
					menuItems[id] = std::make_shared <MenuItem>(value, mainAction);
				}
				else
				{
					//Create simple item
					mainActionPos = value.find(" -> ");
					if (mainActionPos != std::string::npos)
					{
						mainAction = value.substr(mainActionPos + simbol_size);
						value = value.substr(0, mainActionPos);
						menuItems[id] = std::make_shared <MenuItem>(value, mainAction);
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
						menuItems[id] = std::make_shared <MenuItem>(value, mainAction);
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
		std::map<int, std::shared_ptr<MenuItem>> currentItems = m_menuStack.top();

		if (down == true && m_menuPointer < currentItems.size() - 1 )
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
	std::stack<std::map<int, std::shared_ptr<MenuItem>>> Menu::GetStack()
	{
		return m_menuStack;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Menu::AddMenuStackItem(std::map<int, std::shared_ptr<MenuItem>> menuItem)
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
	std::map<int, std::shared_ptr<MenuItem>> Menu::GetMenuActiveItemList()
	{
		return m_menuStack.top();
	}
	//RecordsMenu
	//-----------------------------------------------------------------------------------------------------------
	void RecordsMenu::DrawMenu(sf::RenderWindow& window, Position2D tabMenu)
	{
		std::vector<std::shared_ptr<RecordItem>> score = Game::GetInstance().ReadRecordsList();

		float i = 0.0f;
		bool isScoreDrawed = false;

		for (const auto mapItem : m_menuStack.top())
		{
			//Draw title and active items
			mapItem.second->Draw(window, { tabMenu.y + m_index, tabMenu.x + i });
			i = i + 1.0f;
			if (!isScoreDrawed)
			{
				isScoreDrawed = true;

				//Draw records items
				for (const auto scoreItem : score)
				{
					scoreItem->Draw(window, { tabMenu.y + m_index, tabMenu.x + i });
					i = i + 1.0f;
				}
			}
		}
	}
	//UI
	//-----------------------------------------------------------------------------------------------------------
	void UI::DrawMenu(sf::RenderWindow& window)
	{
		for(int i = 0; i < m_currentMenuList.size(); i++)
		{
			m_currentMenuList[i]->DrawMenu(window, { float(i), 0.0f });
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
	void UI::InitText(sf::Text& text, const std::string title, const sf::Color& color, const float heigh, const float width)
	{
		text.setFont(m_font);
		text.setCharacterSize(TEXT_SIZE - 5);
		text.setString(title);
		text.setFillColor(color);
		text.setPosition(heigh, width);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::DrawGameUI(Game& game, sf::RenderWindow& window)
	{
		m_gameText.setString(SCORE_TITLE + std::to_string(game.GetCurrentGameState()->GetScore()));
		window.draw(m_gameText);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::SetStringInText(sf::Text& text, std::string string)
	{
		text.setString(string);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::SetShapeColor(sf::RectangleShape& button, const sf::Color& color)
	{
		button.setFillColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::SetTextColor(sf::Text& text, const sf::Color& color)
	{
		text.setFillColor(color);
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Menu> UI::CreateMenu(std::string section)
	{
		if (section == "RecordList")
		{
			std::shared_ptr<Menu> menu = std::make_shared<RecordsMenu>(section, *m_reader, m_currentMenuList.size());
			m_currentMenuList.push_back(menu);
			return menu;
		}
		else
		{
			std::shared_ptr<Menu> menu = std::make_shared<Menu>(section, *m_reader, m_currentMenuList.size());
			m_currentMenuList.push_back(menu);
			return menu;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::RemoveMenu(Menu& menu)
	{
		int i = 0;
		Menu* menuAdress = &menu;
		for (i = 0; i < m_currentMenuList.size(); i++)
		{		
			if (m_currentMenuList[i].get() == menuAdress)
			{
				m_currentMenuList.erase(m_currentMenuList.begin() + i);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<Menu>> UI::GetMenu()
	{
		return m_currentMenuList;
	}
	//-----------------------------------------------------------------------------------------------------------
	INIReader UI::GetReader()
	{
		return *m_reader;
	}
	//-----------------------------------------------------------------------------------------------------------
	void UI::RemoveMenu()
	{
		m_currentMenuList.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	UI::UI()
	{
		m_reader = new INIReader("../Config/Menu.ini");

		if (m_reader->ParseError() < 0)
		{
			std::cerr << "Menu.ini not found" << std::endl;
		}

		assert(m_mainMenueTexture.loadFromFile(RESOURCES_PATH + "\\GameStart.png"));
		assert(m_font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		int i = 0;
		float mainMenueSpriteSize = SCREEN_HEIGHT * 0.9f;
		float menuIndentation = SCREEN_WIDTH - 250.0f;

		// Init UI sprites
		InitSprite(m_mainMenuSprite, m_mainMenueTexture, 0.38f, 0.35f);

		// Init UI text
		InitText(m_gameText, SCORE_TITLE, sf::Color::Yellow, SCREEN_WIDTH - 100.0f, 5.0f);
		InitText(m_nameInputText, INPUT_TITLE, sf::Color::Yellow, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f);
	}
	//-----------------------------------------------------------------------------------------------------------
	UI::~UI()
	{
		m_currentMenuList.clear();
	}
}
