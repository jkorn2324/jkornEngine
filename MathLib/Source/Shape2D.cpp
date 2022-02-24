#include "MathPCH.h"
#include "Shape2D.h"

#include "Math.h"
#include "Geometry2D.h"

namespace MathLib
{

	Rect2D::Rect2D(const Vector2& min, const Vector2& max)
		: center(), size()
	{
		size = max - min;
		center = min + size * 0.5f;
	}

	Rect2D::Rect2D(const Vector2& center, float size)
		: center(center), size(size, size)
	{
	}

	Rect2D::Rect2D(const Vector2& center, float sizeX, float sizeY)
		: center(center), size(sizeX, sizeY)
	{
	}

	Vector2 Rect2D::GetMax() const
	{
		return center + size * 0.5f;
	}

	bool Rect2D::Intersects(const LineSegment2D& lineSegment) const
	{		
		// TODO: Implementation
		return false;
	}

	bool Rect2D::Intersects(const LineSegment2D& lineSegment, Vector2& point) const
	{		
		// TODO: Implementation
		return false;
	}

	bool Rect2D::Intersects(const Ray2D& ray) const
	{
		// TODO: Implementation
		return false;
	}

	bool Rect2D::Intersects(const Ray2D& ray, Vector2& point) const
	{
		// TODO: Implementation
		return false;
	}

	Vector2 Rect2D::GetMin() const
	{
		return center - size * 0.5f;
	}

	const Vector2 Rect2D::GetNormal() const
	{
		return Vector3::UnitZ;
	}

	bool Rect2D::IsPointWithin(const Vector2& point) const
	{
		Vector2 min = GetMin();
		Vector2 max = GetMax();

		return min.x < point.x
			&& min.y < point.y
			&& max.x > point.x
			&& max.y > point.x;
	}

	bool IsPointWithin(const Rect2D& box, const Vector2& point)
	{
		return box.IsPointWithin(point);
	}
	

	Circle2D::Circle2D(const Vector2& center, float radius)
		: center(center), radius(radius) { }
	
	bool Circle2D::IsPointWithin(const Vector2& point) const
	{
		float distanceSquared = (center - point).LengthSquared();
		return distanceSquared <= radius * radius;
	}

	const Vector3 Circle2D::GetNormal() const
	{
		return Vector3::UnitZ;
	}

	bool Circle2D::Intersects(const LineSegment2D& segment) const
	{
		Vector2 point;
		return Intersects(segment, point);
	}

	bool Circle2D::Intersects(const LineSegment2D& lineSegment, Vector2& point) const
	{
		Vector2 segmentDirection = lineSegment.GetDirection();
		Vector2 differenceToStart = lineSegment.start - center;
		float sProjected = Dot(differenceToStart, segmentDirection);
		Vector2 comparedPoint = lineSegment.start + segmentDirection * sProjected;
		if (!lineSegment.IsPointOnSegment(comparedPoint))
		{
			return false;
		}
		float distanceSquared = (center - comparedPoint).LengthSquared();
		float radiusSquared = radius * radius;
		if (distanceSquared > radiusSquared)
		{
			return false;
		}
		// Gets the collided point from the distance to the collision points.
		float differenceToCollisionPoints = Sqrt(radiusSquared - distanceSquared);
		point = comparedPoint - segmentDirection * differenceToCollisionPoints;
		return true;
	}

	bool Circle2D::Intersects(const Ray2D& ray) const
	{
		Vector2 point;
		return Intersects(ray, point);
	}

	bool Circle2D::Intersects(const Ray2D& ray, Vector2& point) const
	{
		Vector2 differenceToStart = ray.startPoint - center;
		float sProjected = Dot(differenceToStart, ray.direction);
		Vector2 pointOnRay = ray.startPoint + ray.direction * sProjected;
		if (!ray.IsPointWithin(pointOnRay))
		{
			return false;
		}
		float distanceSquared = (center - pointOnRay).LengthSquared();
		float radiusSquared = radius * radius;
		if (distanceSquared > radiusSquared)
		{
			return false;
		}
		// Gets the collided point from the distance to the collision points.
		float differenceToCollisionPoints = Sqrt(radiusSquared - distanceSquared);
		point = pointOnRay - ray.direction * differenceToCollisionPoints;
		return true;
	}

	bool IsPointWithin(const Circle2D& circle, const Vector2& point)
	{
		return circle.IsPointWithin(point);
	}

	bool Intersects(const Circle2D& circle, const LineSegment2D& segment)
	{
		return circle.Intersects(segment);
	}

	bool Intersects(const Circle2D& circle, const LineSegment2D& segment, Vector2& point)
	{
		return circle.Intersects(segment, point);
	}

	bool Intersects(const Circle2D& circle, const Ray2D& ray)
	{
		return circle.Intersects(ray);
	}

	bool Intersects(const Circle2D& circle, const Ray2D& ray, Vector2& point)
	{
		return circle.Intersects(ray, point);
	}
	
	
	Triangle2D::Triangle2D(const Vector2& p1, const Vector2& p2, const Vector2& p3)
		: point1(p1), point2(p2), point3(p3)
	{
	}

	const Vector2 Triangle2D::GetCenter() const
	{
		Vector2 total = point1 + point2 + point3;
		return Vector2(
			total.x / 3.0f, total.y / 3.0f);
	}

	void Triangle2D::SetCenter(const Vector2& centerPosition)
	{
		Vector2 centerDifference = centerPosition - GetCenter();
		point1 += centerDifference;
		point2 += centerDifference;
		point3 += centerDifference;
	}

	const Vector3 Triangle2D::GetNormal() const
	{
		return Vector3::UnitZ;
	}

	bool Triangle2D::IsPointWithin(const Vector2& point) const
	{
		float point1Cross = Cross(Normalize(point - point1),
			Normalize(point2 - point1));
		float point2Cross = Cross(Normalize(point - point2),
			Normalize(point3 - point2));
		float point3Cross = Cross(Normalize(point - point3),
			Normalize(point1 - point3));
		return point1Cross >= 0.0f == point2Cross >= 0.0f
			&& point2Cross >= 0.0f == point3Cross >= 0.0f;
	}

	bool IsPointWithin(const Triangle2D& triangle, const Vector2& point)
	{
		return triangle.IsPointWithin(point);
	}
}