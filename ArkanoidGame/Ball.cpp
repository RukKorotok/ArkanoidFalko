#include "Game.h"
#include "Ball.h"
namespace Arkanoid
{
	//Ball
	//------------------------------------------------------------------------------------------------------------
	Ball::Ball(Vector2D size, Vector2D position, Vector2D vectorSpeed) : GameObject(RESOURCES_PATH + "Ball.png", size, position)
	{
		m_scalarSpeed = INITIAL_BALL_SPEED;
		m_vectorSpeed = vectorSpeed;
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdateBall(float deltaTime, sf::RenderWindow& window)
	{
		UpdateBallPosition(deltaTime);
		Draw(window);
	}
	//------------------------------------------------------------------------------------------------------------
	void Ball::UpdateBallPosition(float deltaTime)
	{
		m_position = { m_position.x + m_scalarSpeed * m_vectorSpeed.x * deltaTime, m_position.y + m_scalarSpeed * m_vectorSpeed.y * deltaTime };
		m_sprite.setPosition(m_position.x, m_position.y);
	}
	//MainBall
	//------------------------------------------------------------------------------------------------------------
	MainBall::MainBall(Vector2D size, Vector2D position, Vector2D vectorSpeed) : Ball(size, position, vectorSpeed) {}
	//------------------------------------------------------------------------------------------------------------
	void MainBall::UpdateBall(float deltaTime, sf::RenderWindow& window)
	{
		CheckCollisions();
		Ball::UpdateBall(deltaTime, window);
	}
	//------------------------------------------------------------------------------------------------------------
	void MainBall::SetPosition(Vector2D position)
	{
		m_position = position;
	}
	//----------------------------------------------------------------------------------------------------------
	void MainBall::SetVectorSpeed(Vector2D vectorSpeed)
	{
		m_vectorSpeed = vectorSpeed;
	}
	//------------------------------------------------------------------------------------------------------------
	void MainBall::CheckCollisions()
	{
		GameStateInRuntime* gameState = static_cast<GameStateInRuntime*>(Game::GetInstance().GetCurrentGameState().get());
		std::vector<std::shared_ptr<Block>> blocks = gameState->GetBlocks();
		Base* base = gameState->GetBase();

		for (const auto block : blocks)
		{
			if (Math::GetInstance().IsCicleRectangleCollition(m_position, m_size.x * 0.5f, block->GetPosition(), block->GetSize()))
			{
				SetVectorSpeed(Math::GetInstance().CalculateReboundSpeedByRectangle(m_position, m_size.x * 0.5f, m_vectorSpeed, block->GetPosition(), block->GetSize(), BASE_REBOUND_MAX_ANGLE));
				UpdateBallPosition(0.015f);
				OnHit();
				block->OnHit();
				return;
			}
		}

		if (Math::GetInstance().IsCicleRectangleCollition(m_position, m_size.x * 0.5f, base->GetPosition(), base->GetSize()))
		{
			SetVectorSpeed(Math::GetInstance().CalculateReboundSpeedByRectangle(m_position, m_size.x * 0.5f, m_vectorSpeed, base->GetPosition(), base->GetSize(), BASE_REBOUND_MAX_ANGLE));
			UpdateBallPosition(0.015f);
		}

		if (m_position.x <= 0 || m_position.x >= SCREEN_WIDTH)
		{
			m_vectorSpeed = { -1.0f * m_vectorSpeed.x,  m_vectorSpeed.y };
			UpdateBallPosition(0.05f);
		}
		else if (m_position.y <= 0)
		{
			m_vectorSpeed = { m_vectorSpeed.x,  -1.0f * m_vectorSpeed.y };
			UpdateBallPosition(0.05f);
		}
		else if (m_position.y >= SCREEN_HEIGHT)
		{
			GameStateInRuntime* RuntimeGameState = static_cast<GameStateInRuntime*>(Game::GetInstance().GetCurrentGameState().get());
			RuntimeGameState->RemoveObject(*this);
		}
	}
	//PoisonBall
	//------------------------------------------------------------------------------------------------------------
	PoisonBall::PoisonBall(Vector2D size, Vector2D position, Vector2D vectorSpeed) : Ball(size, position, vectorSpeed)
	{
		SetColor(sf::Color::Green);
	}
	//------------------------------------------------------------------------------------------------------------
	void PoisonBall::OnHit()
	{
		GameStateInRuntime* RuntimeGameState = static_cast<GameStateInRuntime*>(Game::GetInstance().GetCurrentGameState().get());
		RuntimeGameState->RemoveObject(*this);
		RuntimeGameState->SetPoison();
	}
	//DesorientBall
	//------------------------------------------------------------------------------------------------------------
	DesorientBall::DesorientBall(Vector2D size, Vector2D position, Vector2D vectorSpeed) : Ball(size, position, vectorSpeed)
	{
		SetColor(sf::Color::Blue);
	}
	//------------------------------------------------------------------------------------------------------------
	void DesorientBall::OnHit()
	{
		GameStateInRuntime* RuntimeGameState = static_cast<GameStateInRuntime*>(Game::GetInstance().GetCurrentGameState().get());
		RuntimeGameState->RemoveObject(*this);
		RuntimeGameState->SetDesorient();
	}
}