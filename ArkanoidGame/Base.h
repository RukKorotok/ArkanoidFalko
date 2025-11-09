#pragma once
#include "Objects.h"
#include "Interfaces.h"

namespace Arkanoid
{
	class BaseSegment : public GameObject, public Collidable
	{

	public:

		BaseSegment(int index, bool edge);
		int GetIndex();
		void SetPosition(float xPos);
		void SetColor(sf::Color color);
		void OnHit() override {}

	private:
		const float m_Y_POSITION_OFFSET = 50.0f;

		int m_index = 0;
	};

	class Base
	{
	public:

		Base();
		~Base();

		void UpdateBase(float xPosition);
		void ChangeBaseColor(sf::Color color);
		Vector2D GetPosition();
		Vector2D GetSize();
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
