#pragma once
#include <memory>
#include "SFML/Graphics.hpp"

namespace Arkanoid
{
	class Collidable
	{

	public:

		virtual void OnHit() = 0;
	};
}