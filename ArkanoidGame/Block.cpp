#include "Block.h"
#include "Ball.h"
#include "Game.h"

//For next rev
namespace Arkanoid
{
	//Block
	//-----------------------------------------------------------------------------------------------------------
	Block::Block(int health, Vector2D size, Vector2D position) : GameObject(RESOURCES_PATH + "FrameClear.png", size, position)
	{
		assert(m_frameCrackTexture.loadFromFile(RESOURCES_PATH + "FrameCrack.png"));
		assert(m_frameBrokenTexture.loadFromFile(RESOURCES_PATH + "FrameBroken.png"));
		m_maxHealth = health;
		m_currentHealth = health;
		SetColor(sf::Color::White);
		UI::GetInstance().InitShape(m_blockShape, BALL_SIZE * 2.0f, BALL_SIZE, 1.0f, sf::Color::White, position.x, position.y);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Block::OnHit()
	{
		m_currentHealth--;
		if (m_currentHealth <=  0)
		{
			Block::DestroyAction();
		}

	}
	//-----------------------------------------------------------------------------------------------------------
	void Block::Visualize(sf::RenderWindow& window)
	{
		window.draw(m_blockShape);
		GameObject::Draw(window);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Block::DestroyAction()
	{
		GameStateInRuntime* RuntimeGameState = static_cast<GameStateInRuntime*>(Game::GetInstance().GetCurrentGameState().get());
		RuntimeGameState->AddScore(m_maxHealth);
		RuntimeGameState->RemoveObject(*this);

	}
	//BlockScoreUp
	//-----------------------------------------------------------------------------------------------------------
	BlockScoreUp::BlockScoreUp(int health, Vector2D size, Vector2D position) : Block(health, size, position)
	{
		SetColor(sf::Color::Magenta);
		m_blockShape.setFillColor(sf::Color::Magenta);
	}
	//-----------------------------------------------------------------------------------------------------------
	void BlockScoreUp::DestroyAction()
	{
		GameStateInRuntime* RuntimeGameState = static_cast<GameStateInRuntime*>(Game::GetInstance().GetCurrentGameState().get());
		RuntimeGameState->RemoveObject(*this);
		Game::GetInstance().GetCurrentGameState()->AddScore(m_maxHealth * 10);
	}
	//-----------------------------------------------------------------------------------------------------------
	PoisonBlock::PoisonBlock(int health, Vector2D size, Vector2D position) : Block(health, size, position)
	{
		SetColor(sf::Color::Green);
		UI::GetInstance().InitShape(m_blockShape, BALL_SIZE, BALL_SIZE * 2.0f, 1.0f, sf::Color::Green, position.x, position.y);
	}

	void PoisonBlock::DestroyAction()
	{
		Vector2D size = { BALL_SIZE, BALL_SIZE };
		Vector2D vectorSpeed = { -1.0f, 0.0f };

		GameStateInRuntime* RuntimeGameState = static_cast<GameStateInRuntime*>(Game::GetInstance().GetCurrentGameState().get());
		Block::DestroyAction();
		RuntimeGameState->RemoveObject(*this);
		RuntimeGameState->AddAdditionalBall(std::make_shared<PoisonBall>(size, m_position, vectorSpeed));
	}
	//DisorientBlock
	//-----------------------------------------------------------------------------------------------------------
	DisorientBlock::DisorientBlock(int health, Vector2D size, Vector2D position) : Block(health, size, position)
	{
		SetColor(sf::Color::Blue);
		UI::GetInstance().InitShape(m_blockShape, BALL_SIZE, BALL_SIZE * 2.0f, 1.0f, sf::Color::Blue, position.x, position.y);
	}
	//-----------------------------------------------------------------------------------------------------------
	void DisorientBlock::DestroyAction()
	{
		Vector2D size = { BALL_SIZE, BALL_SIZE };
		Vector2D vectorSpeed = { -1.0f, 0.0f };

		GameStateInRuntime* RuntimeGameState = static_cast<GameStateInRuntime*>(Game::GetInstance().GetCurrentGameState().get());
		Block::DestroyAction();
		RuntimeGameState->RemoveObject(*this);
		RuntimeGameState->AddAdditionalBall(std::make_shared<DesorientBall>(size, m_position, vectorSpeed));
	}

}
