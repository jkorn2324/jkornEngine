#pragma once

#include "Vector.h"

namespace MathLib
{

	struct LineSegment2D
	{
		Vector2 start;
		Vector2 end;

		LineSegment2D(const Vector2& start, const Vector2& end)
			: start(start), end(end) { }

		const Vector2 GetDirection() const
		{
			Vector2 difference = end - start;
			difference.Normalize();
			return difference;
		}

		const float GetDistance() const
		{
			Vector2 difference = end - start;
			return difference.Length();
		}

		friend bool Intersects(const LineSegment2D& a, const LineSegment2D& b);
		friend bool Intersects(const LineSegment2D& a, const LineSegment2D& b, Vector2& intersectedPoint);
	};

	struct Ray2D
	{
		Vector2 startPoint;
		Vector2 direction;
		float distance = 0.0f;

		Ray2D(const Vector2& point, const Vector2& direction)
			: startPoint(point), direction(direction) { }
		Ray2D(const Vector2& point, const Vector2& direction, float distance)
			: startPoint(point), direction(direction), distance(distance) { }

		bool IsInfinite() const { return distance <= 0.0f; }

		friend bool Intersects(const Ray2D& a, const Ray2D& b);
		friend bool Intersects(const Ray2D& a, const Ray2D& b, Vector2& intersectedPoint);
	};
}