#pragma once
#include <SFML/Graphics.hpp>
#include "GridManager.h"
#include "Config.h"
#include "Math.h"

namespace Arkanoid
{
	class Ball
	{
	public:

		Ball();
		~Ball() {}

		void UpdateBall(float deltaTime);
		void SetPosition(Position2D position);
		Position2D GetPosition();
		void SetVectorSpeed(Position2D vectorSpeed);
		void DrawBall(sf::RenderWindow& window);

	private:

		void UpdataBallDirection();
		void UpdateBallPosition(float deltaTime);

		Position2D m_position;
		float m_scalarSpeed = 0.0f;
		Position2D m_ballVectorSpeed = { 100.0f, 100.0f };
		float m_currentAngle = 0.0f;

		sf::Texture m_ballTexture;
		sf::Sprite m_sprite;
	};
}