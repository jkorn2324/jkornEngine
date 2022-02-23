#pragma once

#include "Vector.h"

namespace MathLib
{
	struct Plane3D;
	struct LineSegment3D;
	struct Ray3D;

	struct Rect3D
	{
		Vector3 center = Vector3::Zero;
		Vector3 size = Vector3::One;

		explicit Rect3D() = default;
		explicit Rect3D(const Vector3& min, const Vector3& max);
		explicit Rect3D(const Vector3& center, float size);
		explicit Rect3D(const Vector3& center, float sizeX, float sizeY, float sizeZ);

		Vector3 GetMin() const;
		Vector3 GetMax() const;

		bool IsPointWithin(const Vector3& point) const;

		bool Intersects(const LineSegment3D& segment) const;
		bool Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const;
		bool Intersects(const Ray3D& ray) const;
		bool Intersects(const Ray3D& ray, Vector3& intersectedPoint) const;

		friend bool IsPointWithin(const Rect3D& box, const Vector3& point);

		friend bool Intersects(const Rect3D& a, const LineSegment3D& segment);
		friend bool Intersects(const Rect3D& a, const LineSegment3D&, Vector3& intersectedPoint);
		friend bool Intersects(const Rect3D& a, const Ray3D& ray);
		friend bool Intersects(const Rect3D& a, const Ray3D& ray, Vector3& intersectedPoint);
	};

	// Circle defined in 3D Space.
	struct Circle3D
	{
		Vector3 position;
		float radius = 1.0f;
		Vector3 normal;

		explicit Circle3D() = default;
		explicit Circle3D(const Vector3& position, float radius, const Vector3& normal);

		bool IsPointWithin(const Vector3& point) const;

		bool Intersects(const LineSegment3D& segment) const;
		bool Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const;
		bool Intersects(const Ray3D& ray) const;
		bool Intersects(const Ray3D& ray, Vector3& intersectedPoint) const;

		friend bool IsPointWithin(const Circle3D& circle, const Vector3& point);

		friend bool Intersects(const Circle3D& circle, const LineSegment3D& segment);
		friend bool Intersects(const Circle3D& circle, const LineSegment3D& segment, Vector3& point);
		friend bool Intersects(const Circle3D& circle, const Ray3D& ray);
		friend bool Intersects(const Circle3D& circle, const Ray3D& ray, Vector3& point);
	};

	struct Sphere3D
	{
		Vector3 center = Vector3::Zero;
		float radius = 1.0f;
		
		explicit Sphere3D() = default;
		explicit Sphere3D(const Vector3& center, float radius);

		bool IsPointWithin(const Vector3& point) const;
		
		bool Intersects(const LineSegment3D& segment) const;
		bool Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const;
		bool Intersects(const Ray3D& ray) const;
		bool Intersects(const Ray3D& ray, Vector3& intersectedPoint) const;

		friend bool IsPointWithin(const Sphere3D& sphere, const Vector3& point);

		friend bool Intersects(const Sphere3D& sphere, const LineSegment3D& segment);
		friend bool Intersects(const Sphere3D& sphere, const LineSegment3D& segment, Vector3& intersectedPoint);
		friend bool Intersects(const Sphere3D& sphere, const Ray3D& ray, Vector3& intersectedPoint);
		friend bool Intersects(const Sphere3D& sphere, const Ray3D& ray);
	};

	struct Triangle3D
	{
		Vector3 point1 = Vector3::Zero;
		Vector3 point2 = Vector3::Zero;
		Vector3 point3 = Vector3::Zero;

		explicit Triangle3D() = default;
		explicit Triangle3D(const Vector3& p1, const Vector3& p2, const Vector3& p3);

		const Vector3 GetNormal() const;
		const Vector3 GetCenter() const;
		void SetCenter(const Vector3& centerPosition);

		bool IsPointWithin(const Vector3& point) const;

		bool Intersects(const LineSegment3D& segment) const;
		bool Intersects(const LineSegment3D& segment, Vector3& intersectedPoint) const;
		bool Intersects(const Ray3D& ray) const;
		bool Intersects(const Ray3D& ray, Vector3& intersectedPoint) const;

		friend bool IsPointWithin(const Triangle3D& triangle, const Vector3& point);

		friend bool Intersects(const Triangle3D& sphere, const LineSegment3D& segment);
		friend bool Intersects(const Triangle3D& sphere, const LineSegment3D& segment, Vector3& intersectedPoint);
		friend bool Intersects(const Triangle3D& sphere, const Ray3D& ray, Vector3& intersectedPoint);
		friend bool Intersects(const Triangle3D& sphere, const Ray3D& ray);
	};
}