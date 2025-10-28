#include "Game.h"
#include "Ball.h"
namespace Arkanoid
{
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdateBall(float deltaTime)
	{
		Ball::GetInstance().UpdataBallDirection();
		Ball::GetInstance().CheckBallCollitions();
		Ball::GetInstance().UpdateBallPosition(deltaTime);
	}
	//------------------------------------------------------------------------------------------------------------
	Ball::Ball()
	{
		assert(m_ballTexture.loadFromFile(RESOURCES_PATH + "Ball.png"));

		m_sprite.setTexture(m_ballTexture);
		Math::GetInstance().SetSpriteSize(m_sprite, BALL_SIZE, BALL_SIZE);
		Math::GetInstance().SetSpriteRelativeOrigin(m_sprite, 0.5f, 0.5f);
		m_ballPosition = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f};
		m_sprite.setPosition(m_ballPosition.x, m_ballPosition.y);
		m_scalarSpeed = INITIAL_BALL_SPEED;

		m_ballVectorSpeed = Math::GetInstance().CalculateReboundSpeedByBase(m_ballPosition.x, m_scalarSpeed, m_ballPosition.x, 10.0f, BASE_REBOUND_MAX_ANGLE);
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::CheckBallCollitions()
	{
		Position2D basePosition = Base::GetInstance().GetPosition();
		Position2D baseSize = Base::GetInstance().GetBaseSize();

		if (Math::GetInstance().IsCicleRectangleCollition(m_ballPosition, BALL_SIZE * 0.5f, basePosition, baseSize))
		{
			m_ballVectorSpeed = Math::GetInstance().CalculateReboundSpeedByBase(m_ballPosition.x, INITIAL_BALL_SPEED, basePosition.x, baseSize.x, BASE_REBOUND_MAX_ANGLE);
			m_ballPosition.y = basePosition.y - (BALL_SIZE * 0.5f + BASE_SEGMENT_SIZE);
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdataBallDirection()
	{
		if (m_ballPosition.x <= 0 || m_ballPosition.x >= SCREEN_WIDTH)
		{
			m_ballVectorSpeed = { -1.0f * m_ballVectorSpeed.x,  m_ballVectorSpeed.y};
			UpdateBallPosition(0.05f);
		}
		else if (m_ballPosition.y <=0 || m_ballPosition.y >= SCREEN_HEIGHT)
		{
			m_ballVectorSpeed = { m_ballVectorSpeed.x,  -1.0f * m_ballVectorSpeed.y };
			UpdateBallPosition(0.05f);
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdateBallPosition(float deltaTime)
	{
		m_ballPosition = {m_ballPosition.x + m_ballVectorSpeed.x * deltaTime, m_ballPosition.y + m_ballVectorSpeed.y * deltaTime};
		m_sprite.setPosition(m_ballPosition.x, m_ballPosition.y);
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::DrawBall(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}
}