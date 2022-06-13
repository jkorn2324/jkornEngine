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

	const BufferLayoutParam
		BufferLayoutParam::Uv1 = { "UV", BufferLayoutSemanticType::Type_TexCoord, BufferLayoutType::Float32, 2, 1 };

	const BufferLayoutParam
		BufferLayoutParam::Uv2 = { "UV", BufferLayoutSemanticType::Type_TexCoord, BufferLayoutType::Float32, 2, 2 };
	
	const BufferLayoutParam
		BufferLayoutParam::Uv3 = { "UV", BufferLayoutSemanticType::Type_TexCoord, BufferLayoutType::Float32, 2, 3 };

	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Position = { BufferLayoutParam::Position0 };
	
	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Normal = { BufferLayoutParam::Normal0 };

	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Binormal = { BufferLayoutParam::Binormal0 };

	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Tangent = { BufferLayoutParam::Tangent0 };

	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Color = { BufferLayoutParam::Color4_0 };

	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Uv0 = { BufferLayoutParam::Uv0 };

	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Uv1 = { BufferLayoutParam::Uv1 };

	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Uv2 = { BufferLayoutParam::Uv2 };

	const BufferLayoutParameterSet
		BufferLayoutParameterSet::Uv3 = { BufferLayoutParam::Uv3 };
}