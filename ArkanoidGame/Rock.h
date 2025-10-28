#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Math.h"

namespace Arkanoid
{
	class Game;

	class Rock
	{
	public:
		Rock(Rock& rock, Game& game);
		void DrawRock(sf::RenderWindow& window);
	private:

		int m_index = 0;
		sf::Texture m_texture;
		sf::Sprite m_sprite;
	};
}

