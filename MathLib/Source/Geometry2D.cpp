#include "MathPCH.h"
#include "Geometry2D.h"

#include "Math.h"

namespace MathLib
{

	// ------------------------- LineSegment2D Implementation --------------------------

	bool LineSegment2D::IsPointWithin(const Vector2& point) const
	{
		Vector2 min = Min(start, end);
		Vector2 max = Max(start, end);
		return point.x >= min.x
			&& point.x <= max.x
			&& point.y >= min.y
			&& point.y <= max.y;
	}

	bool LineSegment2D::IsPointOnSegment(const Vector2& point) const
	{
		if (!IsPointWithin(point))
		{
			return false;
		}
		Vector2 startToPoint = Normalize(point - start);
		Vector2 direction = GetDirection();
		float dotPoint = Dot(direction, startToPoint);
		return Abs(dotPoint) >= 1.0f;
	}
	
	bool LineSegment2D::Intersects(const LineSegment2D& segment) const
	{
		Vector2 intersectedPoint;
		return Intersects(segment, intersectedPoint);
	}

	bool LineSegment2D::Intersects(const LineSegment2D& segment, Vector2& point) const
	{
		Vector2 segmentA = end - start;
		Vector2 segmentB = segment.end - segment.start;

		// Handles colinear segments.
		float dotProduct = Dot(Normalize(segmentA), Normalize(segmentB));
		// They are parallel lines, determine if they are colinear.
		if (Abs(dotProduct) >= 1.0f)
		{
			return IsPointWithin(segment.start)
				|| IsPointWithin(segment.end)
				|| segment.IsPointWithin(start)
				|| segment.IsPointWithin(end);
		}

		float segmentAYStart = start.y;
		float segmentASlope = 0.0f;
		bool isSegmentAVerticalLine = IsCloseEnough(segmentA.x, 0.0f);
		if (!isSegmentAVerticalLine)
		{
			segmentASlope = segmentA.y / segmentA.x;
			segmentAYStart -= segmentASlope * start.x;
		}

		float segmentBYStart = segment.start.y;
		float segmentBSlope = 0.0f;
		bool isSegmentBVerticalLine = IsCloseEnough(segmentB.x, 0.0f);
		if (!isSegmentBVerticalLine)
		{
			segmentBSlope = segmentB.y / segmentB.x;
			segmentBYStart -= segmentBSlope * segment.start.x;
		}

		if (isSegmentAVerticalLine)
		{
			point = Vector2{ start.x, segmentBSlope * start.x + segmentBYStart };
		}
		else if (isSegmentBVerticalLine)
		{
			point = Vector2{ segment.start.x, segmentASlope * segment.start.x + segmentAYStart };
		}
		else
		{
			float numerator = segmentBYStart - segmentAYStart;
			float denominator = segmentASlope - segmentBSlope;
			float t = numerator / denominator;
			point = Vector2{ t, segmentASlope * t + segmentAYStart };
		}
		return IsPointWithin(point)
			&& segment.IsPointWithin(point);
	}

	bool LineSegment2D::Intersects(const Ray2D& ray) const
	{
		Vector2 intersectedPoint;
		return Intersects(ray, intersectedPoint);
	}

	bool LineSegment2D::Intersects(const Ray2D& ray, Vector2& point) const
	{
		Vector2 segmentA = end - start;
		// Handles colinear segments.
		float dotProduct = Dot(Normalize(segmentA), ray.direction);
		// They are parallel lines, determine if they are colinear.
		if (Abs(dotProduct) >= 1.0f)
		{
			return IsPointWithin(ray.startPoint)
				|| ray.IsPointWithin(start)
				|| ray.IsPointWithin(end);
		}

		float segmentAYStart = start.y;
		float segmentASlope = 0.0f;
		bool isSegmentAVerticalLine = IsCloseEnough(segmentA.x, 0.0f);
		if (!isSegmentAVerticalLine)
		{
			segmentASlope = segmentA.y / segmentA.x;
			segmentAYStart -= segmentASlope * start.x;
		}

		float segmentBYStart = ray.startPoint.y;
		float segmentBSlope = 0.0f;
		bool isSegmentBVerticalLine = IsCloseEnough(ray.direction.x, 0.0f);
		if (!isSegmentBVerticalLine)
		{
			segmentBSlope = ray.direction.y / ray.direction.x;
			segmentBYStart -= segmentBSlope * ray.startPoint.x;
		}

		if (isSegmentAVerticalLine)
		{
			point = Vector2{ start.x, segmentBSlope * start.x + segmentBYStart };
		}
		else if (isSegmentBVerticalLine)
		{
			point = Vector2{ ray.startPoint.x, segmentASlope * ray.startPoint.x + segmentAYStart };
		}
		else
		{
			float numerator = segmentBYStart - segmentAYStart;
			float denominator = segmentASlope - segmentBSlope;
			float t = numerator / denominator;
			point = Vector2{ t, segmentASlope * t + segmentAYStart };
		}
		return IsPointWithin(point)
			&& ray.IsPointWithin(point);
	}

