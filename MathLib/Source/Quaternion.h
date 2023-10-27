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

        static Quaternion Normalize(const Quaternion& quat);
		friend Quaternion Normalize(const Quaternion& quat);
        
        static float Dot(const Quaternion& a, const Quaternion& b);
		friend float Dot(const Quaternion& a, const Quaternion& b);
        
        static Vector3 ToEuler(const Quaternion& quat);
		friend Vector3 ToEuler(const Quaternion& quat);
        static Vector3 ToEuler(const Quaternion& quat, bool inDegrees);
		friend Vector3 ToEuler(const Quaternion& quat, bool inDegrees);
		
		friend Quaternion Concatenate(const Quaternion& a, const Quaternion& b);
		friend Quaternion Lerp(const Quaternion& a, const Quaternion& b, float alpha);
		friend Quaternion LerpClamped(const Quaternion& a, const Quaternion& b, float alpha);
		friend Quaternion Slerp(const Quaternion& a, const Quaternion& b, float alpha);
		friend Quaternion Conjugate(const Quaternion& q);
		
		friend bool operator==(const Quaternion& a, const Quaternion& b);
		friend bool operator!=(const Quaternion& a, const Quaternion& b);

		friend Quaternion operator-(const Quaternion& a);

		// X = Pitch, Y = Yaw, Z = Roll
		static Quaternion FromEuler(const Vector3& eulers);
		static Quaternion FromEuler(const Vector3& eulers, bool inDegrees);
		static Quaternion FromEuler(float yaw, float pitch, float roll);
		static Quaternion FromEuler(float yaw, float pitch, float roll, bool inDegrees);

		/**
		 * The quaternion rotation from a direction.
		 */
		static Quaternion FromDirection(const Vector3& desiredDirection);
		static Quaternion FromDirection(const Vector3& desiredDirection, const Vector3& upDirection, const Vector3& rightDirection);

		static const Quaternion Identity;
	};
}
