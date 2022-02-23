#include "MathPCH.h"
#include "Shape3D.h"
#include "Geometry3D.h"
#include "Math.h"

namespace MathLib
{

	// ----------------------------------- Rect 3D Implementation ----------------------------

	struct IntersectionData
	{
		Vector3 intersectedPoint;
		bool withinShape = false;
	};

	static bool GetClosestDistance(const Vector3& point, 
		IntersectionData& closest, const std::initializer_list<IntersectionData>& interesections)
	{
		bool foundClosestPoint = false;
		float closestLength = 0.0f;
		for (const auto& interesectedPoint : interesections)
		{
			if (interesectedPoint.withinShape)
			{
				float lengthSquared = (point - interesectedPoint.intersectedPoint).LengthSquared();
				if (!foundClosestPoint
					|| lengthSquared < closestLength)
				{
					foundClosestPoint = true;
					closestLength = lengthSquared;
					closest = interesectedPoint;
				}
			}
		}
		return foundClosestPoint;
	}

	// Calculates whether a line segment intersects a rect3D.
	static bool IntersectsRect3D(const Rect3D& rect3D, const LineSegment3D& segment, 
		Vector3& intersectedPoint, bool findIntersectedPoint)
	{
		Vector3 min = rect3D.GetMin();
		Vector3 max = rect3D.GetMax();

		Plane3D maxXPlane = { Vector3::UnitX, max.x };
		Plane3D maxYPlane = { Vector3::UnitY, max.y };
		Plane3D maxZPlane = { Vector3::UnitZ, max.z };

		Plane3D minXPlane = { -Vector3::UnitX, min.x };
		Plane3D minYPlane = { -Vector3::UnitY, min.y };
		Plane3D minZPlane = { -Vector3::UnitZ, min.z };

		IntersectionData maxXPlaneIntersectionPoint, maxYPlaneIntersectionPoint,
			maxZPlaneIntersectionPoint;
		IntersectionData minXPlaneIntersectionPoint, minYPlaneIntersectionPoint,
			minZPlaneIntersectionPoint;

		maxXPlaneIntersectionPoint.withinShape
			= maxXPlane.Intersects(segment, maxXPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(maxXPlaneIntersectionPoint.intersectedPoint);
		maxYPlaneIntersectionPoint.withinShape
			= maxYPlane.Intersects(segment, maxYPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(maxYPlaneIntersectionPoint.intersectedPoint);
		maxZPlaneIntersectionPoint.withinShape
			= maxZPlane.Intersects(segment, maxZPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(maxZPlaneIntersectionPoint.intersectedPoint);

		minXPlaneIntersectionPoint.withinShape
			= minXPlane.Intersects(segment, minXPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(minXPlaneIntersectionPoint.intersectedPoint);
		minYPlaneIntersectionPoint.withinShape
			= minYPlane.Intersects(segment, minYPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(minYPlaneIntersectionPoint.intersectedPoint);
		minZPlaneIntersectionPoint.withinShape
			= minZPlane.Intersects(segment, minZPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(minZPlaneIntersectionPoint.intersectedPoint);

		if (!findIntersectedPoint)
		{
			return maxXPlaneIntersectionPoint.withinShape
				|| maxYPlaneIntersectionPoint.withinShape
				|| maxZPlaneIntersectionPoint.withinShape
				|| minXPlaneIntersectionPoint.withinShape
				|| minYPlaneIntersectionPoint.withinShape
				|| minZPlaneIntersectionPoint.withinShape;
		}

		IntersectionData enteredPoint;
		bool foundClosestPoint = GetClosestDistance(segment.start, 
			enteredPoint, { maxXPlaneIntersectionPoint, maxYPlaneIntersectionPoint, maxZPlaneIntersectionPoint,
				minXPlaneIntersectionPoint, minYPlaneIntersectionPoint, minZPlaneIntersectionPoint});
		intersectedPoint = enteredPoint.intersectedPoint;
		return foundClosestPoint;
	}

	static bool IntersectsRect3D(const Rect3D& rect3D, const Ray3D& ray,
		Vector3& intersectedPoint, bool findIntersectedPoint)
	{
		Vector3 min = rect3D.GetMin();
		Vector3 max = rect3D.GetMax();

		Plane3D maxXPlane = { Vector3::UnitX, max.x };
		Plane3D maxYPlane = { Vector3::UnitY, max.y };
		Plane3D maxZPlane = { Vector3::UnitZ, max.z };

		Plane3D minXPlane = { -Vector3::UnitX, min.x };
		Plane3D minYPlane = { -Vector3::UnitY, min.y };
		Plane3D minZPlane = { -Vector3::UnitZ, min.z };

		IntersectionData maxXPlaneIntersectionPoint, maxYPlaneIntersectionPoint,
			maxZPlaneIntersectionPoint;
		IntersectionData minXPlaneIntersectionPoint, minYPlaneIntersectionPoint,
			minZPlaneIntersectionPoint;

		maxXPlaneIntersectionPoint.withinShape
			= maxXPlane.Intersects(ray, maxXPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(maxXPlaneIntersectionPoint.intersectedPoint);
		maxYPlaneIntersectionPoint.withinShape
			= maxYPlane.Intersects(ray, maxYPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(maxYPlaneIntersectionPoint.intersectedPoint);
		maxZPlaneIntersectionPoint.withinShape
			= maxZPlane.Intersects(ray, maxZPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(maxZPlaneIntersectionPoint.intersectedPoint);

		minXPlaneIntersectionPoint.withinShape
			= minXPlane.Intersects(ray, minXPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(minXPlaneIntersectionPoint.intersectedPoint);
		minYPlaneIntersectionPoint.withinShape
			= minYPlane.Intersects(ray, minYPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(minYPlaneIntersectionPoint.intersectedPoint);
		minZPlaneIntersectionPoint.withinShape
			= minZPlane.Intersects(ray, minZPlaneIntersectionPoint.intersectedPoint)
			&& rect3D.IsPointWithin(minZPlaneIntersectionPoint.intersectedPoint);

		if (!findIntersectedPoint)
		{
			return maxXPlaneIntersectionPoint.withinShape
				|| maxYPlaneIntersectionPoint.withinShape
				|| maxZPlaneIntersectionPoint.withinShape
				|| minXPlaneIntersectionPoint.withinShape
				|| minYPlaneIntersectionPoint.withinShape
				|| minZPlaneIntersectionPoint.withinShape;
		}

		IntersectionData enteredPoint;
		bool foundClosestPoint = GetClosestDistance(ray.startPoint,
			enteredPoint, { maxXPlaneIntersectionPoint, maxYPlaneIntersectionPoint, maxZPlaneIntersectionPoint,
				minXPlaneIntersectionPoint, minYPlaneIntersectionPoint, minZPlaneIntersectionPoint });
		intersectedPoint = enteredPoint.intersectedPoint;
		return foundClosestPoint;
	}

	Rect3D::Rect3D(const Vector3& min, const Vector3& max)
		: center(), size()
	{
		size = max - min;
		center = min + size * 0.5f;
	}
	
	Rect3D::Rect3D(const Vector3& center, float size)
		: center(center), size(size, size, size)
	{
	}
	
	Rect3D::Rect3D(const Vector3& center, float sizeX, float sizeY, float sizeZ)
		: center(center), size(sizeX, sizeY, sizeZ)
	{
	}

	Vector3 Rect3D::GetMax() const
	{
		return center + size * 0.5f;
	}

	Vector3 Rect3D::GetMin() const
	{
		return center - size * 0.5f;
	}
	
	bool Rect3D::IsPointWithin(const Vector3& point) const
	{
		Vector3 min = GetMin();
		Vector3 max = GetMax();

		return min.x < point.x
			&& min.y < point.y
			&& min.z < point.z
			&& max.x > point.x
			&& max.y > point.y
			&& max.z > point.z;
	}

	bool Rect3D::Intersects(const LineSegment3D& segment) const
	{
		Vector3 interesectedPoint;
		return IntersectsRect3D(*this, segment, interesectedPoint, false);
	}

	bool Rect3D::Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const
	{
		return IntersectsRect3D(*this, segment, intersectedPoint, true);
	}

	bool Rect3D::Intersects(const Ray3D& ray) const
	{
		Vector3 interesectedPoint;
		return IntersectsRect3D(*this, ray, interesectedPoint, false);
	}

	bool Rect3D::Intersects(const Ray3D& ray, Vector3& intersectedPoint) const
	{
		return IntersectsRect3D(*this, ray, intersectedPoint, true);
	}

	bool Intersects(const Rect3D& a, const LineSegment3D& segment)
	{
		return a.Intersects(segment);
	}

	bool Intersects(const Rect3D& a, const LineSegment3D& segment, Vector3& intersectedPoint)
	{
		return a.Intersects(segment, intersectedPoint);
	}

	bool Intersects(const Rect3D& a, const Ray3D& ray)
	{
		return a.Intersects(ray);
	}

	bool Intersects(const Rect3D& a, const Ray3D& ray, Vector3& intersectedPoint)
	{
		return a.Intersects(ray, intersectedPoint);
	}

	bool IsPointWithin(const Rect3D& box, const Vector3& point)
	{
		return box.IsPointWithin(point);
	}

	// ------------------------------------ Sphere 3D Implementation -------------------------

	Sphere3D::Sphere3D(const Vector3& center, float radius)
		: center(center), radius(radius) { }
	
	bool Sphere3D::IsPointWithin(const Vector3& point) const
	{
		float distanceSquared = (center - point).LengthSquared();
		return distanceSquared <= radius * radius;
	}

	bool Sphere3D::Intersects(const LineSegment3D& segment) const
	{
		Vector3 intersectedPoint;
		return Intersects(segment, intersectedPoint);
	}

	bool Sphere3D::Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const
	{
		Vector3 segmentDirection = segment.GetDirection();
		Vector3 differenceToPoint = segment.start - center;
		float scalarProjected = Dot(differenceToPoint, -segmentDirection);
		Vector3 closestPointToCenter = segment.start + scalarProjected * segmentDirection;
		differenceToPoint = closestPointToCenter - center;
		float distanceSquared = differenceToPoint.LengthSquared();
		float radiusSquared = radius * radius;
		if (distanceSquared > radiusSquared)
		{
			return false;
		}
		// Distance to y represents the y component in the pythagorean theorem
		float distanceToY = Sqrt(distanceSquared - radiusSquared);
		intersectedPoint = closestPointToCenter + -segmentDirection * distanceToY;
		return true;
	}

	bool Sphere3D::Intersects(const Ray3D& ray) const
	{
		Vector3 intersectedPoint;
		return Intersects(ray, intersectedPoint);
	}

	bool Sphere3D::Intersects(const Ray3D& ray, Vector3& intersectedPoint) const
	{
		Vector3 differenceToPoint = ray.startPoint - center;
		float scalarProjected = Dot(differenceToPoint, -ray.direction);
		Vector3 closestPointToCenter = ray.startPoint + ray.direction * scalarProjected;
		differenceToPoint = closestPointToCenter - center;
		float distanceSquared = differenceToPoint.LengthSquared();
		float radiusSquared = radius * radius;
		if (distanceSquared > radiusSquared)
		{
			return false;
		}
		// Distance to y represents the y component in the pythagorean theorem
		float distanceToY = Sqrt(distanceSquared - radiusSquared);
		intersectedPoint = closestPointToCenter + -ray.direction * distanceToY;
		return true;
	}

	bool IsPointWithin(const Sphere3D& sphere, const Vector3& point)
	{
		return sphere.IsPointWithin(point);
	}

	bool Intersects(const Sphere3D& sphere, const LineSegment3D& segment)
	{
		return sphere.Intersects(segment);
	}

	bool Intersects(const Sphere3D& sphere, const LineSegment3D& segment, Vector3& intersectedPoint)
	{
		return sphere.Intersects(segment, intersectedPoint);
	}

	bool Intersects(const Sphere3D& sphere, const Ray3D& ray, Vector3& intersectedPoint)
	{
		return sphere.Intersects(ray, intersectedPoint);
	}

	bool Intersects(const Sphere3D& sphere, const Ray3D& ray)
	{
		return sphere.Intersects(ray);
	}

	// ------------------------------------ Triangle 3D Implementation -------------------------

	Triangle3D::Triangle3D(const Vector3& p1, const Vector3& p2, const Vector3& p3)
		: point1(p1), point2(p2), point3(p3)
	{
	}

	const Vector3 Triangle3D::GetNormal() const
	{
		return Normalize(Cross(
			Normalize(point2 - point1), 
			Normalize(point3 - point1)));
	}

	const Vector3 Triangle3D::GetCenter() const
	{
		Vector3 total = point1 + point2 + point3;
		return Vector3(
			total.x / 3.0f, total.y / 3.0f, total.z / 3.0f);
	}

	void Triangle3D::SetCenter(const Vector3& centerPosition)
	{
		Vector3 centerDifference = centerPosition - GetCenter();
		point1 += centerDifference;
		point2 += centerDifference;
		point3 += centerDifference;
	}

	bool Triangle3D::Intersects(const LineSegment3D& segment) const
	{
		Vector3 point;
		return Intersects(segment, point);
	}

	bool Triangle3D::Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const
	{
		Plane3D plane = { GetNormal(), GetCenter() };
		if (plane.Intersects(segment, intersectedPoint))
		{
			return IsPointWithin(intersectedPoint);
		}
		return false;
	}

	bool Triangle3D::Intersects(const Ray3D& ray) const
	{
		Vector3 point;
		return Intersects(ray, point);
	}

	bool Triangle3D::Intersects(const Ray3D& ray, Vector3& intersectedPoint) const
	{
		Plane3D plane = { GetNormal(), GetCenter() };
		if (plane.Intersects(ray, intersectedPoint))
		{
			return IsPointWithin(intersectedPoint);
		}
		return false;
	}

	bool Triangle3D::IsPointWithin(const Vector3& point) const
	{
		Vector3 normal = GetNormal();

		Vector3 point1Cross = Cross(Normalize(point - point1), 
			Normalize(point2 - point1));
		Vector3 point2Cross = Cross(Normalize(point - point2),
			Normalize(point3 - point2));
		Vector3 point3Cross = Cross(Normalize(point - point3),
			Normalize(point1 - point3));

		float p1DotSign = Dot(point1Cross, normal);
		float p2DotSign = Dot(point2Cross, normal);
		float p3DotSign = Dot(point3Cross, normal);
		return p1DotSign >= 0.0f == p2DotSign >= 0.0f
			&& p2DotSign >= 0.0f == p3DotSign >= 0.0f;
	}

	bool IsPointWithin(const Triangle3D& triangle, const Vector3& point)
	{
		return triangle.IsPointWithin(point);
	}

	bool Intersects(const Triangle3D& triangle, const LineSegment3D& segment)
	{
		return triangle.Intersects(segment);
	}

	bool Intersects(const Triangle3D& triangle, const LineSegment3D& segment, Vector3& point)
	{
		return triangle.Intersects(segment, point);
	}

	bool Intersects(const Triangle3D& triangle, const Ray3D& ray)
	{
		return triangle.Intersects(ray);
	}

	bool Intersects(const Triangle3D& triangle, const Ray3D& ray, Vector3& point)
	{
		return triangle.Intersects(ray, point);
	}
	
	// ------------------------------------ Circle 3D Implementation -------------------------

	Circle3D::Circle3D(const Vector3& position, float radius, const Vector3& normal)
		: position(position), radius(radius), normal(normal)
	{
	}

	bool Circle3D::IsPointWithin(const Vector3& point) const
	{
		Plane3D plane = { normal, position };
		if (!plane.IsPointWithin(point))
		{
			return false;
		}
		float squareDistance = (point - position).LengthSquared();
		return squareDistance <= radius * radius;
	}

	bool Circle3D::Intersects(const LineSegment3D& segment) const
	{
		Vector3 point;
		return Intersects(segment, point);
	}

	bool Circle3D::Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const
	{
		Plane3D plane = { normal, position };
		if (plane.Intersects(segment, intersectedPoint))
		{
			float distanceSquared = (intersectedPoint - position).LengthSquared();
			return distanceSquared <= radius * radius;
		}
		return false;
	}

	bool Circle3D::Intersects(const Ray3D& ray) const
	{
		Vector3 point;
		return Intersects(ray, point);
	}

	bool Circle3D::Intersects(const Ray3D& ray, Vector3& intersectedPoint) const
	{
		Plane3D plane = { normal, position };
		if (plane.Intersects(ray, intersectedPoint))
		{
			float distanceSquared = (intersectedPoint - position).LengthSquared();
			return distanceSquared <= radius * radius;
		}
		return false;
	}

	bool IsPointWithin(const Circle3D& circle, const Vector3& point)
	{
		return circle.IsPointWithin(point);
	}

	bool Intersects(const Circle3D& circle, const Ray3D& ray)
	{
		return circle.Intersects(ray);
	}

	bool Intersects(const Circle3D& circle, const Ray3D& ray, Vector3& position)
	{
		return circle.Intersects(ray, position);
	}

	bool Intersects(const Circle3D& circle, const LineSegment3D& segment)
	{
		return circle.Intersects(segment);
	}

	bool Intersects(const Circle3D& circle, const LineSegment3D& segment, Vector3& position)
	{
		return circle.Intersects(segment, position);
	}
}