#include "MathPCH.h"
#include "Geometry3D.h"
#include "MathLib.h"

namespace MathLib
{
	static float DEFAULT_MAX_DISTANCE = 0.5f;

	bool Intersects(const LineSegment3D& a, const LineSegment3D& b)
	{
		return Intersects(a, b, DEFAULT_MAX_DISTANCE);
	}
	
	bool Intersects(const LineSegment3D& a, const LineSegment3D& b, float maxDistance)
	{
		Vector3 intersectedPoint;
		return Intersects(a, b, DEFAULT_MAX_DISTANCE, intersectedPoint);
	}
	
	bool Intersects(const LineSegment3D& a, const LineSegment3D& b, Vector3& intersectedPoint)
	{
		return Intersects(a, b, DEFAULT_MAX_DISTANCE, intersectedPoint);
	}
	
	bool Intersects(const LineSegment3D& a, const LineSegment3D& b, float maxDistance, Vector3& intersectedPoint)
	{
		// TODO: Swept sphere implementation
		return false;
	}

	bool Intersects(const Ray3D& a, const Ray3D& b)
	{
		return Intersects(a, b, DEFAULT_MAX_DISTANCE);
	}
	
	bool Intersects(const Ray3D& a, const Ray3D& b, float maxDistance)
	{
		Vector3 intersectedPoint;
		return Intersects(a, b, DEFAULT_MAX_DISTANCE, intersectedPoint);
	}
	
	bool Intersects(const Ray3D& a, const Ray3D& b, Vector3& intersectedPoint)
	{
		return Intersects(a, b, DEFAULT_MAX_DISTANCE, intersectedPoint);
	}
	
	bool Intersects(const Ray3D& a, const Ray3D& b, float maxDistance, Vector3& intersectedPoint)
	{
		// TODO: Swept sphere implementation
		return false;
	}

	bool Plane3D::Intersects(const LineSegment3D& segment) const
	{
		Vector3 point;
		return Intersects(segment, point);
	}

	bool Plane3D::Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const
	{
		float distance = segment.GetDistance();
		Vector3 direction = segment.GetDirection();
		float dotWithRayAndNormal = Dot(direction * distance, normal);
		// Plane3D and line are never going to intersect.
		if (IsCloseEnough(dotWithRayAndNormal, 0.0f))
		{
			return false;
		}
		float dotWithStartAndNormal = -Dot(segment.start, normal) - distanceFromZero;
		float t = dotWithStartAndNormal / dotWithRayAndNormal;
		if (t >= 1.0f)
		{
			return false;
		}
		intersectedPoint = direction * distance * t + segment.start;
		return IsPointWithin(intersectedPoint);
	}

	bool Plane3D::Intersects(const Ray3D& ray) const
	{
		Vector3 intersectedPoint;
		return Intersects(ray, intersectedPoint);
	}

	bool Plane3D::Intersects(const Ray3D& ray, Vector3& intersectedPoint) const
	{
		float distance = ray.IsInfinite() ? 1.0f : ray.distance;
		float dotWithRayAndNormal = Dot(ray.direction * distance, normal);
		// States that the ray is parallel to the plane.
		if (IsCloseEnough(dotWithRayAndNormal, 0.0f))
		{
			return false;
		}
		float dotWithStartAndNormal = -Dot(ray.startPoint, normal) - distanceFromZero;
		float t = dotWithStartAndNormal / dotWithRayAndNormal;
		if (t <= 0.0f)
		{
			return false;
		}

		if (t >= 1.0f && !ray.IsInfinite())
		{
			return false;
		}
		intersectedPoint = t * ray.direction * distance + ray.startPoint;
		return IsPointWithin(intersectedPoint);
	}

	bool Plane3D::IsPointWithin(const Vector3& point) const
	{
		return IsCloseEnough(Dot(point, normal) + distanceFromZero, 0.0f);
	}

	bool Intersects(const Plane3D& plane, const LineSegment3D& lineSegment)
	{
		Vector3 intersectedPoint;
		return plane.Intersects(lineSegment, intersectedPoint);
	}

	bool Intersects(const Plane3D& plane, const LineSegment3D& lineSegment, Vector3& intersectedPoint)
	{
		return plane.Intersects(lineSegment, intersectedPoint);
	}

	bool Intersects(const Plane3D& plane, const Ray3D& ray)
	{
		Vector3 intersectedPoint;
		return plane.Intersects(ray, intersectedPoint);
	}

	bool IsPointWithin(const Plane3D& plane, const Vector3& point)
	{
		return plane.IsPointWithin(point);
	}

	bool Intersects(const Plane3D& plane, const Ray3D& ray, Vector3& intersectedPoint)
	{
		return plane.Intersects(ray, intersectedPoint);
	}
}