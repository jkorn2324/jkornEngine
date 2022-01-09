#include "EnginePCH.h"
#include "Camera.h"

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"

namespace Engine
{
	MathLib::Vector3 Camera::ScreenToWorld(const MathLib::Vector2& screenPos)
	{
		RenderingAPI* api = GraphicsRenderer::Get()->GetRenderingAPI();
		MathLib::Vector2 camSpace(screenPos.x / (float)api->GetWidth(),
			screenPos.y / (float)api->GetHeight());
		camSpace = camSpace * 2.0f - MathLib::Vector2::One;
		MathLib::Matrix4x4 invertedProjMat(GetViewProjectionMatrix());
		invertedProjMat.Invert();
		MathLib::Vector4 worldSpace = MathLib::Vector4(camSpace, 0.0f, 1.0f)
			* invertedProjMat;
		return MathLib::Vector3(worldSpace.x, -worldSpace.y, worldSpace.z);
	}
	
	MathLib::Vector2 Camera::WorldToScreen(const MathLib::Vector3& worldPos)
	{
		RenderingAPI* api = GraphicsRenderer::Get()->GetRenderingAPI();
		MathLib::Vector4 asVec4 = MathLib::Vector4(worldPos, 1.0f);
		asVec4.y *= -1.0f;
		asVec4 = asVec4 * GetViewProjectionMatrix();
		MathLib::Vector2 camSpace((asVec4.x + 1.0f) * 0.5f, (asVec4.y + 1.0f) * 0.5f);
		camSpace.x *= (float)api->GetWidth();
		camSpace.y *= (float)api->GetHeight();
		return camSpace;
	}
}