#include "Game.h"
#include "Ball.h"
namespace Arkanoid
{
	//------------------------------------------------------------------------------------------------------------
	Ball::Ball()
	{
		assert(m_ballTexture.loadFromFile(RESOURCES_PATH + "Ball.png"));

		m_sprite.setTexture(m_ballTexture);
		Math::GetInstance().SetSpriteSize(m_sprite, BALL_SIZE, BALL_SIZE);
		Math::GetInstance().SetSpriteRelativeOrigin(m_sprite, 0.5f, 0.5f);
		m_position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
		m_sprite.setPosition(m_position.x, m_position.y);
		m_scalarSpeed = INITIAL_BALL_SPEED;

		m_ballVectorSpeed = Math::GetInstance().CalculateReboundSpeedByBase(m_position.x, m_scalarSpeed, m_position.x, 10.0f, BASE_REBOUND_MAX_ANGLE);
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdateBall(float deltaTime)
	{
		UpdataBallDirection();
		UpdateBallPosition(deltaTime);
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::SetPosition(Position2D position)
	{
		m_position = position;
	}
	//-----------------------------------------------------------------------------------------------------------
	Position2D Ball::GetPosition()
	{
		return m_position;
	}
	//----------------------------------------------------------------------------------------------------------
	void Ball::SetVectorSpeed(Position2D vectorSpeed)
	{
		m_ballVectorSpeed = vectorSpeed;
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::DrawBall(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdataBallDirection()
	{
		if (m_position.x <= 0 || m_position.x >= SCREEN_WIDTH)
		{
			m_ballVectorSpeed = { -1.0f * m_ballVectorSpeed.x,  m_ballVectorSpeed.y};
			UpdateBallPosition(0.05f);
		}
		else if (m_position.y <=0 || m_position.y >= SCREEN_HEIGHT)
		{
			m_ballVectorSpeed = { m_ballVectorSpeed.x,  -1.0f * m_ballVectorSpeed.y };
			UpdateBallPosition(0.05f);
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdateBallPosition(float deltaTime)
	{
		m_position = {m_position.x + m_ballVectorSpeed.x * deltaTime, m_position.y + m_ballVectorSpeed.y * deltaTime};
		m_sprite.setPosition(m_position.x, m_position.y);
	}
}