#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Objects.h"
#include "Math.h"
#include "Interfaces.h"

namespace Arkanoid
{
	class Game;

	enum BlockType
	{
		Normal = 0,
		ScoreUp,
		Poison,
		Disorient,
		Wall
	};
	enum BlockDamage
	{
		Clear = 0,
		Crack,
		Broken
	};

	class Block : public GameObject, public Collidable
	{
	public:

		Block(int health, Vector2D size, Vector2D position);
		void Visualize(sf::RenderWindow& window) override;
		void OnHit() override;

	protected:

		virtual void DestroyAction();

		sf::Texture m_frameCrackTexture;
		sf::Texture m_frameBrokenTexture;
		sf::RectangleShape m_blockShape;
		sf::Color m_color = sf::Color::White;
		int m_maxHealth = 1;
		int m_currentHealth = 1;
	};

	class BlockScoreUp final : public Block 
	{
	public:

		BlockScoreUp(int health, Vector2D size, Vector2D position);

	private:
		void OnHit() override { Block::OnHit(); }
		void DestroyAction() override;
	};

	class PoisonBlock final  : public Block
	{
	public:

		PoisonBlock(int health, Vector2D size, Vector2D position);

	private:
		void OnHit() override { Block::OnHit(); }
		void DestroyAction() override;
	};

	class DisorientBlock final : public Block
	{
	public:

		DisorientBlock(int health, Vector2D size, Vector2D position);

	private:
		void OnHit() override { Block::OnHit(); }
		void DestroyAction() override;
	};

}
