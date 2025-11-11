#include "Block.h"
#include "Ball.h"
#include "Game.h"

//For next rev
namespace Arkanoid
{
	//Block
	//-----------------------------------------------------------------------------------------------------------
	Block::Block(Vector2D size, Vector2D position) : GameObject(RESOURCES_PATH + "FrameClear.png", size, position)
	{
		SetColor(sf::Color::White);
		UI::GetInstance().InitShape(m_blockShape, size.x, size.y, 0.9f, sf::Color::White, position.x, position.y);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Block::Visualize(sf::RenderWindow& window)
	{
		window.draw(m_blockShape);
		GameObject::Draw(window);
	}
	//DestractedBlock
	//-----------------------------------------------------------------------------------------------------------
	DistractedBlock::DistractedBlock(int health, Vector2D size, Vector2D position) : Block(size, position)
	{
		assert(m_frameCrackTexture.loadFromFile(RESOURCES_PATH + "FrameCrack.png"));
		assert(m_frameBrokenTexture.loadFromFile(RESOURCES_PATH + "FrameBroken.png"));
		m_maxHealth = health;
		m_currentHealth = health;
		sf::Color color = sf::Color(100, 255 - health * 25, 255 - health * 25, 255);
		SetColor(sf::Color(color));
		UI::GetInstance().InitShape(m_blockShape, size.x, size.y, 0.9f, sf::Color::White, position.x, position.y);
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::OnHit()
	{
		float healthPersent = 0.0f;

		m_currentHealth--;
		if (m_currentHealth <= 0)
		{
			StartTimer(0.20f);
		}
		else
		{
			healthPersent = float(m_currentHealth) / float(m_maxHealth);

			if (healthPersent > 0.33f && healthPersent < 0.66f)
			{
				m_sprite.setTexture(m_frameCrackTexture);
			}
			else if (healthPersent < 0.33f)
			{
				m_sprite.setTexture(m_frameBrokenTexture);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::UpdateBlock(float deltaTime)
	{
		UpdateTimer(deltaTime);
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::FinalAction()
	{
		GameStateInRuntime* gameState = Game::GetInstance().GetRuntimeGameState().get();
		if (gameState)
		{
			gameState->AddScore(m_maxHealth);
			gameState->RemoveObject(*this);
			gameState->SubtractBlockCount();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void DistractedBlock::EachTickAction(float deltaTime)
	{
		sf::Color spriteColor = m_sprite.getColor();
		sf::Color shapeColor = m_blockShape.getFillColor();

		if (spriteColor.a > 0)
		{
			spriteColor.a -= 10;
			shapeColor.a = 0;
		}
		m_sprite.setColor(spriteColor);
		m_blockShape.setFillColor(shapeColor);
	}
	//BlockScoreUp
	//-----------------------------------------------------------------------------------------------------------
	BlockScoreUp::BlockScoreUp(int health, Vector2D size, Vector2D position) : DistractedBlock(health, size, position)
	{
		SetColor(sf::Color::Magenta);
	}
	//-----------------------------------------------------------------------------------------------------------
	void BlockScoreUp::FinalAction()
	{
		GameStateInRuntime* gameState = Game::GetInstance().GetRuntimeGameState().get();
		if (gameState)
		{
			gameState->RemoveObject(*this);
			gameState->AddScore(m_maxHealth * 10);
			gameState->SubtractBlockCount();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	PoisonBlock::PoisonBlock(int health, Vector2D size, Vector2D position) : DistractedBlock(health, size, position)
	{
		SetColor(sf::Color::Green);
	}
	//-----------------------------------------------------------------------------------------------------------
	void PoisonBlock::FinalAction()
	{
		Vector2D size = { BALL_SIZE, BALL_SIZE };
		Vector2D vectorSpeed = { 0.0f, 1.0f };
		GameStateInRuntime* gameState = Game::GetInstance().GetRuntimeGameState().get();
		if (gameState)
		{
			DistractedBlock::FinalAction();
			gameState->AddAdditionalBall(std::make_shared<PoisonBall>(size, m_position, vectorSpeed));
		}
	}
	//DisorientBlock
	//-----------------------------------------------------------------------------------------------------------
	DisorientBlock::DisorientBlock(int health, Vector2D size, Vector2D position) : DistractedBlock(health, size, position)
	{
		SetColor(sf::Color::Blue);
	}
	//-----------------------------------------------------------------------------------------------------------
	void DisorientBlock::FinalAction()
	{
		Vector2D size = { BALL_SIZE, BALL_SIZE };
		Vector2D vectorSpeed = { 0.0f, 1.0f };
		GameStateInRuntime* gameState = Game::GetInstance().GetRuntimeGameState().get();
		if (gameState)
		{
			DistractedBlock::FinalAction();
			gameState->AddAdditionalBall(std::make_shared<DesorientBall>(size, m_position, vectorSpeed));
		}
	}
}
