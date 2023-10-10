#include "MathPCH.h"
#include "Quaternion.h"
#include "Vector.h"
#include "MathLib.h"

namespace MathLib
{

	const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

	Quaternion::Quaternion()
	{
		*this = Identity;
	}

	Quaternion::Quaternion(float x, float y, float z, float w) 
		: x(x), y(y), z(z), w(w)
	{
	}

	Quaternion::Quaternion(const Vector3& axis, float angle, bool inDegrees)
	{
		x = axis.x * Sin(angle * 0.5f, inDegrees);
		y = axis.y * Sin(angle * 0.5f, inDegrees);
		z = axis.z * Sin(angle * 0.5f, inDegrees);
		w = Cos(angle * 0.5f, inDegrees);
	}

	float Quaternion::LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	float Quaternion::Length() const
	{
		return Sqrt(LengthSquared());
	}

	void Quaternion::Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	Vector3 Quaternion::ToEuler() const
	{
		return ToEuler(false);
	}

	Vector3 Quaternion::ToEuler(bool inDegrees) const
	{
		// Rotation around X
		float pitch = ATan2(
			2.0f * ((w * x) + (y * z)),
			1.0f - 2.0f * (x * x + y * y));
		
		// Rotation around Y
		float yaw = 2.0f * (w * y - z * x);
		if (Abs(yaw) >= 1.0f)
		{
			yaw = std::copysign(PI * 0.5f, yaw);
		}
		else
		{
			yaw = ASin(yaw, false);
		}

		// Rotation around Z
		float roll = ATan2(
			2.0f * (w * z + x * y), 
			1.0f - 2.0f * (y * y + z * z));
		float conversion = inDegrees ? RAD2DEG : 1.0f;
		return Vector3(pitch * conversion, yaw * conversion, roll * conversion);
	}

	void Quaternion::Conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	float Dot(const Quaternion& a, const Quaternion& b)
	{
        return Quaternion::Dot(a, b);
	}

