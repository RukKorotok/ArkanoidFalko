#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Math.h"
#include <stack>

namespace Arkanoid
{

	class Game;

	struct ParticipantString
	{
		std::string participant = "player";
		int score = 0;
	};

	struct MenuStateInfo
	{
		MenuStates state;
		MenuItemsList menuActiveItems;
		std::vector<std::string> menuPassiveItems;

		int pointsCount = 0;
		int previewPointer = 0;
	};

	enum MenuType
	{
		StartMenu = 0,
		PauseMenu,
		AfterDeathMenu,
		RecordListMenu,
		AcceptMenu,
		EnterNammeMenu
	};

	class Menu
	{
	public:
		Menu(MenuType type);
		~Menu();

		void SwitchMenuItem(bool down);
		void GoBack();
		std::stack<MenuStateInfo> GetStack();
		void AddMenuStackItem(MenuStateInfo menuItem);
		void RemoveMenuStackItem();
		int GetMenuPointer();
		void SetMenuPointer(int pointer);
		std::map<MenuStates, MenuItemsList> GetMenuActiveItemList();
		std::map<MenuStates, std::vector<std::string>> GetMenuPassiveItemList();
	private:
		sf::Text m_menuTitleText;
		sf::Font m_font;
		std::vector<sf::Text> m_menuItemsText;
		std::vector<MenuItem> m_menuItems;
		std::stack<MenuStateInfo> m_menuStack;
		sf::RectangleShape m_checkboxFraim;
		sf::RectangleShape m_checkbox;
		int m_menuPointer = 0;
		std::map<MenuStates, MenuItemsList> m_menuActiveItemsList;
		std::map<MenuStates, std::vector<std::string>> m_menuPassiveItemsList;

		sf::Sprite m_mainMenuSprite;

	};

	class UI
	{
	public:

		static UI& GetInstance()
		{
			static UI ui;
			return ui;
		}

		void DrawMenu(sf::RenderWindow& window);
		void InitSprite(sf::Sprite& sprite, const sf::Texture& texture, float heightMulty, float widthMulty);
		void InitShape(sf::RectangleShape& shape, float shapeSizeX, float shapeSizeY, float sizeMulty, const sf::Color& color, float height, float width);
		void InitText(sf::Text& text, const sf::Font& font, const std::string title, const sf::Color& color, const float height, const float width);
		void InitRecordsList();
		void RefreshRecordList(Game& game);
		std::map<int, ParticipantString> GetRecordList();
		bool IsSetNewRecord(Game& game);

		void DrawGameUI(Game& game, sf::RenderWindow& window);
		sf::Font GetFont();
		void SetStringInText(sf::Text& text, std::string string);
		void SetShapeColor(sf::RectangleShape& shape, const sf::Color& color);
		void SetTextColor(sf::Text& text, const sf::Color& color);
		void CreateMenu(MenuType menuType);
		void RemoveMenu(Menu* menu);
		std::vector<Menu*> GetMenu();

	private:

		UI();
		~UI();

		UI(const UI&) = delete;
		UI& operator = (UI const&) = delete;

		const float CHECKBOX_SIZE = 25.0f;
		const float SUB_MENU_INDENTATION = SCREEN_WIDTH * 0.6f;
		const Position2D m_MENU_ITEMS_INDETATION = { SCREEN_WIDTH * 0.7f, 100.0f };
		const Position2D m_MENU_CHECKBOX_INDETATION = { SCREEN_WIDTH - 50.0f, 110.0f };
		const float MENU_INDENTATION_OFFSET = 120.0f;

		std::vector<Menu*> m_currentMenuList;

		sf::Font m_font;
		sf::Texture m_mainMenueTexture;
		sf::Sprite m_mainMenuSprite;

		sf::Text m_gameText;
		sf::Text m_nameInputText;
		std::map<int, ParticipantString> m_recordListText;

		bool m_drawPopUpMenu = false;
		std::string m_inputString = "";
	};
}
