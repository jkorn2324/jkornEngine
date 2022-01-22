#include "MathPCH.h"
#include "Matrix.h"
#include "Math.h"

namespace MathLib
{

#pragma region matrix3x3


	static float identityMat3x3[3][3] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	const Matrix3x3 Matrix3x3::Identity = Matrix3x3(identityMat3x3);

	Matrix3x3 Matrix3x3::CreateTranslation(const Vector2& vec)
	{
		return CreateTranslation(vec.x, vec.y);
	}

	Matrix3x3 Matrix3x3::CreateTranslation(float x, float y)
	{
		float mat[3][3] = {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ x, y, 1.0f }
		};
		return Matrix3x3(mat);
	}

	Matrix3x3 Matrix3x3::CreateRotation(float rotation, bool inDegrees)
	{
		float mat[3][3] = {
			{ Cos(rotation, inDegrees), Sin(rotation, inDegrees), 0.0f},
			{ Sin(rotation, inDegrees), Cos(rotation, inDegrees), 0.0f},
			{0.0f, 0.0f, 1.0f}
		};
		return Matrix3x3(mat);
	}


	Matrix3x3 Matrix3x3::CreateScale(const Vector2& vec)
	{
		return CreateScale(vec.x, vec.y);
	}

	Matrix3x3 Matrix3x3::CreateScale(float x, float y)
	{
		float mat[3][3] = {
			{x, 0.0f, 0.0f},
			{0.0f, y, 0.0f},
			{0.0f, 0.0f, 1.0f}
		};
		return Matrix3x3(mat);
	}

	Matrix3x3::Matrix3x3(const float mat[3][3])
	{
		std::memcpy(matrix, mat,
			sizeof(float) * 3 * 3);
	}

	Matrix3x3::Matrix3x3(const Matrix3x3& mat)
	{
		std::memcpy(matrix, mat.matrix,
			sizeof(float) * 3 * 3);
	}

	Vector2 Matrix3x3::GetScale() const
	{
		Vector2 scale;
		scale.x = Vector2(matrix[0][0], matrix[0][1]).Length();
		scale.y = Vector2(matrix[1][0], matrix[1][1]).Length();
		return scale;
	}

	Vector2 Matrix3x3::GetTranslation() const
	{
		return Vector2(
			matrix[2][0], matrix[2][1]);
	}

	Vector2 Matrix3x3::GetXAxis() const
	{
		return Normalize(
			Vector2(matrix[0][0], matrix[0][1]));
	}

	Vector2 Matrix3x3::GetYAxis() const
	{
		return Normalize(
			Vector2(matrix[1][0], matrix[1][1]));
	}

	void Matrix3x3::Transpose()
	{
		const std::uint8_t ROWS = 3, COLS = 3;
	
		float temporaryMat[ROWS][COLS];
		for (int r = 0; r < ROWS; r++)
		{
			for (int c = 0; c < COLS; c++)
			{
				temporaryMat[r][c] = matrix[c][r];
			}
		}
		std::memcpy(matrix, temporaryMat, sizeof(float) * ROWS * COLS);
	}

	void Matrix3x3::Invert()
	{
		Vector2 scale = GetScale();

		// Creates the inverted scale matrix.
		Matrix3x3 invertedScaleMat = Matrix3x3::Identity;
		invertedScaleMat[0][0] = 1 / (scale.x * scale.x);
		invertedScaleMat[1][1] = 1 / (scale.y * scale.y);

		// Creates the transposed rotation matrix w/ scale.
		Matrix3x3 transposedSelf = *this;
		transposedSelf[2][0] = 0.0f;
		transposedSelf[2][1] = 0.0f;
		transposedSelf[2][2] = 1.0f;
		transposedSelf.Transpose();

		// Creates the inverted translation matrix.
		Matrix3x3 invertedTranslationMatrix = Identity;
		invertedTranslationMatrix[2][0] = -matrix[2][0];
		invertedTranslationMatrix[2][1] = -matrix[2][1];
		invertedTranslationMatrix[2][2] = 1.0f;

		// Sets this to the inverted translation matrix * the inverted scale rotation matrix.
		*this = invertedTranslationMatrix * (transposedSelf * invertedScaleMat);
	}

	float* Matrix3x3::operator[](int index) const
	{
		return const_cast<float*>(matrix[index]);
	}

	Matrix3x3 operator*(const Matrix3x3& a, const Matrix3x3& b)
	{
		const std::uint32_t ROWS = 3, COLS = 3;

		Matrix3x3 output{};
		std::uint32_t num = ROWS * COLS;
		for (std::uint32_t r = 0; r < ROWS; r++)
		{
			float total = 0.0f;
			for (std::uint32_t c = 0; c < num; c++)
			{
				total += a[r][c % COLS] * b[c % COLS][c / COLS];

				if ((c + 1) % COLS == 0
					&& c > 0)
				{
					output[r][c / COLS] = total;
					total = 0.0f;
				}
			}
		}
		return output;
	}

	Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& mat)
	{
		const std::uint8_t ROWS = 3, COLS = 3;

		std::uint8_t num = ROWS * COLS;
		float oldMatrixData[ROWS][COLS];
		std::memcpy(oldMatrixData, matrix,
			num * sizeof(float));
		for (std::uint8_t r = 0; r < ROWS; r++)
		{
			float total = 0.0f;
			for (std::uint8_t c = 0; c < num; c++)
			{
				total += oldMatrixData[r][c % COLS] * mat[c % COLS][c / COLS];
				if (c % COLS == 0
					&& c > 0)
				{
					matrix[r / ROWS][c / COLS] = total;
					total = 0.0f;
				}
			}
		}
		return *this;
	}

	bool operator==(const Matrix3x3& a, const Matrix3x3& b)
	{
		const std::uint8_t ROW = 3, COL = 3;
		
		for (std::uint8_t r = 0; r < ROW; r++)
		{
			for (std::uint8_t c = 0; c < COL; c++)
			{
				if (!IsCloseEnough(a[r][c], b[r][c]))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool operator!=(const Matrix3x3& a, const Matrix3x3& b)
	{
		const std::uint8_t ROW = 3, COL = 3;
		
		for (std::uint8_t r = 0; r < ROW; r++)
		{
			for (std::uint8_t c = 0; c < COL; c++)
			{
				if (!IsCloseEnough(a[r][c], b[r][c]))
				{
					return true;
				}
			}
		}
		return false;
	}

	Matrix3x3 Transpose(const Matrix3x3& mat)
	{
		Matrix3x3 cpy(mat);
		cpy.Transpose();
		return cpy;
	}

	Matrix3x3 Invert(const Matrix3x3& inverted)
	{
		Matrix3x3 cpy(inverted);
		cpy.Invert();
		return cpy;
	}

#pragma endregion


#pragma region matrix4x4

	static float identityMat4x4[4][4] = 
	{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	const Matrix4x4 Matrix4x4::Identity = Matrix4x4(identityMat4x4);


	Matrix4x4 Matrix4x4::CreateEuler(const Vector3& eulers, bool inDegrees)
	{
		return CreateEuler(eulers.y, eulers.x, eulers.z, inDegrees);
	}

	Matrix4x4 Matrix4x4::CreateEuler(float yaw, float pitch, float roll, bool inDegrees)
	{
		return CreateRotationZ(roll, inDegrees) * 
			CreateRotationX(pitch, inDegrees) * CreateRotationY(yaw, inDegrees);
	}

	Matrix4x4 Matrix4x4::CreateFromQuaternion(const Quaternion& quat)
	{
		float matrix[4][4];
		matrix[0][0] = 1.0f - 2.0f * quat.y * quat.y - 2.0f * quat.z * quat.z;
		matrix[0][1] = 2.0f * quat.x * quat.y + 2.0f * quat.z * quat.w;
		matrix[0][2] = 2.0f * quat.x * quat.z - 2.0f * quat.y * quat.w;
		matrix[0][3] = 0.0f;

		matrix[1][0] = 2.0f * quat.x * quat.y - 2.0f * quat.z * quat.w;
		matrix[1][1] = 1.0f - 2.0f * quat.x * quat.x - 2.0f * quat.z * quat.z;
		matrix[1][2] = 2.0f * quat.y * quat.z + 2.0f * quat.x * quat.w;
		matrix[1][3] = 0.0f;

		matrix[2][0] = 2.0f * quat.x * quat.z + 2.0f * quat.y * quat.w;
		matrix[2][1] = 2.0f * quat.y * quat.z - 2.0f * quat.x * quat.w;
		matrix[2][2] = 1.0f - 2.0f * quat.x * quat.x - 2.0f * quat.y * quat.y;
		matrix[2][3] = 0.0f;

		matrix[3][0] = 0.0f;
		matrix[3][1] = 0.0f;
		matrix[3][2] = 0.0f;
		matrix[3][3] = 1.0f;
		return Matrix4x4(matrix);
	}

	Matrix4x4 Matrix4x4::CreateRotationX(float rotation, bool inDegrees)
	{
		float matrix[4][4] = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, Cos(rotation, inDegrees), Sin(rotation, inDegrees), 0.0f},
			{0.0f, -Sin(rotation, inDegrees), Cos(rotation, inDegrees), 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
		return Matrix4x4(matrix);
	}

	Matrix4x4 Matrix4x4::CreateRotationY(float rotation, bool inDegrees)
	{
		float matrix[4][4] = {
			{Cos(rotation, inDegrees), 0.0f, -Sin(rotation, inDegrees), 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{Sin(rotation, inDegrees), 0.0f, Cos(rotation, inDegrees), 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
		return Matrix4x4(matrix);
	}

	Matrix4x4 Matrix4x4::CreateRotationZ(float rotation, bool inDegrees)
	{
		float matrix[4][4] = {
			{Cos(rotation, inDegrees), Sin(rotation, inDegrees), 0.0f, 0.0f},
			{-Sin(rotation, inDegrees), Cos(rotation, inDegrees), 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
		return Matrix4x4(matrix);
	}

	Matrix4x4 Matrix4x4::CreateTranslation(const Vector3& vec)
	{
		return CreateTranslation(vec.x, vec.y, vec.z);
	}

	Matrix4x4 Matrix4x4::CreateTranslation(float x, float y, float z)
	{
		float matrix[4][4] = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{x, y, z, 1.0f}
		};
		return Matrix4x4(matrix);
	}

	Matrix4x4 Matrix4x4::CreateScale(const Vector3& vec)
	{
		return CreateScale(vec.x, vec.y, vec.z);
	}

	Matrix4x4 Matrix4x4::CreateScale(float x, float y, float z)
	{
		float matrix[4][4] = 
		{
			{x, 0.0f, 0.0f, 0.0f},
			{0.0f, y, 0.0f, 0.0f},
			{0.0f, 0.0f, z, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
		return Matrix4x4(matrix);
	}

	Matrix4x4 Matrix4x4::CreateOrtho(float left, float right, float top, float bottom,
		float nearPlane, float farPlane)
	{
		float orthoProj[4][4] =
		{
			{ 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / (top - bottom), 0.0f, 0.0f },
			{ 0.0f, 0.0f, -2.0f / (farPlane - nearPlane), 0.0f },
			{ -((right + left) / (right - left)), -((top + bottom) / (top - bottom)), -((farPlane + nearPlane) / (farPlane - nearPlane)), 1.0f }
		};
		return Matrix4x4(orthoProj);
	}

	Matrix4x4 Matrix4x4::CreateOrtho(float width, float height, float nearPlane, float farPlane)
	{
		return CreateOrtho(-width * 0.5f, width * 0.5f, height * 0.5f, -height * 0.5f, nearPlane, farPlane);
	}

	Matrix4x4 Matrix4x4::CreatePersp(float fov, float width, float height, float nearPlane, float farPlane, bool fovInRadians)
	{
		return CreatePersp(fov, height / width, nearPlane, farPlane);
	}

	Matrix4x4 Matrix4x4::CreatePersp(float fov, float aspectRatio, float nearPlane, float farPlane, bool fovInRadians)
	{
		fov *= fovInRadians ? 1.0f : DEG2RAD;
		float yScale = 1.0f / Tan(fov * 0.5f, false);
		float xScale = yScale * aspectRatio;
		float mat[4][4] =
		{
			{xScale, 0.0f, 0.0f, 0.0f},
			{0.0f, yScale, 0.0f, 0.0f},
			{0.0f, 0.0f, -(farPlane / (farPlane - nearPlane)), 1.0f},
			{0.0f, 0.0f, -((nearPlane * farPlane) / (farPlane - nearPlane)), 0.0f}
		};
		return Matrix4x4(mat);
	}

	Matrix4x4 Matrix4x4::CreateLookAt(const Vector3& lookAtPosition, const Vector3& eyePos, const Vector3& upDir)
	{
		Vector3 forward = Normalize((lookAtPosition - eyePos));
		Vector3 right = Normalize(Cross(Normalize(upDir), forward));
		Vector3 newUp = Normalize(Cross(forward, right));

		float mat[4][4] =
		{
			{ right.x, right.y, right.z, 0.0f },
			{ newUp.x, newUp.y, newUp.z, 0.0f },
			{ forward.x, forward.y, forward.z, 0.0f},
			{ eyePos.x, eyePos.y, eyePos.z, 1.0f }
		};
		return Matrix4x4(mat);
	}

	Matrix4x4::Matrix4x4(const float mat[4][4])
	{
		std::memcpy(matrix, mat, sizeof(float) * 16);
	}

	Matrix4x4::Matrix4x4(const Matrix4x4& mat)
	{
		std::memcpy(matrix, mat.matrix, sizeof(float) * 16);
	}

	Vector3 Matrix4x4::GetScale() const
	{
		return Vector3(
			Vector3(matrix[0][0], matrix[0][1], matrix[0][2]).Length(),
			Vector3(matrix[1][0], matrix[1][1], matrix[1][2]).Length(),
			Vector3(matrix[2][0], matrix[2][1], matrix[2][2]).Length());
	}

	Vector3 Matrix4x4::GetTranslation() const
	{
		return Vector3(matrix[3][0], matrix[3][1], matrix[3][2]);
	}

	Vector3 Matrix4x4::GetXAxis() const
	{
		return Normalize(
			Vector3(matrix[0][0], matrix[0][1], matrix[0][2]));
	}

	Vector3 Matrix4x4::GetYAxis() const
	{
		return Normalize(
			Vector3(matrix[1][0], matrix[1][1], matrix[1][2]));
	}

	Vector3 Matrix4x4::GetZAxis() const
	{
		return Normalize(
			Vector3(matrix[2][0], matrix[2][1], matrix[2][2]));
	}

	void Matrix4x4::Invert()
	{
		Vector3 scale = GetScale();

		// Creates the inverted scale matrix.
		Matrix4x4 invertedScaleMat = Matrix4x4::Identity;
		invertedScaleMat[0][0] = 1.0f / (scale.x * scale.x);
		invertedScaleMat[1][1] = 1.0f / (scale.y * scale.y);
		invertedScaleMat[2][2] = 1.0f / (scale.z * scale.z);

		// Creates the transposed rotation matrix w/ scale.
		Matrix4x4 transposedSelf = *this;
		transposedSelf[3][0] = 0.0f;
		transposedSelf[3][1] = 0.0f;
		transposedSelf[3][2] = 0.0f;
		transposedSelf[3][3] = 1.0f;
		transposedSelf.Transpose();

		// Creates the inverted translation matrix.
		Matrix4x4 invertedTranslationMatrix = Identity;
		invertedTranslationMatrix[3][0] = -matrix[3][0];
		invertedTranslationMatrix[3][1] = -matrix[3][1];
		invertedTranslationMatrix[3][2] = -matrix[3][2];
		invertedTranslationMatrix[3][3] = 1.0f;

		// Sets this to the inverted translation matrix * the inverted scale rotation matrix.
		*this = invertedTranslationMatrix * (transposedSelf * invertedScaleMat);
	}

	void Matrix4x4::Transpose()
	{
		const std::uint8_t COLS = 4, ROWS = 4;

		float temporaryMat[COLS][ROWS];
		for (std::uint8_t r = 0; r < COLS; r++)
		{
			for (int c = 0; c < COLS; c++)
			{
				temporaryMat[r][c] = matrix[c][r];
			}
		}
		std::memcpy(matrix, temporaryMat, sizeof(float) * COLS * ROWS);
	}

	float* Matrix4x4::operator[](int index) const
	{
		return const_cast<float*>(matrix[index]);
	}

	Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
	{
		const std::uint8_t ROWS = 4, COLS = 4;

		Matrix4x4 output{};
		std::uint32_t num = ROWS * COLS;
		for (std::uint8_t r = 0; r < ROWS; r++)
		{
			float total = 0.0f;
			for (std::uint8_t c = 0; c < num; c++)
			{
				total += a[r][c % COLS] * b[c % COLS][c / COLS];

				if ((c + 1) % COLS == 0
					&& c > 0)
				{
					output[r][c / COLS] = total;
					total = 0.0f;
				}
			}
		}
		return output;
	}

	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& mat)
	{
		const std::uint8_t ROWS = 4, COLS = 4;

		std::uint8_t num = ROWS * COLS;
		float oldMatrixData[ROWS][COLS];
		std::memcpy(oldMatrixData, matrix,
			num * sizeof(float));
		for (std::uint8_t r = 0; r < ROWS; r++)
		{
			float total = 0.0f;
			for (std::uint8_t c = 0; c < num; c++)
			{
				total += oldMatrixData[r][c % COLS] * mat[c % COLS][c / COLS];
				if (c % COLS == 0
					&& c > 0)
				{
					matrix[r / ROWS][c / COLS] = total;
					total = 0.0f;
				}
			}
		}
		return *this;
	}

	bool operator==(const Matrix4x4& a, const Matrix4x4& b)
	{
		const std::uint8_t ROWS = 4, COLS = 4;

		for (std::uint8_t r = 0; r < ROWS; r++)
		{
			for (std::uint8_t c = 0; c < COLS; c++)
			{
				if (!IsCloseEnough(a[r][c], b[r][c]))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool operator!=(const Matrix4x4& a, const Matrix4x4& b)
	{
		const std::uint8_t ROWS = 4, COLS = 4;

		for (std::uint8_t r = 0; r < ROWS; r++)
		{
			for (std::uint8_t c = 0; c < COLS; c++)
			{
				if (!IsCloseEnough(a[r][c], b[r][c]))
				{
					return true;
				}
			}
		}
		return false;
	}

	Matrix4x4 Transpose(const Matrix4x4& matrix)
	{
		Matrix4x4 cpy(matrix);
		cpy.Transpose();
		return cpy;
	}

	Matrix4x4 Invert(const Matrix4x4& matrix)
	{
		Matrix4x4 cpy(matrix);
		cpy.Invert();
		return cpy;
	}


#pragma endregion

}