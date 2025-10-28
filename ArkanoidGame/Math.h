#pragma once

namespace sf
{
	class Sprite;
}

namespace Arkanoid
{
	enum class Direction
	{
		Right = 0,
		Up,
		Left,
		Down
	};

	struct Vector2D
	{
		float x = 0;
		float y = 0;
	};

	typedef Vector2D Position2D;

	class Math
	{
	public:

		static Math& GetInstance()
		{
			static Math math;
			return math;
		}

		float GetNearest(float origin, float min, float max);
		void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight);
		void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY);
		bool IsRectanglesCollide(Position2D rect1Position, Vector2D rect1Size, Position2D rect2Position, Vector2D rect2Size);
		bool IsCirclesCollide(Position2D circle1Position, float circle1Radius, Position2D circle2Position, float circle2Radius);
		bool IsCicleRectangleCollition(Position2D circlePosition, float circleRadius, Position2D rectPosition, Position2D rectSize);
		Position2D CalculateReboundSpeedByBase(float pointPosition, float pointSpeed, float basePosition, float baseSize, float maxReboundAngle);

	private:

		Math() {}
		~Math() {}

		Math(Math const&) = delete;
		Math& operator = (Math const&) = delete;
	};
}
