#pragma once

namespace MathLib
{

	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		explicit Quaternion();
		explicit Quaternion(const class Vector3& axis, float angle, bool inDegrees = true);
		explicit Quaternion(float x, float y, float z, float w);

		float Length() const;
		float LengthSquared() const;
		void Normalize();
		Vector3 ToEuler() const;
		Vector3 ToEuler(bool inDegrees) const;
		void Conjugate();

		friend Quaternion Normalize(const Quaternion& quat);
		friend float Dot(const Quaternion& a, const Quaternion& b);
		friend Vector3 ToEuler(const Quaternion& quat);
		friend Vector3 ToEuler(const Quaternion& quat, bool inDegrees);
		
		friend Quaternion Concatenate(const Quaternion& a, const Quaternion& b);
		friend Quaternion Lerp(const Quaternion& a, const Quaternion& b, float alpha);
		friend Quaternion LerpClamped(const Quaternion& a, const Quaternion& b, float alpha);
		friend Quaternion Slerp(const Quaternion& a, const Quaternion& b, float alpha);
		friend Quaternion Conjugate(const Quaternion& q);
		
		friend bool operator==(const Quaternion& a, const Quaternion& b);
		friend bool operator!=(const Quaternion& a, const Quaternion& b);

		static Quaternion FromEuler(const Vector3& eulers);
		static Quaternion FromEuler(const Vector3& eulers, bool inDegrees);
		static Quaternion FromEuler(float roll, float pitch, float yaw);
		static Quaternion FromEuler(float roll, float pitch, float yaw, bool inDegrees);


		static const Quaternion Identity;
	};
}