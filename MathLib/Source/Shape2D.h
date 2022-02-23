#pragma once

#include "Vector.h"

namespace MathLib
{
	struct LineSegment2D;
	struct Ray2D;

	// Rect defined in 2D Space.
	struct Rect2D
	{
		Vector2 size;
		Vector2 center;

		explicit Rect2D(const Vector2& min, const Vector2& max);
		explicit Rect2D(const Vector2& center, float size);
		explicit Rect2D(const Vector2& center, float sizeX, float sizeY);

		const Vector2 GetNormal() const;
		Vector2 GetMin() const;
		Vector2 GetMax() const;

		bool Intersects(const LineSegment2D& lineSegment) const;
		bool Intersects(const LineSegment2D& lineSegment, Vector2& point) const;
		bool Intersects(const Ray2D& ray) const;
		bool Intersects(const Ray2D& ray, Vector2& point) const;

		bool IsPointWithin(const Vector2& point) const;
		friend bool IsPointWithin(const Rect2D& box, const Vector2& point);
	};

	// Circle defined in 2D Space.
	struct Circle2D
	{
		Vector2 center = Vector2::Zero;
		float radius = 1.0f;

		explicit Circle2D() = default;
		explicit Circle2D(const Vector2& center, float radius);

		bool IsPointWithin(const Vector2& point) const;
		const Vector3 GetNormal() const;

		bool Intersects(const LineSegment2D& lineSegment) const;
		bool Intersects(const LineSegment2D& lineSegment, Vector2& point) const;
		bool Intersects(const Ray2D& ray) const;
		bool Intersects(const Ray2D& ray, Vector2& point) const;

		friend bool IsPointWithin(const Circle2D& circle, const Vector2& point);

		friend bool Intersects(const Circle2D& circle, const LineSegment2D& lineSegment);
		friend bool Intersects(const Circle2D& circle, const LineSegment2D& lineSegment, Vector2& point);
		friend bool Intersects(const Circle2D& circle, const Ray2D& ray);
		friend bool Intersects(const Circle2D& circle, const Ray2D& ray, Vector2& point);
	};

	// Triangle defined in 2D space.
	struct Triangle2D
	{
		Vector2 point1 = Vector2::Zero;
		Vector2 point2 = Vector2::Zero;
		Vector2 point3 = Vector2::Zero;

		explicit Triangle2D() = default;
		explicit Triangle2D(const Vector2& p1, const Vector2& p2, const Vector2& p3);

		const Vector2 GetCenter() const;
		void SetCenter(const Vector2& centerPosition);
		const Vector3 GetNormal() const;

		bool IsPointWithin(const Vector2& point) const;
		friend bool IsPointWithin(const Triangle2D& triangle, const Vector2& point);
	};
}