#include "Rock.h"
#include "Game.h"

namespace Arkanoid
{
	//For next rev
	//-----------------------------------------------------------------------------------------------------------
	Rock::Rock(Rock& rock, Game& game)
	{
		assert(m_texture.loadFromFile(RESOURCES_PATH + "PlatformMiddle.png"));

		// get index
		int currentIndex = 0;
		currentIndex = GridManager::GetInstance().GetRandomIndex();
		while (!GridManager::GetInstance().CheckIndexForFree(game, currentIndex))
		{
			currentIndex = GridManager::GetInstance().GetRandomIndex();
		}

		//set to map
		GridManager::GetInstance().AddIndexToEventMap(game, currentIndex, IndexEvents::Wall);
		m_index = currentIndex;

		m_sprite.setTexture(m_texture);
		Math::GetInstance().SetSpriteSize(m_sprite, CELL_SIZE, CELL_SIZE);
		Math::GetInstance().SetSpriteRelativeOrigin(m_sprite, 0.5f, 0.5f);
		m_sprite.setPosition(GridManager::GetInstance().GetPositionFromIndex(game, currentIndex).x, GridManager::GetInstance().GetPositionFromIndex(game, currentIndex).y);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Rock::DrawRock(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}
}
