#include "EnginePCH.h"
#include "DirectX11BufferLayoutParser.h"
#include "Memory.h"

#include <d3d11.h>

namespace Engine
{

	static bool GetSemanticNameFromType(std::string& out, BufferLayoutSemanticType semanticType)
	{
		switch (semanticType)
		{
		case BufferLayoutSemanticType::Type_Binormal:
		{
			out = "BINORMAL";
			return true;
		}
		case BufferLayoutSemanticType::Type_Color:
		{
			out = "COLOR";
			return true;
		}
		case BufferLayoutSemanticType::Type_Normal:
		{
			out = "NORMAL";
			return true;
		}
		case BufferLayoutSemanticType::Type_Position:
		{
			out = "POSITION";
			return true;
		}
		case BufferLayoutSemanticType::Type_Tangent:
		{
			out = "TANGENT";
			return true;
		}
		case BufferLayoutSemanticType::Type_TexCoord:
		{
			out = "TEXCOORD";
			return true;
		}
		}
		return false;
	}

	static DXGI_FORMAT GetFormatFromParam(const BufferLayoutParam& param)
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

	DirectX11BufferLayoutParser::DirectX11BufferLayoutParser(const BufferLayout& bufferLayout)
		: m_numElements(bufferLayout.GetTotalParamSize()),
		m_inputElementDesc(nullptr)
	{
		m_inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[m_numElements];
		uint32_t numInputSlots = bufferLayout.GetNumElements();

		uint32_t descSlot = 0;
		for (uint32_t inputSlot = 0; inputSlot < numInputSlots; ++inputSlot)
		{
			const BufferLayoutParameterSet& params = *(bufferLayout.parameters.begin() + inputSlot);
			uint32_t currentOffset = 0;
			for (size_t paramIndex = 0; paramIndex < params.parameters.size(); ++paramIndex)
			{
				if (descSlot >= m_numElements) return;
				const BufferLayoutParam& param = params.parameters[paramIndex];
				D3D11_INPUT_ELEMENT_DESC description;
				ZeroMemory(&description, sizeof(description));

				std::string outSemanticName;
				if (GetSemanticNameFromType(outSemanticName, param.semanticType))
				{
					char* semanticName = new char[outSemanticName.size() + 1];
					semanticName[outSemanticName.size()] = 0;
					Memory::Memcpy(semanticName,
						outSemanticName.c_str(), outSemanticName.size());
					description.SemanticName = semanticName;
				}
				else
				{
					char* semanticName = new char[param.name.size() + 1];
					semanticName[param.name.size()] = 0;
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
				currentOffset += param.GetStride();
				descSlot++;
			}
		}
	}
	
	DirectX11BufferLayoutParser::~DirectX11BufferLayoutParser()
	{
		for (std::uint32_t i = 0; i < m_numElements; i++)
		{
			D3D11_INPUT_ELEMENT_DESC desc = m_inputElementDesc[i];
			delete[] desc.SemanticName;
		}
		delete[] m_inputElementDesc;
	}
	
	const D3D11_INPUT_ELEMENT_DESC* DirectX11BufferLayoutParser::GetD3D11InputElementDesc() const
	{
		return m_inputElementDesc;
	}
}