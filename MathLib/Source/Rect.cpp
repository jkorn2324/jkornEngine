#include "MathPCH.h"
#include "Rect.h"

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

	Vector2 Rect2D::GetMin() const
	{
		return center - size * 0.5f;
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

	bool IsPointWithin(const Rect3D& box, const Vector3& point)
	{
		return box.IsPointWithin(point);
	}
}