	bool IsPointWithin(const LineSegment2D& a, const Vector2& point)
	{
		return a.IsPointWithin(point);
	}

	bool IsPointOnSegment(const LineSegment2D& a, const Vector2& point)
	{
		return a.IsPointOnSegment(point);
	}

	bool Intersects(const LineSegment2D& a, const LineSegment2D& b)
	{
		return a.Intersects(b);
	}

	bool Intersects(const LineSegment2D& a, const LineSegment2D& b, Vector2& intersectedPoint)
	{
		return a.Intersects(b, intersectedPoint);
	}

	bool Intersects(const LineSegment2D& a, const Ray2D& b)
	{
		Vector2 point;
		return a.Intersects(b, point);
	}

	bool Intersects(const LineSegment2D& a, const Ray2D& b, Vector2& point)
	{
		return a.Intersects(b, point);
	}

	// ------------------------- Ray2D Implementation --------------------------

	bool Ray2D::IsPointWithin(const Vector2& point) const
	{
		if (IsInfinite())
		{
			Vector2 differenceFromPoint = point - startPoint;
			if (differenceFromPoint == Vector2::Zero)
			{
				return true;
			}
			differenceFromPoint.Normalize();
			return Dot(differenceFromPoint, direction) > 0.0f;
		}
		Vector2 endPoint = startPoint + direction * distance;
		Vector2 min = Min(startPoint, endPoint);
		Vector2 max = Max(startPoint, endPoint);
		return point.x >= min.x
			&& point.x <= max.x
			&& point.y >= min.y
			&& point.y <= max.y;
	}

	bool Ray2D::IsPointOnRay(const Vector2& point) const
	{
		if (!IsPointWithin(point))
		{
			return false;
		}
		Vector2 difference = Normalize(point - startPoint);
		float dotWithDirection = Dot(difference, direction);
		return Abs(dotWithDirection) >= 1.0f;
	}

	bool Ray2D::Intersects(const Ray2D& ray) const
	{
		Vector2 point;
		return Intersects(ray, point);
	}

	bool Ray2D::Intersects(const Ray2D& ray, Vector2& point) const
	{
		// Handles colinear segments.
		float dotProduct = Dot(direction, ray.direction);
		// They are parallel lines, determine if they are colinear.
		if (Abs(dotProduct) >= 1.0f)
		{
			return IsPointWithin(ray.startPoint)
				|| ray.IsPointWithin(startPoint);
		}

		float segmentAYStart = startPoint.y;
		float segmentASlope = 0.0f;
		bool isSegmentAVerticalLine = IsCloseEnough(direction.x, 0.0f);
		if (!isSegmentAVerticalLine)
		{
			segmentASlope = direction.y / direction.x;
			segmentAYStart -= segmentASlope * startPoint.x;
		}

		float segmentBYStart = ray.startPoint.y;
		float segmentBSlope = 0.0f;
		bool isSegmentBVerticalLine = IsCloseEnough(ray.direction.x, 0.0f);
		if (!isSegmentBVerticalLine)
		{
			segmentBSlope = ray.direction.y / ray.direction.x;
			segmentBYStart -= segmentBSlope * ray.startPoint.x;
		}

		if (isSegmentAVerticalLine)
		{
			point = Vector2{ startPoint.x, segmentBSlope * startPoint.x + segmentBYStart };
		}
		else if (isSegmentBVerticalLine)
		{
			point = Vector2{ ray.startPoint.x, segmentASlope * ray.startPoint.x + segmentAYStart };
		}
		else
		{
			float numerator = segmentBYStart - segmentAYStart;
			float denominator = segmentASlope - segmentBSlope;
			float t = numerator / denominator;
			point = Vector2{ t, segmentASlope * t + segmentAYStart };
		}
		return IsPointWithin(point)
			&& ray.IsPointWithin(point);
	}

	bool Ray2D::Intersects(const LineSegment2D& segment) const
	{
		return segment.Intersects(*this);
	}

	bool Ray2D::Intersects(const LineSegment2D& segment, Vector2& point) const
	{
		return segment.Intersects(*this, point);
	}

	bool IsPointWithin(const Ray2D& a, const Vector2& point)
	{
		return a.IsPointWithin(point);
	}

	bool IsPointOnRay(const Ray2D& a, const Vector2& point)
	{
		return a.IsPointOnRay(point);
	}

	bool Intersects(const Ray2D& a, const Ray2D& b)
	{
		return a.Intersects(b);
	}

	bool Intersects(const Ray2D& a, const Ray2D& b, Vector2& intersectedPoint)
	{
		return a.Intersects(b, intersectedPoint);
	}

	bool Intersects(const Ray2D& a, const LineSegment2D& b)
	{
		return a.Intersects(b);
	}

	bool Intersects(const Ray2D& a, const LineSegment2D& b, Vector2& intersectedPoint)
	{
		return a.Intersects(b, intersectedPoint);
	}
}