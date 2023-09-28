#include "MathPCH.h"
#include "Shape2D.h"

#include "MathLib.h"
#include "Geometry2D.h"

namespace MathLib
{

	struct SegmentIntersectionOutput
	{
		Vector2 intersectionPoint;
		bool intersected;
	};

	static void GetClosestPoint(Vector2& point, const Vector2& relativePoint,
		const Vector2& a, const Vector2& b)
	{
		float aDistance = (a - relativePoint).LengthSquared();
		float bDistance = (b - relativePoint).LengthSquared();
		point = aDistance <= bDistance ? a : b;
	}

	// ------------------------------------- Rect2D Implementation --------------------------

	static bool Rect2DTestPoints(Vector2& intersectedPoint, const Vector2& startPoint,
		const SegmentIntersectionOutput& a, const SegmentIntersectionOutput& b, const SegmentIntersectionOutput& c, const SegmentIntersectionOutput& d)
	{
		if (a.intersected)
		{
			if (b.intersected)
			{
				GetClosestPoint(intersectedPoint, startPoint,
					a.intersectionPoint, b.intersectionPoint);
				return true;
			}
			else if (c.intersected)
			{
				GetClosestPoint(intersectedPoint, startPoint,
					a.intersectionPoint, c.intersectionPoint);
				return true;
			}
			else if (d.intersected)
			{
				GetClosestPoint(intersectedPoint, startPoint,
					a.intersectionPoint, d.intersectionPoint);
				return true;
			}
			intersectedPoint = a.intersectionPoint;
			return true;
		}
		return false;
	}

	static bool Rect2DIntersectsRay(const Rect2D& rect, const Ray2D& ray, Vector2& intersectedPoint, bool shouldCalculateIntersection)
	{
		Vector2 max = rect.GetMax();
		Vector2 min = rect.GetMin();

		LineSegment2D topEdge = { { min.x, max.y }, max };
		LineSegment2D bottomEdge = { min, { max.x, min.y} };
		LineSegment2D rightEdge = { max, { max.x, min.y } };
		LineSegment2D leftEdge = { {min.x, max.y}, min };

		SegmentIntersectionOutput topPoint, rightPoint, leftPoint, bottomPoint;
		topPoint.intersected = topEdge.Intersects(ray, topPoint.intersectionPoint);
		bottomPoint.intersected = bottomEdge.Intersects(ray, bottomPoint.intersectionPoint);
		rightPoint.intersected = rightEdge.Intersects(ray, rightPoint.intersectionPoint);
		leftPoint.intersected = leftEdge.Intersects(ray, leftPoint.intersectionPoint);

		if (!topPoint.intersected
			&& !bottomPoint.intersected
			&& !rightPoint.intersected
			&& !leftPoint.intersected)
		{
			return false;
		}

		if (!shouldCalculateIntersection)
		{
			return true;
		}

		if (Rect2DTestPoints(intersectedPoint, ray.startPoint,
			topPoint, bottomPoint, leftPoint, rightPoint))
		{
			return true;
		}
		if (Rect2DTestPoints(intersectedPoint, ray.startPoint,
			bottomPoint, leftPoint, rightPoint, topPoint))
		{
			return true;
		}
		if (Rect2DTestPoints(intersectedPoint, ray.startPoint,
			leftPoint, rightPoint, topPoint, bottomPoint))
		{
			return true;
		}
		if (Rect2DTestPoints(intersectedPoint, ray.startPoint,
			rightPoint, topPoint, bottomPoint, leftPoint))
		{
			return true;
		}
		return true;
	}

