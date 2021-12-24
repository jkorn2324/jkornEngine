#pragma once

#include "Vector.h"

namespace MathLib
{

	struct Rect2D
	{
		Vector2 size;
		Vector2 center;

		explicit Rect2D(const Vector2& min, const Vector2& max);
		explicit Rect2D(const Vector2& center, float size);
		explicit Rect2D(const Vector2& center, float sizeX, float sizeY);

		Vector2 GetMin() const;
		Vector2 GetMax() const;

		bool IsPointWithin(const Vector2& point) const;

		friend bool IsPointWithin(const Rect2D& box, const Vector2& point);
	};

	struct Rect3D
	{
		Vector3 center;
		Vector3 size;

		explicit Rect3D(const Vector3& min, const Vector3& max);
		explicit Rect3D(const Vector3& center, float size);
		explicit Rect3D(const Vector3& center, float sizeX, float sizeY, float sizeZ);

		Vector3 GetMin() const;
		Vector3 GetMax() const;

		bool IsPointWithin(const Vector3& point) const;

		friend bool IsPointWithin(const Rect3D& box, const Vector3& point);
	};
}