#include <cstdlib>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Game.h"

namespace Arkanoid
{
	//------------------------------------------------------------------------------------------------------------
	float Math::GetNearest(float origin, float min, float max)
	{
		if (origin < min)
		{
			return min;
		}
		else if (origin > max)
		{
			return max;
		}
		else
		{
			return origin;
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void Math::SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sf::Vector2f scale = {desiredWidth / spriteRect.width, desiredHeight / spriteRect.height};
		sprite.setScale(scale);
	}
	//------------------------------------------------------------------------------------------------------------
	void Math::SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sprite.setOrigin(originX * spriteRect.width, originY * spriteRect.height);
	}
	//------------------------------------------------------------------------------------------------------------
	bool Math::IsRectanglesCollide(Position2D rect1Position, Vector2D rect1Size, Position2D rect2Position, Vector2D rect2Size)
	{
		float dx = (float)fabs(rect1Position.x - rect2Position.x);
		float dy = (float)fabs(rect1Position.y - rect2Position.y);

		return (dx <= (rect1Size.x + rect2Size.x) / 2.f &&
			dy <= (rect1Size.y + rect2Size.y) / 2.f);
	}
	//------------------------------------------------------------------------------------------------------------
	bool Math::IsCirclesCollide(Position2D circle1Position, float circle1Radius, Position2D circle2Position, float circle2Radius)
	{
		float squareDistance = (circle1Position.x - circle2Position.x) *
			(circle1Position.x - circle2Position.x) +
			(circle1Position.y - circle2Position.y) * (circle1Position.y - circle2Position.y);
		float squareRadiusSum = (circle1Radius + circle2Radius) * (circle1Radius + circle2Radius);

		return squareDistance <= squareRadiusSum;
	}
	//------------------------------------------------------------------------------------------------------------
	bool Math::IsCicleRectangleCollition(Position2D circlePosition, float circleRadius, Position2D rectPosition, Position2D rectSize)
	{
		float closestPointX = GetNearest(circlePosition.x, rectPosition.x - rectSize.x * 0.5f, rectPosition.x + rectSize.x * 0.5f);
		float closestPointY = GetNearest(circlePosition.y, rectPosition.y - rectSize.y * 0.5f, rectPosition.y + rectSize.y * 0.5f);
		float distanceX = circlePosition.x - closestPointX;
		float distanceY = circlePosition.y - closestPointY;
		float distanceSquered = (distanceX * distanceX) + (distanceY * distanceY);

		return distanceSquered <= (circleRadius * circleRadius);
	}
	//------------------------------------------------------------------------------------------------------------
	Position2D Math::CalculateReboundSpeedByBase(float pointPosition, float pointSpeed, float basePosition, float baseSize, float maxReboundAngle)
	{
		const float normal_angle = 90.0f;
		float relativeXPosition = 0.0f;
		float normalizeXPosition = 0.0f;
		float reboundAngle = 0.0f;

		relativeXPosition =  pointPosition - basePosition;
		normalizeXPosition = relativeXPosition / baseSize * 0.5f;
		reboundAngle = normalizeXPosition * maxReboundAngle * 3.14f / 180.0f;

		return Position2D({ sin(reboundAngle) * pointSpeed, -cos(reboundAngle) * pointSpeed } );
	}
}
