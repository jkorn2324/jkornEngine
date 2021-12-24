#include "MathPCH.h"
#include "Quaternion.h"
#include "Vector.h"
#include "Math.h"

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
		Vector3 quatX(
			1.0f - 2.0f * y * y - 2.0f * z * z,
			2.0f * x * y + 2.0f * z * w,
			2.0f * x * z - 2.0f * y * w);
		quatX.Normalize();
		Vector3 quatY(
			2.0f * x * y - 2.0f * z * w,
			1.0f - 2.0f * x * x - 2.0f * z * z,
			2.0f * y * z + 2.0f * x * w);
		quatY.Normalize();
		Vector3 quatZ(
			2.0f * x * z + 2.0f * y * w,
			2.0f * y * z - 2.0f * x * w,
			1.0f - 2.0f * x * x - 2.0f * y * y);
		quatZ.Normalize();

		return Vector3(
			ACos(Dot(quatX, Vector3::UnitX), false),
			ACos(Dot(quatY, Vector3::UnitY), false),
			ACos(Dot(quatZ, Vector3::UnitZ), false));
	}

	void Quaternion::Conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	float Dot(const Quaternion& a, const Quaternion& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	Vector3 ToEuler(const Quaternion& quaternion)
	{
		return quaternion.ToEuler();
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
		return FromEuler(eulers.z, eulers.y, eulers.x, inDegrees);
	}

	Quaternion Quaternion::FromEuler(float yaw, float pitch, float roll)
	{
		return FromEuler(yaw, pitch, roll, true);
	}

	Quaternion Quaternion::FromEuler(float yaw, float pitch, float roll, bool inDegrees)
	{
		return Quaternion(
			Sin(roll * 0.5f, inDegrees) * Cos(pitch * 0.5f, inDegrees) * Cos(yaw * 0.5f, inDegrees) 
				- Cos(roll * 0.5f, inDegrees) * Sin(pitch * 0.5f, inDegrees) * Sin(yaw * 0.5f, inDegrees),
			Cos(roll * 0.5f, inDegrees) * Sin(pitch * 0.5f, inDegrees) * Cos(yaw * 0.5f, inDegrees)
				- Sin(roll * 0.5f, inDegrees) * Cos(pitch * 0.5f, inDegrees) * Sin(yaw * 0.5f, inDegrees),
			Cos(roll * 0.5f, inDegrees) * Cos(pitch * 0.5f, inDegrees) * Sin(yaw * 0.5f, inDegrees)
				- Sin(roll * 0.5f, inDegrees) * Sin(pitch * 0.5f, inDegrees) * Cos(yaw * 0.5f, inDegrees),
			Cos(roll * 0.5f, inDegrees) * Cos(pitch * 0.5f, inDegrees) * Cos(yaw * 0.5f, inDegrees)
				- Sin(roll * 0.5f, inDegrees) * Sin(pitch * 0.5f, inDegrees) * Sin(yaw * 0.5f, inDegrees));
	}

	Quaternion Normalize(const Quaternion& quat)
	{
		float length = quat.Length();
		return Quaternion(
			quat.x / length,
			quat.y / length,
			quat.z / length,
			quat.w / length);
	}
}