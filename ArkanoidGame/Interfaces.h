#pragma once
#include <iostream>

namespace Arkanoid
{
	class ICollidable
	{

	public:

		virtual void OnHit(ICollidable& other) = 0;
	};

	class ISerializable 

	{
	public:
		virtual ~ISerializable() = default;

		virtual void Serialize(std::ostream& out) const = 0;
		virtual void Deserialize(std::istream& in) = 0;
	};
}