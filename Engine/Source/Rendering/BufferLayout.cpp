#include "EnginePCH.h"
#include "BufferLayout.h"

#include <string.h>

namespace Engine
{
	static DXGI_FORMAT GetFormatFromEnum(std::size_t stride, const BufferLayoutType& layoutType)
	{
		switch (layoutType)
		{
			case BufferLayoutType::FLOAT:	return DXGI_FORMAT_R32_FLOAT;
			case BufferLayoutType::FLOAT2:	return DXGI_FORMAT_R32G32_FLOAT;
			case BufferLayoutType::FLOAT3:	return DXGI_FORMAT_R32G32B32_FLOAT;
			case BufferLayoutType::FLOAT4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case BufferLayoutType::INT:		
			{
				std::size_t sizeOfStride = sizeof(stride);
				if (sizeOfStride == sizeof(std::int16_t))
				{
					return DXGI_FORMAT_R16_SINT;
				}
				else if (sizeOfStride == sizeof(std::int8_t))
				{
					return DXGI_FORMAT_R8_SINT;
				}
				else if (sizeOfStride == sizeof(std::int32_t))
				{
					return DXGI_FORMAT_R32_SINT;
				}
				DebugAssert(false, 
					"Buffer stride components should be either 8, 16, or 32 Bytes");
				break;
			}
			case BufferLayoutType::INT2: 
			{
				std::size_t sizeOfStride = sizeof(stride) / 2;
				if (sizeOfStride == sizeof(std::int16_t))
				{
					return DXGI_FORMAT_R16G16_SINT;
				}
				else if (sizeOfStride == sizeof(std::int8_t))
				{
					return DXGI_FORMAT_R8G8_SINT;
				}
				else if (sizeOfStride == sizeof(std::int32_t))
				{
					return DXGI_FORMAT_R32G32_SINT;
				}
				DebugAssert(false,
					"Buffer stride components should be either 8, 16, or 32 Bytes");
				break;
			}
			case BufferLayoutType::INT3:
			{
				if (sizeof(stride) / 3 == sizeof(std::int32_t))
				{
					return DXGI_FORMAT_R32G32B32_SINT;
				}
				DebugAssert(false,
					"Buffer Stride components should be 32 Bytes");
				break;
			}
			case BufferLayoutType::INT4:
			{
				std::size_t sizeOfStride = sizeof(stride) / 4;
				if (sizeOfStride == sizeof(std::int16_t))
				{
					return DXGI_FORMAT_R16G16B16A16_SINT;
				}
				else if (sizeOfStride == sizeof(std::int8_t))
				{
					return DXGI_FORMAT_R8G8B8A8_SINT;
				}
				else if (sizeOfStride == sizeof(std::int32_t))
				{
					return DXGI_FORMAT_R32G32B32A32_SINT;
				}
				DebugAssert(false,
					"Buffer stride components should be either 8, 16, or 32 Bytes");
				break;
			}
			case BufferLayoutType::UINT:
			{
				std::size_t sizeOfStride = sizeof(stride);
				if (sizeOfStride == sizeof(std::uint16_t))
				{
					return DXGI_FORMAT_R16_UINT;
				}
				else if (sizeOfStride == sizeof(std::uint8_t))
				{
					return DXGI_FORMAT_R8_UINT;
				}
				else if (sizeOfStride == sizeof(std::uint32_t))
				{
					return DXGI_FORMAT_R32_UINT;
				}
				DebugAssert(false,
					"Buffer stride components should be either 8, 16, or 32 Bytes");
				break;
			}
			case BufferLayoutType::UINT2:
			{
				std::size_t sizeOfStride = sizeof(stride) / 2;
				if (sizeOfStride == sizeof(std::uint16_t))
				{
					return DXGI_FORMAT_R16G16_SINT;
				}
				else if (sizeOfStride == sizeof(std::uint8_t))
				{
					return DXGI_FORMAT_R8G8_SINT;
				}
				else if (sizeOfStride == sizeof(std::uint32_t))
				{
					return DXGI_FORMAT_R32G32_UINT;
				}
				DebugAssert(false,
					"Buffer stride components should be either 8, 16, or 32 Bytes");
				break;
			}
			case BufferLayoutType::UINT3:
			{
				if (sizeof(stride) / 3 == sizeof(std::uint32_t))
				{
					return DXGI_FORMAT_R32G32B32_UINT;
				}
				DebugAssert(false,
					"Buffer Stride components should be 32 Bytes");
				break;
			}
			case BufferLayoutType::UINT4:
			{
				std::size_t sizeOfStride = sizeof(stride) / 4;
				if (sizeOfStride == sizeof(std::uint16_t))
				{
					return DXGI_FORMAT_R16G16B16A16_UINT;
				}
				else if (sizeOfStride == sizeof(std::uint8_t))
				{
					return DXGI_FORMAT_R8G8B8A8_UINT;
				}
				else if (sizeOfStride == sizeof(std::uint32_t))
				{
					return DXGI_FORMAT_R32G32B32A32_UINT;
				}
				DebugAssert(false,
					"Buffer stride components should be either 8, 16, or 32 Bytes");
				break;
			}
			default:
				DebugAssert(false, "Format from buffer layout type doesn't exist.");
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferLayoutParam>& layoutParameters)
		: m_numElements((uint32_t)layoutParameters.size()), m_inputElementDesc(nullptr)
	{
		m_inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[m_numElements];
		// Creates the input element description.
		for (std::uint32_t i = 0; i < m_numElements; i++)
		{
			const BufferLayoutParam& param = *(layoutParameters.begin() + i);
			D3D11_INPUT_ELEMENT_DESC description;
			ZeroMemory(&description, sizeof(description));

			// TODO: Make it so that semantic indices can change.
			description.SemanticIndex = 0;
			description.InputSlot = 0;
			description.InstanceDataStepRate = 0;

			description.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
			description.SemanticName = new char[MAX_SIZE_OF_NAME + 1];
			std::memcpy(const_cast<char*>(description.SemanticName),
				param.name, MAX_SIZE_OF_NAME);
			description.Format = GetFormatFromEnum(
				param.stride, param.layoutType);
			description.AlignedByteOffset = param.offset;
			m_inputElementDesc[i] = description;
		}
	}

	BufferLayout::~BufferLayout()
	{
		for (std::uint32_t i = 0; i < m_numElements; i++)
		{
			D3D11_INPUT_ELEMENT_DESC desc = m_inputElementDesc[i];
			delete[] desc.SemanticName;
		}
		delete[] m_inputElementDesc;
	}

	std::uint32_t BufferLayout::GetNumElements() const
	{
		return m_numElements;
	}

	const D3D11_INPUT_ELEMENT_DESC* BufferLayout::GetD3D11InputElementDesc() const
	{
		return m_inputElementDesc;
	}
}