    float Quaternion::Dot(const Quaternion &a, const Quaternion &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

	Vector3 ToEuler(const Quaternion& quaternion)
	{
        return Quaternion::ToEuler(quaternion);
	}

    Vector3 Quaternion::ToEuler(const Quaternion& quaternion)
    {
        return quaternion.ToEuler();
    }

	Vector3 ToEuler(const Quaternion& quat, bool inDegrees)
	{
        return Quaternion::ToEuler(quat, inDegrees);
	}

    Vector3 Quaternion::ToEuler(const Quaternion &quat, bool inDegrees)
    {
        return quat.ToEuler(inDegrees);
    }

	Quaternion Concatenate(const Quaternion& a, const Quaternion& b)
	{
		Vector3 vecQuatA(a.x, a.y, a.z);
		Vector3 vecQuatB(b.x, b.y, b.z);
		Vector3 val = a.w * vecQuatB + b.w * vecQuatA + Cross(vecQuatA, vecQuatB);
		return Quaternion(
			val.x, val.y, val.z, a.w * b.w - Dot(vecQuatA, vecQuatB));
	}

	Quaternion Lerp(const Quaternion& a, const Quaternion& b, float alpha)
	{
		float dotResult = Dot(a, b);
		float bias = dotResult >= 0.0f ? 1.0f : -1.0f;
		Quaternion outQuat(
			b.x * alpha + a.x * bias * (1.0f - alpha),
			b.y * alpha + a.y * bias * (1.0f - alpha),
			b.z * alpha + a.z * bias * (1.0f - alpha),
			b.w * alpha + a.w * bias * (1.0f - alpha));
		outQuat.Normalize();
		return outQuat;
	}

	Quaternion LerpClamped(const Quaternion& a, const Quaternion& b, float alpha)
	{
		return Lerp(a, b, Clamp(0.0f, 1.0f, alpha));
	}

	Quaternion Slerp(const Quaternion& a, const Quaternion& b, float alpha)
	{
		float halfCosTheta = Dot(a, b);
		if (abs(halfCosTheta) >= 1.0f)
		{
			return a;
		}
		Quaternion outB = b;
		if (halfCosTheta < 0.0f)
		{
			outB.x *= -1.0f;
			outB.y *= -1.0f;
			outB.z *= -1.0f;
			outB.w *= -1.0f;
			halfCosTheta *= -1.0f;
		}
		float halfTheta = ACos(halfCosTheta, false);
		float sinHalfTheta = Sqrt(1.0f - halfCosTheta * halfCosTheta);
		if (sinHalfTheta < EPSILON)
		{
			return Quaternion(
				(a.x * 0.5f + outB.x * 0.5f),
				(a.y * 0.5f + outB.y * 0.5f),
				(a.z * 0.5f + outB.z * 0.5f),
				(a.w * 0.5f + outB.w * 0.5f));
		}
		float ratioA = Sin((1.0f - alpha) * halfTheta, false) / sinHalfTheta;
		float ratioB = Sin(alpha * halfTheta, false) / sinHalfTheta;
		return Quaternion(
			a.x * ratioA + outB.x * ratioB,
			a.y * ratioA + outB.y * ratioB,
			a.z * ratioA + outB.z * ratioB,
			a.w * ratioA + outB.w * ratioB);
	}

	Quaternion Conjugate(const Quaternion& q)
	{
		return Quaternion(-q.x, -q.y, -q.z, q.w);
	}

	Quaternion Quaternion::FromEuler(const Vector3& eulers)
	{
		return FromEuler(eulers, true);
	}

	Quaternion Quaternion::FromEuler(const Vector3& eulers, bool inDegrees)
	{
		return FromEuler(eulers.y, eulers.x, eulers.z, inDegrees);
	}

	Quaternion Quaternion::FromEuler(float yaw, float pitch, float roll)
	{
		return FromEuler(yaw, pitch, roll, true);
	}

	Quaternion Quaternion::FromEuler(float yaw, float pitch, float roll, bool inDegrees)
	{
		return Quaternion(
			Sin(pitch * 0.5f, inDegrees) * Cos(yaw * 0.5f, inDegrees) * Cos(roll * 0.5f, inDegrees)
			- Cos(pitch * 0.5f, inDegrees) * Sin(yaw * 0.5f, inDegrees) * Sin(roll * 0.5f, inDegrees),

			Cos(pitch * 0.5f, inDegrees) * Sin(yaw * 0.5f, inDegrees) * Cos(roll * 0.5f, inDegrees)
			+ Sin(pitch * 0.5f, inDegrees) * Cos(yaw * 0.5f, inDegrees) * Sin(roll * 0.5f, inDegrees),
			
			Cos(pitch * 0.5f, inDegrees) * Cos(yaw * 0.5f, inDegrees) * Sin(roll * 0.5f, inDegrees)
			- Sin(pitch * 0.5f, inDegrees) * Sin(yaw * 0.5f, inDegrees) * Cos(roll * 0.5f, inDegrees),
			
			Cos(pitch * 0.5f, inDegrees) * Cos(yaw * 0.5f, inDegrees) * Cos(roll * 0.5f, inDegrees)
			+ Sin(pitch * 0.5f, inDegrees) * Sin(yaw * 0.5f, inDegrees) * Sin(roll * 0.5f, inDegrees));
	}

	Quaternion Quaternion::FromDirection(const Vector3& desiredDirection)
	{
		return FromDirection(desiredDirection, Vector3::UnitY, Vector3::UnitX);
	}

	Quaternion Quaternion::FromDirection(const Vector3& desiredDirection, const Vector3& upDirection, const Vector3& rightDirection)
	{
		Vector3 rotationAxis;
		float dotProduct = Vector3::Dot(upDirection, desiredDirection);
		if (MathLib::IsCloseEnough(dotProduct, 1.0f))
		{
			rotationAxis = rightDirection;
		}
		else
		{
			rotationAxis = Cross(desiredDirection, upDirection);
		}
		// rotation axis for a quaternion.
		rotationAxis.Normalize();
		float rotationAngle = ACos(dotProduct, false);
		return Quaternion(rotationAxis, rotationAngle, false);
	}

	Quaternion Normalize(const Quaternion& quat)
	{
        return Quaternion::Normalize(quat);
	}

    Quaternion Quaternion::Normalize(const Quaternion& quat)
    {
        float length = quat.Length();
        return Quaternion(
            quat.x / length,
            quat.y / length,
            quat.z / length,
            quat.w / length);
    }

	bool operator==(const Quaternion& a, const Quaternion& b)
	{
		return MathLib::IsCloseEnough(a.x, b.x)
			&& MathLib::IsCloseEnough(a.y, b.y)
			&& MathLib::IsCloseEnough(a.z, b.z)
			&& MathLib::IsCloseEnough(a.w, b.w);
	}

	bool operator!=(const Quaternion& a, const Quaternion& b)
	{
		return !MathLib::IsCloseEnough(a.x, b.x)
			|| !MathLib::IsCloseEnough(a.y, b.y)
			|| !MathLib::IsCloseEnough(a.z, b.z)
			|| !MathLib::IsCloseEnough(a.w, b.w);
	}

	Quaternion operator-(const Quaternion& a)
	{
		return Quaternion(-a.x, -a.y, -a.z, a.w);
	}
}
