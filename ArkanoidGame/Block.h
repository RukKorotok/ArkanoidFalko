#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Objects.h"
#include "Math.h"
#include "Interfaces.h"
#include "SmothTimer.h"

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

		Block(Vector2D size, Vector2D position);
		void Visualize(sf::RenderWindow& window) override;
		void OnHit() override {};
		virtual void UpdateBlock(float deltaTime) {}

	protected:

		sf::RectangleShape m_blockShape;
		sf::Color m_color = sf::Color::White;
	};

	class DistractedBlock : public Block , public SmothTimer
	{
	public:

		DistractedBlock(int health, Vector2D size, Vector2D position);
		void OnHit() override;
		void UpdateBlock(float deltaTime) override;

	protected:

		void FinalAction() override;
		void EachTickAction(float deltaTime) override;

		sf::Texture m_frameCrackTexture;
		sf::Texture m_frameBrokenTexture;
		sf::Color m_color = sf::Color::White;
		int m_maxHealth = 1;
		int m_currentHealth = 1;
	};


	class BlockScoreUp final : public DistractedBlock
	{
	public:

		BlockScoreUp(int health, Vector2D size, Vector2D position);

	private:
		void OnHit() override { DistractedBlock::OnHit(); }
		void FinalAction() override;
		void EachTickAction(float deltaTime) override { DistractedBlock::EachTickAction(deltaTime); }
	};

	class PoisonBlock final  : public DistractedBlock
	{
	public:

		PoisonBlock(int health, Vector2D size, Vector2D position);

	private:
		void OnHit() override { DistractedBlock::OnHit(); }
		void FinalAction() override;
		void EachTickAction(float deltaTime) override { DistractedBlock::EachTickAction(deltaTime); }
	};

	class DisorientBlock final : public DistractedBlock
	{
	public:

		DisorientBlock(int health, Vector2D size, Vector2D position);

	private:
		void OnHit() override { DistractedBlock::OnHit(); }
		void FinalAction() override;
		void EachTickAction(float deltaTime) override { DistractedBlock::EachTickAction(deltaTime); }
	};

}
