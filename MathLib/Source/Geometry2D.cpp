#include "MathPCH.h"
#include "Geometry2D.h"

#include "Math.h"

namespace MathLib
{

	static bool IsWithinSegment(const LineSegment2D& segment, const Vector2& point)
	{
		Vector2 min = Min(segment.start, segment.end);
		Vector2 max = Max(segment.start, segment.end);
		return point.x >= min.x
			&& point.x <= max.x
			&& point.y >= min.y
			&& point.y <= max.y;
	}

	static bool IsWithinRay(const Ray2D& ray, const Vector2& point)
	{
		if (ray.IsInfinite())
		{
			// TODO: Implementation
			return false;
		}

		Vector2 endPoint = ray.startPoint + ray.direction * ray.distance;
		Vector2 min = Min(ray.startPoint, endPoint);
		Vector2 max = Max(ray.startPoint, endPoint);
		return point.x >= min.x
			&& point.x <= max.x
			&& point.y >= min.y
			&& point.y <= max.y;
	}

	bool Intersects(const LineSegment2D& a, const LineSegment2D& b)
	{
		Vector2 intersectedPoint;
		return Intersects(a, b, intersectedPoint);
	}
	
	bool Intersects(const LineSegment2D& a, const LineSegment2D& b, Vector2& intersectedPoint)
	{
		Vector2 segmentA = a.end - a.start;
		Vector2 segmentB = b.end - b.start;

		// Handles colinear segments.
		float dotProduct = Dot(Normalize(segmentA), Normalize(segmentB));
		// They are parallel lines, determine if they are colinear.
		if (Abs(dotProduct) >= 1.0f)
		{
			// TODO: Add colinear checks.
#if 0
			float check1 = Cross(b.end - a.start, segmentA);
			float check2 = Cross(b.start - a.start, segmentA);
			float check3 = Cross(a.end - b.start, segmentB);
			float check4 = Cross(a.start - b.start, segmentB);
#endif
			return false;
		}

		float segmentAYStart = a.start.y;
		float segmentASlope = 0.0f;
		bool isSegmentAVerticalLine = IsCloseEnough(segmentA.x, 0.0f);
		if (!isSegmentAVerticalLine)
		{
			segmentASlope = segmentA.y / segmentA.x;
			segmentAYStart -= segmentASlope * a.start.x;
		}

		float segmentBYStart = b.start.y;
		float segmentBSlope = 0.0f;
		bool isSegmentBVerticalLine = IsCloseEnough(segmentB.x, 0.0f);
		if (!isSegmentBVerticalLine)
		{
			segmentBSlope = segmentB.y / segmentB.x;
			segmentBYStart -= segmentBSlope * a.start.x;
		}

		if (isSegmentAVerticalLine)
		{
			intersectedPoint = Vector2{ a.start.x, segmentBSlope * a.start.x + segmentBYStart };
		}
		else if (isSegmentBVerticalLine)
		{
			intersectedPoint = Vector2{ b.start.x, segmentASlope * b.start.x + segmentAYStart };
		}
		else
		{
			float numerator = segmentBYStart - segmentAYStart;
			float denominator = segmentASlope - segmentBSlope;
			float t = numerator / denominator;
			intersectedPoint = Vector2{ t, segmentASlope * t + segmentAYStart };
		}
		return IsWithinSegment(a, intersectedPoint)
			&& IsWithinSegment(b, intersectedPoint);
	}
	
	bool Intersects(const Ray2D& a, const Ray2D& b)
	{
		Vector2 intersectedPoint;
		return Intersects(a, b, intersectedPoint);
	}
	
	bool Intersects(const Ray2D& a, const Ray2D& b, Vector2& intersectedPoint)
	{
		// Handles colinear segments.
		float dotProduct = Dot(a.direction, b.direction);
		if (Abs(dotProduct) >= 1.0f)
		{
			// TODO: They are colinear, determine if segment a is on segment b or vice versa.
			return false;
		}
		// TODO: Implementation
		return false;
	}
}