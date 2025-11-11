#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Math.h"
#include "Objects.h"
#include "Interfaces.h"

namespace Arkanoid
{
	class GameObject;

	class Ball : public GameObject, public Collidable

	{
	public:

		Ball(Vector2D size, Vector2D position, Vector2D vectorSpeed);
		~Ball() {}
		virtual void UpdateBall(float deltaTime, sf::RenderWindow& window);
		void OnHit() override {}
	protected:

		virtual void CheckCollisions() {};
		void UpdateBallPosition(float deltaTime);

		float m_scalarSpeed = 0.0f;
		Vector2D m_vectorSpeed = { 100.0f, 100.0f };
	};

	class MainBall final : public Ball

	{
	public:

		MainBall(Vector2D size, Vector2D position, Vector2D vectorSpeed);
		~MainBall() {}

		void UpdateBall(float deltaTime, sf::RenderWindow& window) override;
		void SetPosition(Vector2D position);
		void SetVectorSpeed(Vector2D vectorSpeed);
		void OnHit() override {}

	private:

		void CheckCollisions() override;
		float m_currentAngle = 0.0f;
	};

	class PoisonBall final : public Ball

	{
	public:

		PoisonBall(Vector2D size, Vector2D position, Vector2D vectorSpeed);
		~PoisonBall() {}
		void OnHit() override;
	private:

		void CheckCollisions() override;
	};

	class DesorientBall final : public Ball

	{
	public:

		DesorientBall(Vector2D size, Vector2D position, Vector2D vectorSpeed);
		~DesorientBall() {}
		void OnHit() override;
	};


}