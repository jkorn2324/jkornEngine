#pragma once

#include "Vector.h"

namespace MathLib
{

	struct LineSegment3D
	{
		Vector3 start;
		Vector3 end;

		LineSegment3D(const Vector3& start, const Vector3& end)
			: start(start), end(end) { }

		const Vector3 GetDirection() const
		{
			Vector3 difference = end - start;
			difference.Normalize();
			return difference;
		}

		const float GetDistance() const
		{
			Vector3 difference = end - start;
			return difference.Length();
		}

		friend bool Intersects(const LineSegment3D& a, const LineSegment3D& b);
		friend bool Intersects(const LineSegment3D& a, const LineSegment3D& b, float maxDistance);
		friend bool Intersects(const LineSegment3D& a, const LineSegment3D& b, Vector3& intersectedPoint);
		friend bool Intersects(const LineSegment3D& a, const LineSegment3D& b, float maxDistance, Vector3& intersectedPoint);
	};

	struct Ray3D
	{
		Vector3 startPoint;
		Vector3 direction;
		float distance = 0.0f;

		Ray3D(const Vector3& point, const Vector3& direction)
			: startPoint(point), direction(direction) { }
		Ray3D(const Vector3& point, const Vector3& direction, float distance)
			: startPoint(point), direction(direction), distance(distance) { }

		bool IsInfinite() const { return distance <= 0.0f; }

		friend bool Intersects(const Ray3D& a, const Ray3D& b);
		friend bool Intersects(const Ray3D& a, const Ray3D& b, float maxDistance);
		friend bool Intersects(const Ray3D& a, const Ray3D& b, Vector3& intersectedPoint);
		friend bool Intersects(const Ray3D& a, const Ray3D& b, float maxDistance, Vector3& intersectedPoint);
	};

	struct Plane3D
	{
		Vector3 normal = Vector3::UnitZ;
		float distanceFromZero = 0.0f;

		Plane3D() = default;
		Plane3D(const Vector3& normal)
			: normal(normal) { }
		Plane3D(const Vector3& normal, float distanceFromZero)
			: normal(normal), distanceFromZero(distanceFromZero) { }
		Plane3D(const Vector3& normal, const Vector3& position)
			: normal(normal), distanceFromZero(position.Length()) { }

		bool Intersects(const LineSegment3D& segment) const;
		bool Intersects(const LineSegment3D& segment, Vector3& interesectedPoint) const;
		bool Intersects(const Ray3D& ray) const;
		bool Intersects(const Ray3D& ray, Vector3& intersectedPoint) const;

		bool IsPointWithin(const Vector3& point) const;

		friend bool Intersects(const Plane3D& plane, const LineSegment3D& lineSegment, Vector3& intersectedPoint);
		friend bool Intersects(const Plane3D& plane, const LineSegment3D& lineSegment);
		friend bool Intersects(const Plane3D& plane, const Ray3D& ray, Vector3& intersectedPoint);
		friend bool Intersects(const Plane3D& plane, const Ray3D& ray);

		friend bool IsPointWithin(const Plane3D& plane, const Vector3& point);
	};
}