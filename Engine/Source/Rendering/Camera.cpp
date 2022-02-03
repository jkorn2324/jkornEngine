#include "EnginePCH.h"
#include "Camera.h"

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "Profiler.h"

namespace Engine
{
	MathLib::Vector3 Camera::ScreenToWorld(const MathLib::Vector2& screenPos)
	{
		PROFILE_SCOPE(CameraScreenToWorld, Camera);

		RenderingAPI& api = GraphicsRenderer::GetRenderingAPI();
		MathLib::Vector2 camSpace(screenPos.x / (float)api.GetWidth(),
			screenPos.y / (float)api.GetHeight());
		camSpace = camSpace * 2.0f - MathLib::Vector2::One;
		MathLib::Matrix4x4 invertedProjMat(GetViewProjectionMatrix());
		invertedProjMat.Invert();
		MathLib::Vector4 worldSpace = MathLib::Vector4(camSpace, 0.0f, 1.0f)
			* invertedProjMat;
		return MathLib::Vector3(worldSpace.x, -worldSpace.y, worldSpace.z) 
			+ -m_viewMatrix.GetTranslation();
	}
	
	MathLib::Vector2 Camera::WorldToScreen(const MathLib::Vector3& worldPos)
	{
		PROFILE_SCOPE(CameraWorldToScreen, Camera);

		RenderingAPI& api =(RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
		MathLib::Vector4 asVec4 = MathLib::Vector4(worldPos, 1.0f);
		asVec4.y *= -1.0f;
		asVec4 = asVec4 * GetViewProjectionMatrix();
		MathLib::Vector2 camSpace((asVec4.x + 1.0f) * 0.5f, (asVec4.y + 1.0f) * 0.5f);
		camSpace.x *= (float)api.GetWidth();
		camSpace.y *= (float)api.GetHeight();
		return camSpace;
	}
}