#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Math.h"
#include <stack>
#include "INIreader.h"

namespace Arkanoid
{

	enum SettingsMode
	{
		SoundOn = 1 << 0,
		MusicOn = 1 << 1,
	};

	enum DifficultyLevel
	{
		Easy = 0,
		Medium,
		Hard
	};

	enum State
	{
		Undefined = 0,
		GameInProgress,
		ResumeGame,
		Main,
		GameOver,
		Win,
		ExitGame
	};

	class Game;

	class RecordItem
	{
	public:
		RecordItem(std::string name, int score);
		void Draw(sf::RenderWindow& window, Position2D tabIndex);
		std::string GetName();
		int GetScore();

	private:
		std::string m_name = "";
		int m_score = 0;
		sf::Text m_text;
		const Position2D m_MENU_ITEMS_INDETATION = { SCREEN_WIDTH * 0.7f, 100.0f };
		const Position2D m_MENU_CHECKBOX_INDETATION = { SCREEN_WIDTH - 50.0f, 110.0f };
		const float MENU_INDENTATION_OFFSET = 120.0f;
	};

	class MenuItem
	{
	public:

		MenuItem(std::string text, std::string action);

		virtual void Draw(sf::RenderWindow& window, Position2D tabIndex);
		virtual void ActionHandle();
		void SetSelection(bool selected);
		std::string GetAction();
		bool GetSelected();

	protected:

		const Position2D m_MENU_ITEMS_INDETATION = { SCREEN_WIDTH * 0.7f, 100.0f };
		const Position2D m_MENU_CHECKBOX_INDETATION = { SCREEN_WIDTH - 50.0f, 110.0f };
		const float MENU_INDENTATION_OFFSET = 120.0f;
		std::string m_string = "";
		std::string m_action = "";
		std::string m_atribute = "";
		sf::Text m_text;
		bool m_selected = false;
	};

	class MenuItemToOpenSubmenu final : public MenuItem
	{
	public:

		MenuItemToOpenSubmenu(std::string text, std::string action);

		void ActionHandle() override;
	};

	class MenuItemToSetState final : public MenuItem
	{
	public:

		MenuItemToSetState(std::string text, std::string mainAction, std::string secondAction, std::string attribute);

		void ActionHandle() override;
		void Draw(sf::RenderWindow& window, Position2D tabIndex) override;

	private:
		const std::map<std::string, SettingsMode> m_SETTINGS_MAP = {
			{"SetSound", SoundOn},
			{"SetMusic", MusicOn}
		};
		const std::map<std::string, DifficultyLevel> m_DIFFICULTY_MAP = {
			{"SetEasy", Easy},
			{"SetMedium", Medium},
			{"SetHard", Hard}
		};
		const std::map<std::string, State> m_STATE_ACTIONS_MAP = {
			{"StartGame", State::GameInProgress},
			{"Exit", State::ExitGame},
			{"Resume", State::ResumeGame},
			{"Main", State::Main},
		};

		const float m_CHECKBOX_SIZE = 30.0f;
		bool m_isHaveCheckBox = false;
		bool m_isOpenSubMenu = false;
		sf::RectangleShape m_checkBox;
		sf::RectangleShape m_checkBoxFrame;
		std::string m_secondAction = "";
	};

	class Menu
	{
	public:
		Menu(const std::string& section, const INIReader& reader, int index);
		~Menu();

		virtual void DrawMenu(sf::RenderWindow& window, Position2D tabMenu);
		void AddMenuItems(const std::string& section, const INIReader& reader);
		void SwitchMenuItem(bool down);
		std::stack<std::map<int, std::shared_ptr<MenuItem>>> GetStack();
		void AddMenuStackItem(std::map<int, std::shared_ptr<MenuItem>> menuItem);
		void RemoveMenuStackItem();
		int GetMenuPointer();
		void SetMenuPointer(int pointer);
		std::map<int, std::shared_ptr<MenuItem>> GetMenuActiveItemList();
	protected:

		virtual std::map<int, std::shared_ptr<MenuItem>> LoadMenuItems(const std::string& section, const INIReader& reader);

		std::stack<std::map<int, std::shared_ptr<MenuItem>>> m_menuStack;
		sf::RectangleShape m_checkboxFraim;
		sf::RectangleShape m_checkbox;
		int m_menuPointer = 1;
		sf::Sprite m_mainMenuSprite;
		int m_index =0;
	};

	class RecordsMenu : public Menu
	{
	public:

		RecordsMenu(const std::string& section, const INIReader& reader, int index) :
			Menu(section, reader, index) {}

		void DrawMenu(sf::RenderWindow& window, Position2D tabMenu) override;
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
		void InitText(sf::Text& text, const std::string title, const sf::Color& color, const float height, const float width);
		void DrawGameUI(Game& game, sf::RenderWindow& window);
		void SetStringInText(sf::Text& text, std::string string);
		void SetShapeColor(sf::RectangleShape& shape, const sf::Color& color);
		void SetTextColor(sf::Text& text, const sf::Color& color);
		std::shared_ptr<Menu> CreateMenu(std::string section);
		void RemoveMenu(Menu& menu);
		std::vector<std::shared_ptr<Menu>> GetMenu();
		INIReader GetReader();
		void RemoveMenu();

	private:

		UI();
		~UI();

		UI(const UI&) = delete;
		UI& operator = (UI const&) = delete;

		std::vector<std::shared_ptr<Menu>> m_currentMenuList;
		sf::Font m_font;
		sf::Texture m_mainMenueTexture;
		sf::Sprite m_mainMenuSprite;
		sf::Text m_gameText;
		sf::Text m_nameInputText;
		bool m_drawPopUpMenu = false;
		std::string m_inputString = "";
		INIReader* m_reader;
	};
}
