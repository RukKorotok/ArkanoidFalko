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

		static Ball& GetInstance()
		{
			static Ball ball;
			return ball;
		}

		void UpdateBall(float deltaTime);
		void DrawBall(sf::RenderWindow& window);

	private:

		Ball();
		~Ball() {}

		Ball(const Ball&) = delete;
		Ball& operator = (Ball const&) = delete;

		void CheckBallCollitions();
		void UpdataBallDirection();
		void UpdateBallPosition(float deltaTime);

		Position2D m_ballPosition;
		float m_scalarSpeed = 0.0f;
		Position2D m_ballVectorSpeed = { 100.0f, 100.0f };
		float m_currentAngle = 0.0f;

		sf::Texture m_ballTexture;
		sf::Sprite m_sprite;
	};
}