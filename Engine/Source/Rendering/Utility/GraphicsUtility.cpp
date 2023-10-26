#include "EnginePCH.h"
#include "GraphicsUtility.h"

#include "ConstantBuffer.h"
#include "GraphicsRenderer3D.h"

namespace Engine
{
    // Sets the camera constant buffer.
	ConstantBuffer* s_cameraConstantBuffer = nullptr;

	void Graphics::Utility::SetCameraConstants(const CameraConstants &cameraConstants)
	{
        if (!s_cameraConstantBuffer)
        {
            s_cameraConstantBuffer = ConstantBuffer::Create(&cameraConstants,
                sizeof(CameraConstants));
        }
        else 
        {
            s_cameraConstantBuffer->SetData(&cameraConstants,
                sizeof(cameraConstants));
        }
        JKORN_ENGINE_ASSERT(s_cameraConstantBuffer, "The camera constant buffer must exist.");
		s_cameraConstantBuffer->Bind(0,
			Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);
	}

    void Graphics::Utility::BeginRenderScene(const CameraConstants &cameraConstants)
    {
        Graphics::Utility::SetCameraConstants(cameraConstants);
        GraphicsRenderer3D::BindLights();
    }
}