	static bool Rect2DIntersectsSegment(const Rect2D& rect, const LineSegment2D& segment, Vector2& intersectedPoint, bool shouldCalculateIntersection)
	{
		Vector2 max = rect.GetMax();
		Vector2 min = rect.GetMin();

		LineSegment2D topEdge = { { min.x, max.y }, max };
		LineSegment2D bottomEdge = { min, { max.x, min.y} };
		LineSegment2D rightEdge = { max, { max.x, min.y } };
		LineSegment2D leftEdge = { {min.x, max.y}, min };

		SegmentIntersectionOutput topPoint, rightPoint, leftPoint, bottomPoint;
		topPoint.intersected = topEdge.Intersects(segment, topPoint.intersectionPoint);
		bottomPoint.intersected = bottomEdge.Intersects(segment, bottomPoint.intersectionPoint);
		rightPoint.intersected = rightEdge.Intersects(segment, rightPoint.intersectionPoint);
		leftPoint.intersected = leftEdge.Intersects(segment, leftPoint.intersectionPoint);

		if (!topPoint.intersected
			&& !bottomPoint.intersected
			&& !rightPoint.intersected
			&& !leftPoint.intersected)
		{
			return false;
		}

		if (!shouldCalculateIntersection)
		{
			return true;
		}

		if (Rect2DTestPoints(intersectedPoint, segment.start,
			topPoint, bottomPoint, leftPoint, rightPoint))
		{
			return true;
		}
		if (Rect2DTestPoints(intersectedPoint, segment.start,
			bottomPoint, leftPoint, rightPoint, topPoint))
		{
			return true;
		}
		if (Rect2DTestPoints(intersectedPoint, segment.start,
			leftPoint, rightPoint, topPoint, bottomPoint))
		{
			return true;
		}
		if (Rect2DTestPoints(intersectedPoint, segment.start,
			rightPoint, topPoint, bottomPoint, leftPoint))
		{
			return true;
		}
		return true;
	}


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
		Vector2 point;
		return Rect2DIntersectsSegment(*this, lineSegment, point, false);
	}

	bool Rect2D::Intersects(const LineSegment2D& lineSegment, Vector2& point) const
	{		
		return Rect2DIntersectsSegment(*this, lineSegment, point, true);
	}

	bool Rect2D::Intersects(const Ray2D& ray) const
	{
		Vector2 point;
		return Rect2DIntersectsRay(*this, ray, point, false);
	}

	bool Rect2D::Intersects(const Ray2D& ray, Vector2& point) const
	{
		return Rect2DIntersectsRay(*this, ray, point, true);
	}

	bool Rect2D::Intersects(const Rect2D& rect) const
	{
		Rect2D intersectionRect;
		return Intersects(rect, intersectionRect);
	}

	bool Rect2D::Intersects(const Rect2D& rect, Rect2D& intersectionRect) const
	{
		Vector2 min = GetMin();
		Vector2 max = GetMax();
		Vector2 rectMin = rect.GetMin();
		Vector2 rectMax = rect.GetMax();
		if (min.x > rectMax.x
			|| min.y > rectMax.y
			|| rectMin.x > min.x
			|| rectMin.y > min.y)
		{
			return false;
		}
		intersectionRect = { Max(rectMin, min), Min(rectMax, max) };
		return true;
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
	
	// -------------------------------------- Circle 2D Implementation --------------------------------

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

	bool Circle2D::Intersects(const Circle2D& circle) const
	{
		Vector2 intersectionA, intersectionB;
		return Intersects(circle, intersectionA, intersectionB);
	}

	bool Circle2D::Intersects(const Circle2D& circle, Vector2& pointA, Vector2& pointB) const
	{
		Vector2 diffCenters = circle.center - center;
		float addedRadius = radius + circle.radius;
		float distanceSquared = diffCenters.LengthSquared();
		if (distanceSquared > addedRadius * addedRadius)
		{
			return false;
		}
		diffCenters.Normalize();
		float sProjected = Dot(diffCenters, Vector2::UnitX * radius);
		Vector2 centerOfIntersectionLine = sProjected * diffCenters;
		float intersectionLengthDiff = Sqrt(sProjected * sProjected - radius * radius);
		pointA = intersectionLengthDiff * Vector2{ diffCenters.x, -diffCenters.y };
		pointB = intersectionLengthDiff * Vector2{ -diffCenters.x, diffCenters.y };
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

	// --------------------- Triangle 2D Class ----------------------------------

	static bool Triangle2DTestPoints(Vector2& intersectedPoint, const Vector2& startPoint,
		const SegmentIntersectionOutput& a, const SegmentIntersectionOutput& b, const SegmentIntersectionOutput& c)
	{
		if (a.intersected)
		{
			if (b.intersected)
			{
				GetClosestPoint(intersectedPoint, startPoint,
					a.intersectionPoint, b.intersectionPoint);
				return true;
			}
			else if (c.intersected)
			{
				GetClosestPoint(intersectedPoint, startPoint,
					a.intersectionPoint, c.intersectionPoint);
				return true;
			}
			intersectedPoint = a.intersectionPoint;
			return true;
		}
		return false;
	}

	static bool Triangle2DIntersectsRay(const Triangle2D& triangle, const Ray2D& ray, Vector2& point, bool calculateIntersection)
	{
		LineSegment2D edge1 = { triangle.point1, triangle.point2 };
		LineSegment2D edge2 = { triangle.point2, triangle.point3 };
		LineSegment2D edge3 = { triangle.point3, triangle.point1 };

		// Intersection Tests.
		SegmentIntersectionOutput edge1Intersection, edge2Intersection, edge3Intersection;
		edge1Intersection.intersected = edge1.Intersects(ray, edge1Intersection.intersectionPoint);
		edge2Intersection.intersected = edge2.Intersects(ray, edge2Intersection.intersectionPoint);
		edge3Intersection.intersected = edge3.Intersects(ray, edge3Intersection.intersectionPoint);

		if (!edge1Intersection.intersected
			&& !edge2Intersection.intersected
			&& !edge3Intersection.intersected)
		{
			return false;
		}

		if (!calculateIntersection)
		{
			return true;
		}

		// Checks whether or not the 1st edge has an intersection.
		if (Triangle2DTestPoints(point, ray.startPoint,
			edge1Intersection, edge2Intersection, edge3Intersection))
		{
			return true;
		}
		// Checks whether or not the second edge has an intersection.
		if (Triangle2DTestPoints(point, ray.startPoint,
			edge2Intersection, edge3Intersection, edge1Intersection))
		{
			return true;
		}
		// Checks whether or not the third edge has an intersection.
		if (Triangle2DTestPoints(point, ray.startPoint,
			edge3Intersection, edge1Intersection, edge2Intersection))
		{
			return true;
		}
		return false;
	}

	static bool Triangle2DIntersectsSegment(const Triangle2D& triangle, const LineSegment2D& segment, Vector2& point, bool calculateIntersection)
	{
		LineSegment2D edge1 = { triangle.point1, triangle.point2 };
		LineSegment2D edge2 = { triangle.point2, triangle.point3 };
		LineSegment2D edge3 = { triangle.point3, triangle.point1 };

		// Intersection Tests.
		SegmentIntersectionOutput edge1Intersection, edge2Intersection, edge3Intersection;
		edge1Intersection.intersected = edge1.Intersects(segment, edge1Intersection.intersectionPoint);
		edge2Intersection.intersected = edge2.Intersects(segment, edge2Intersection.intersectionPoint);
		edge3Intersection.intersected = edge3.Intersects(segment, edge3Intersection.intersectionPoint);

		if (!edge1Intersection.intersected
			&& !edge2Intersection.intersected
			&& !edge3Intersection.intersected)
		{
			return false;
		}

		if (!calculateIntersection)
		{
			return true;
		}

		// Checks whether or not the 1st edge has an intersection.
		if (Triangle2DTestPoints(point, segment.start,
			edge1Intersection, edge2Intersection, edge3Intersection))
		{
			return true;
		}
		// Checks whether or not the second edge has an intersection.
		if (Triangle2DTestPoints(point, segment.start,
			edge2Intersection, edge3Intersection, edge1Intersection))
		{
			return true;
		}
		// Checks whether or not the third edge has an intersection.
		if (Triangle2DTestPoints(point, segment.start,
			edge3Intersection, edge1Intersection, edge2Intersection))
		{
			return true;
		}
		return false;
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

	bool Triangle2D::Intersects(const Ray2D& ray) const
	{
		Vector2 point;
		return Triangle2DIntersectsRay(*this, ray, point, false);
	}

	bool Triangle2D::Intersects(const Ray2D& ray, Vector2& point) const
	{
		return Triangle2DIntersectsRay(*this, ray, point, true);
	}

	bool Triangle2D::Intersects(const LineSegment2D& lineSegment) const
	{
		Vector2 point;
		return Triangle2DIntersectsSegment(*this, lineSegment, point, false);
	}

	bool Triangle2D::Intersects(const LineSegment2D& lineSegment, Vector2& point) const
	{
		return Triangle2DIntersectsSegment(*this, lineSegment, point, true);
	}

	bool IsPointWithin(const Triangle2D& triangle, const Vector2& point)
	{
		return triangle.IsPointWithin(point);
	}
}
