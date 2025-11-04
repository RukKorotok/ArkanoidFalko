#pragma once
#include <SFML/Graphics.hpp>
#include "GridManager.h"
#include "Config.h"
#include "Math.h"

//For next rev
namespace Arkanoid
{
	class Game;

	enum BlockDamage
	{
		Clear = 0,
		Crack,
		Broken
	};

	class Block
	{
	public:

		Block(IndexEvents type, int index);

		bool HitBlock(Game& game, int appleIndex);
		void Draw(sf::RenderWindow& window);
	private:

		sf::Texture m_frameClearTexture;
		sf::Texture m_frameCrackTexture;
		sf::Texture m_frameBrokenTexture;
		sf::RectangleShape m_blockShape;
		int m_index = 0;
		sf::Sprite m_frameSprite;
		sf::Color m_color = sf::Color::White;
		IndexEvents m_type = IndexEvents::Normal;
		int m_health = 1;
	};
}
