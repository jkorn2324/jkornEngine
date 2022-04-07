#pragma once

#include "EngineMacros.h"
#include "Source\Vector.h"
#include "Source\Matrix.h"

#include <string>

namespace Engine
{
	class ConstantBuffer;
	class Texture;

	template<typename T>
	class AssetSerializer;
	template<typename T>
	class AssetCache;

	// TODO: Compute Shader Buffer Layout, Works in Tandem with Serialization
	// So that we can easily just put stuff inside of the Compute Shader directly
	// 
	// Structured like the material buffer layout, but instead there are
	// raw variables with given types specific to compute shader, such as:
	// Primitives (Ints, Floats, Bools)
	// Structures (Matrices, Float3s, Float2s, Float4s)
	// StructuredBuffers
	// RWStructuredBuffers
	// RWTexture2D - Must support asset cached textures & be able to be read from the GPU.

	class ComputeShader
	{
	public:
		explicit ComputeShader() = default;
		virtual ~ComputeShader() { }

		virtual void Dispatch(uint32_t x, uint32_t y, uint32_t z) = 0;
		virtual bool IsValid() const = 0;

		// Sets a constant buffer.
		template<typename T>
		void SetConstants(T& bufferData)
		{
			InternalSetConstants(&bufferData, sizeof(T));
		}

		virtual void SetFloat(const std::string& name, float value) =0;
		virtual void SetInt(const std::string& name, int value) =0;
		// TODO: Vector of Integers
		virtual void SetFloat2(const std::string& name, const MathLib::Vector2& vec2) =0;
		virtual void SetFloat3(const std::string& name, const MathLib::Vector3& vec3) =0;
		virtual void SetFloat4(const std::string& name, const MathLib::Vector4& vec4) =0;
		virtual void SetMatrix3x3(const std::string& name, const MathLib::Matrix3x3& mat) =0;
		virtual void SetMatrix4x4(const std::string& name, const MathLib::Matrix4x4& mat) =0;
		virtual void SetTexture(const std::string& name, const Texture& texture)=0;

	protected:
		virtual bool Load(const wchar_t* fileName) = 0;
		virtual void InternalSetConstants(void* bufferData, size_t stride)=0;
		
		SERIALIZABLE_ASSET(ComputeShader);
	};
}