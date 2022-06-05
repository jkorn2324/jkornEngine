#include "EnginePCH.h"
#include "BufferLayout.h"

namespace Engine
{

	const BufferLayoutParam
		BufferLayoutParam::Position0 = { "Position", BufferLayoutSemanticType::Type_Position, BufferLayoutType::Float32, 3 };
	
	const BufferLayoutParam
		BufferLayoutParam::Normal0 = { "Normal", BufferLayoutSemanticType::Type_Normal, BufferLayoutType::Float32, 3 };
	
	const BufferLayoutParam
		BufferLayoutParam::Binormal0 = {"Binormal", BufferLayoutSemanticType::Type_Binormal, BufferLayoutType::Float32, 3 };

	const BufferLayoutParam
		BufferLayoutParam::Tangent0 = { "Tangent", BufferLayoutSemanticType::Type_Tangent, BufferLayoutType::Float32, 3 };

	const BufferLayoutParam
		BufferLayoutParam::Color4_0 = { "Color", BufferLayoutSemanticType::Type_Color, BufferLayoutType::Float32, 4 };

	const BufferLayoutParam
		BufferLayoutParam::Uv0 = { "UV", BufferLayoutSemanticType::Type_TexCoord, BufferLayoutType::Float32, 2 };

}