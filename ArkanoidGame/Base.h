#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Math.h"

namespace Arkanoid
{
	class BaseSegment
	{
	public:

		BaseSegment(int index, bool edge);
		int GetIndex();
		void SetPosition(float xPos);
		Position2D GetPosition();
		sf::Sprite GetSprite();
		void SetColor(sf::Color color);
		void DrawSegment(sf::RenderWindow& window);

	private:

		const float m_Y_POSITION_OFFSET = 50.0f;

		int m_index = 0;
		Position2D m_position;
		sf::Sprite m_sprite;
		sf::Texture m_segmentTexture;
	};

	class Base
	{
	public:

		Base();
		~Base();

		void UpdateBase(float xPosition);
		void ChangeBaseColor(sf::Color color);
		Position2D GetPosition();
		Position2D GetBaseSize();
		void DrawBase(sf::RenderWindow& window);

	private:

		const int m_START_SIZE = 3;

		std::vector<BaseSegment*> m_segments;
		bool m_isNormalOrientation = true;
		int m_toEndDobleSpeedSteps = 0;
		int m_toEndDisorientationSteps = 0;
		float m_xPosition = 0;
	};
}
