#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Math.h"

namespace Arkanoid
{
	enum IndexEvents
	{
		Normal = 0,
		ScoreUp,
		Poison,
		Disorient,
		Armored,
		HeavilyArmored,
		Wall
	};
	class Game;

	class GridManager
	{
	public:
		static GridManager& GetInstance()
		{
			static GridManager gridManager;
			return gridManager;
		}

		Position2D GetPositionFromIndex(Game& game, int index);
		bool AddIndexToEventMap(Game& game, int index, IndexEvents event);
		bool RemoveIndexToEventMap(Game& game, int index);
		void CleareEventMap();
		IndexEvents GetEventMapValue(Game& game, int index);
		bool CheckIndexForValid(Game& game, int index);
		bool CheckIndexForFree(Game& game, int index);
		int GetXCoordFromIndex(int index);
		int GetYCoordFromIndex(int index);
		int GetNewIndexForDirection(Direction direction, int index, bool forwardMove);
		int GetRandomIndex();

	private:

		GridManager();
		~GridManager();

		GridManager(const GridManager&) = delete;
		GridManager& operator = (GridManager const&) = delete;

		std::map<int, Position2D> m_indexLocationMap;
		std::map<int, IndexEvents> m_indexEventMap;
		int m_maxIndex = 0;
		const int m_Y_MULTY = 1000;
	};
}
