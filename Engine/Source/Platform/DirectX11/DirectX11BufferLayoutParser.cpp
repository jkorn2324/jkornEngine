#include "EnginePCH.h"
#include "DirectX11BufferLayoutParser.h"
#include "Memory.h"

#include <d3d11.h>

namespace Engine
{
	namespace
	{
		bool GetSemanticNameFromType(char* outPtr, BufferLayoutSemanticType semanticType)
		{
			switch (semanticType)
			{
			case BufferLayoutSemanticType::Type_Binormal:
			{
				const char semantic[] = "BINORMAL";
				Memory::Memcpy(outPtr, semantic, sizeof(semantic));
				return true;
			}
			case BufferLayoutSemanticType::Type_Color:
			{
				const char semantic[] = "COLOR";
				Memory::Memcpy(outPtr, semantic, sizeof(semantic));
				return true;
			}
			case BufferLayoutSemanticType::Type_Normal:
			{
				const char semantic[] = "NORMAL";
				Memory::Memcpy(outPtr, semantic, sizeof(semantic));
				return true;
			}
			case BufferLayoutSemanticType::Type_Position:
			{
				const char semantic[] = "POSITION";
				Memory::Memcpy(outPtr, semantic, sizeof(semantic));
				return true;
			}
			case BufferLayoutSemanticType::Type_Tangent:
			{
				const char semantic[] = "TANGENT";
				Memory::Memcpy(outPtr, semantic, sizeof(semantic));
				return true;
			}
			case BufferLayoutSemanticType::Type_TexCoord:
			{
				const char semantic[] = "TEXCOORD";
				Memory::Memcpy(outPtr, semantic, sizeof(semantic));
				return true;
			}
			}
			return false;
		}

