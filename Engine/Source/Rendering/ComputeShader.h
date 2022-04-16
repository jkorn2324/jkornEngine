#pragma once

#include "EngineMacros.h"
#include "Source\Vector.h"
#include "Source\Matrix.h"

#include <string>
#include <memory>

namespace Engine
{
	class ConstantBuffer;
	class Texture;

	template<typename T>
	class AssetSerializer;
	template<typename T>
	class AssetCache;
	template<typename T>
	class AssetRef;

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

	enum ComputeBufferType
	{
		Type_RWStructuredBuffer,
		Type_StructuredBuffer,
		Type_ConstantBuffer
	};

	// Compute Buffer abstraction class.
	class ComputeBuffer
	{
	public:
		ComputeBuffer(const ComputeBufferType& bufferType)
			: m_computeBufferType(bufferType),
			m_bufferStride(0),
			m_bufferData(nullptr),
			m_numElements(0)
		{

		}

		~ComputeBuffer()
		{
			if (m_bufferData != nullptr)
			{
				delete[] m_bufferData;
			}
		}

		const ComputeBufferType& GetBufferType() const { return m_computeBufferType; }

		uint32_t GetNumElements() const { return m_numElements; }

		size_t GetStride() const { return m_bufferStride; }

		template<typename T>
		void SetData(T* inData, uint32_t numElements)
		{
			size_t tStride = sizeof(T);
			if (m_numElements != numElements
				|| tStride != m_bufferStride)
			{
				if (m_bufferData != nullptr)
				{
					delete[] m_bufferData;
				}
				m_bufferData = new char[tStride * m_numElements];
				m_bufferStride = tStride;
			}
			std::memcpy(m_bufferData, inData, tStride * m_numElements);
		}

		const void* GetData() const
		{
			return m_bufferData;
		}

	private:
		void* m_bufferData;
		size_t m_bufferStride;
		uint32_t m_numElements;
		ComputeBufferType m_computeBufferType;
	};

	class ComputeShader
	{
	public:
		explicit ComputeShader() = default;
		virtual ~ComputeShader() { }

		virtual void Dispatch(uint32_t x, uint32_t y, uint32_t z) = 0;
		virtual bool IsValid() const = 0;

		virtual void SetTexture(const AssetRef<Texture>& texture)=0;
		virtual void SetBuffer(const std::shared_ptr<ComputeBuffer>& buffer)=0;

	protected:
		virtual bool Load(const wchar_t* fileName) = 0;
		
		SERIALIZABLE_ASSET(ComputeShader);
	};
}