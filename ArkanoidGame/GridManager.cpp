#include "Game.h"
#include "GridManager.h"

namespace Arkanoid
{
	//-----------------------------------------------------------------------------------------------------------
	GridManager::GridManager()
	{
		m_maxIndex = int(SCREEN_WIDTH / CELL_SIZE) + int(SCREEN_HEIGHT / CELL_SIZE) * 1000;
		int xCount = GetXCoordFromIndex(m_maxIndex);
		int yCount = GetYCoordFromIndex(m_maxIndex);

		for (int x = 0; x < xCount; x++)
		{
			for (int y = 0; y < yCount; y++)
			{
				m_indexLocationMap[y * m_Y_MULTY + x] = { float(x) * CELL_SIZE + CELL_SIZE * 0.5f, float(y) * CELL_SIZE + CELL_SIZE * 0.5f };
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	GridManager::~GridManager()
	{
		m_indexLocationMap.clear();
		m_indexEventMap.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	Position2D GridManager::GetPositionFromIndex(Game& game, int index)
	{
		return m_indexLocationMap.find(index)->second;
	}
	//-----------------------------------------------------------------------------------------------------------
	bool GridManager::AddIndexToEventMap(Game& game, int index, IndexEvents event)
	{
		if (!m_indexEventMap.count(index) )
		{
			m_indexEventMap[index] = event;
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------------------------------------
	bool GridManager::RemoveIndexToEventMap(Game& game, int index)
	{
		if (m_indexEventMap.count(index) )
		{
			m_indexEventMap.erase(index);
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GridManager::CleareEventMap()
	{
		m_indexEventMap.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	IndexEvents GridManager::GetEventMapValue(Game& game, int index)
	{
		return m_indexEventMap.find(index)->second;
	}
	//-----------------------------------------------------------------------------------------------------------
	bool GridManager::CheckIndexForValid(Game& game, int index)
	{
		return m_indexLocationMap.count(index);
	}
	//-----------------------------------------------------------------------------------------------------------
	bool GridManager::CheckIndexForFree(Game& game, int index)
	{
		return !m_indexEventMap.count(index);
	}
	//-----------------------------------------------------------------------------------------------------------
	int GridManager::GetXCoordFromIndex(int index)
	{
		return index - index / m_Y_MULTY * m_Y_MULTY;
	}
	//-----------------------------------------------------------------------------------------------------------
	int GridManager::GetYCoordFromIndex(int index)
	{
		return index / m_Y_MULTY;
	}
	//-----------------------------------------------------------------------------------------------------------
	int GridManager::GetNewIndexForDirection(Direction direction, int index, bool forwardMove)
	{
		switch (direction)
		{
		case Direction::Right:
		{
			if (forwardMove)
			{
				return index + 1;
			}
			else
			{
				return index - 2;
			}
		}
		case Direction::Up:
		{
			if (forwardMove)
			{
				return index - 1000;
			}
			else
			{
				return index + 2000;
			}
		}
		case Direction::Left:
		{
			if (forwardMove)
			{
				return index - 1;
			}
			else
			{
				return index + 2;
			}
		}
		case Direction::Down:
		{
			if (forwardMove)
			{
				return index + 1000;
			}
			else
			{
				return index - 2000;
			}
		}
		default:
		{
			return -1;
		}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	int GridManager::GetRandomIndex()
	{
		int indexX;
		int indexY;
		indexX = rand() % GetXCoordFromIndex(m_maxIndex);
		indexY = rand() % GetYCoordFromIndex(m_maxIndex);
		
		return indexY * m_Y_MULTY + indexX;

	}

}