		DXGI_FORMAT GetFormatFromParam(const BufferLayoutParam& param)
		{
			switch (param.layoutType)
			{
			case BufferLayoutType::Float16:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R16_FLOAT;
				case 2: return DXGI_FORMAT_R16G16_FLOAT;
				default: return DXGI_FORMAT_R16G16B16A16_FLOAT;
				}
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
			}
			case BufferLayoutType::Float32:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R32_FLOAT;
				case 2: return DXGI_FORMAT_R32G32_FLOAT;
				case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
				default: return DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			case BufferLayoutType::Int8:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R8_SINT;
				case 2: return DXGI_FORMAT_R8G8_SINT;
				default: return DXGI_FORMAT_R8G8B8A8_SINT;
				}
				return DXGI_FORMAT_R8G8B8A8_SINT;
			}
			case BufferLayoutType::Int16:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R16_SINT;
				case 2: return DXGI_FORMAT_R16G16_SINT;
				default: return DXGI_FORMAT_R16G16B16A16_SINT;
				}
				return DXGI_FORMAT_R16G16B16A16_SINT;
			}
			case BufferLayoutType::Int32:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R32_SINT;
				case 2: return DXGI_FORMAT_R32G32_SINT;
				case 3: return DXGI_FORMAT_R32G32B32_SINT;
				default: return DXGI_FORMAT_R32G32B32A32_SINT;
				}
				return DXGI_FORMAT_R32G32B32A32_SINT;
			}
			case BufferLayoutType::Uint8:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R8_UINT;
				case 2: return DXGI_FORMAT_R8G8_UINT;
				default: return DXGI_FORMAT_R8G8B8A8_UINT;
				}
				return DXGI_FORMAT_R8G8B8A8_UINT;
			}
			case BufferLayoutType::Uint16:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R16_UINT;
				case 2: return DXGI_FORMAT_R16G16_UINT;
				default: return DXGI_FORMAT_R16G16B16A16_UINT;
				}
				return DXGI_FORMAT_R16G16B16A16_UINT;
			}
			case BufferLayoutType::Uint32:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R32_UINT;
				case 2: return DXGI_FORMAT_R32G32_UINT;
				case 3: return DXGI_FORMAT_R32G32B32_UINT;
				default: return DXGI_FORMAT_R32G32B32A32_UINT;
				}
				return DXGI_FORMAT_R32G32B32A32_UINT;
			}
			case BufferLayoutType::SNorm8:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R8_SNORM;
				case 2: return DXGI_FORMAT_R8G8_SNORM;
				default: return DXGI_FORMAT_R8G8B8A8_SNORM;
				}
				return DXGI_FORMAT_R8G8B8A8_SNORM;
			}
			case BufferLayoutType::SNorm16:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R16_SNORM;
				case 2: return DXGI_FORMAT_R16G16_SNORM;
				default: return DXGI_FORMAT_R16G16B16A16_SNORM;
				}
				return DXGI_FORMAT_R16G16B16A16_SNORM;
			}
			case BufferLayoutType::UNorm8:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R8_UNORM;
				case 2: return DXGI_FORMAT_R8G8_UNORM;
				default: return DXGI_FORMAT_R8G8B8A8_UNORM;
				}
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}
			case BufferLayoutType::UNorm16:
			{
				switch (param.numValues)
				{
				case 1: return DXGI_FORMAT_R16_UNORM;
				case 2: return DXGI_FORMAT_R16G16_UNORM;
				default: return DXGI_FORMAT_R16G16B16A16_UNORM;
				}
				return DXGI_FORMAT_R16G16B16A16_UNORM;
			}
			}
			return DXGI_FORMAT_UNKNOWN;
		}

		const size_t c_MaxStringLength = 100;
		const size_t c_MaxSlots = DirectX11BufferLayoutParser::MaxSlots * 16;

		constexpr size_t ClampLength(size_t size)
		{
			return size >= c_MaxStringLength ? c_MaxStringLength : size + 1;
		}

		char* GetSemanticPtr(size_t index)
		{
			if (index >= c_MaxSlots)
			{
				return nullptr;
			}
			static char semantic[c_MaxSlots][c_MaxStringLength];
			return semantic[index];
		}
	}

	DirectX11BufferLayoutParser::DirectX11BufferLayoutParser(const BufferLayout& bufferLayout)
		: m_numElements(bufferLayout.GetTotalParamSize()), m_inputElementDesc()
	{
		uint32_t numInputSlots = bufferLayout.GetNumElements();
		uint32_t descSlot = 0;
		for (uint32_t inputSlot = 0; inputSlot < numInputSlots; ++inputSlot)
		{
			const BufferLayoutParameterSet& params = *(bufferLayout.parameters.begin() + inputSlot);
			uint32_t currentOffset = 0;
			for (size_t paramIndex = 0; paramIndex < params.parameters.size(); ++paramIndex)
			{
				if (descSlot >= m_numElements)
				{
					return;
				}
				const BufferLayoutParam& param = params.parameters[paramIndex];
				D3D11_INPUT_ELEMENT_DESC description;
				ZeroMemory(&description, sizeof(description));
				
				// Temporary semantic 
				char* semanticName = GetSemanticPtr(descSlot);
				ZeroMemory(semanticName, c_MaxStringLength);

				if (GetSemanticNameFromType(semanticName, param.semanticType))
				{
					description.SemanticName = semanticName;
				}
				else
				{
					Memory::Memcpy(semanticName,
						param.name.c_str(), param.name.size());
					description.SemanticName = semanticName;
				}

				description.SemanticIndex = param.semanticIndex;
				description.InputSlot = inputSlot;
				description.InstanceDataStepRate = 0;

				description.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
				description.Format = GetFormatFromParam(param);
				description.AlignedByteOffset = currentOffset;
				m_inputElementDesc[descSlot] = description;
				currentOffset += (uint32_t)param.GetStride();
				descSlot++;
			}
		}
	}

	DirectX11BufferLayoutParser::~DirectX11BufferLayoutParser()
	{
	}

	const D3D11_INPUT_ELEMENT_DESC* DirectX11BufferLayoutParser::GetD3D11InputElementDesc() const
	{
		return m_inputElementDesc;
	}
}