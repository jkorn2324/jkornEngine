#pragma once

#include "ComputeShader.h"

struct ID3D11ComputeShader;

namespace Engine
{
	class ConstantBuffer;

	class DirectX11ComputeShader : public ComputeShader
	{
	public:
		explicit DirectX11ComputeShader();
		~DirectX11ComputeShader();

		void Dispatch(uint32_t x, uint32_t y, uint32_t z) override;
		bool IsValid() const override;

		void SetFloat(const std::string& name, float value) override;
		void SetInt(const std::string& name, int value) override;
		// TODO: Vector of Integers
		void SetFloat2(const std::string& name, const MathLib::Vector2& vec2) override;
		void SetFloat3(const std::string& name, const MathLib::Vector3& vec3) override;
		void SetFloat4(const std::string& name, const MathLib::Vector4& vec4) override;
		void SetMatrix3x3(const std::string& name, const MathLib::Matrix3x3& mat) override;
		void SetMatrix4x4(const std::string& name, const MathLib::Matrix4x4& mat) override;
		void SetTexture(const std::string& name, const Texture& texture) override;

	protected:
		void InternalSetConstants(void* bufferData, size_t stride) override;
		bool Load(const wchar_t* fileName) override;
		
	private:
		ConstantBuffer* m_constantBuffer;
		ID3D11ComputeShader* m_computeShader;
	};
}