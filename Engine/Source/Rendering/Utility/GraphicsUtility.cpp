#include "EnginePCH.h"
#include "GraphicsUtility.h"

#include "ConstantBuffer.h"
#include "GraphicsRenderer3D.h"

namespace Engine
{
    void Graphics::Utility::SetCameraConstants(const CameraConstants& cameraConstants, ConstantBuffer** cBuffer)
    {
        if (!*cBuffer)
        {
            ConstantBuffer::Create(cBuffer, &cameraConstants, sizeof(CameraConstants));
        }
        else
        {
            (*cBuffer)->SetData(&cameraConstants,
                sizeof(CameraConstants));
        }
        JKORN_ENGINE_ASSERT(*cBuffer, "The camera constant buffer must exist.");
        (*cBuffer)->Bind(0,
            Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);
    }

    void Graphics::Utility::BeginRenderScene(const CameraConstants &cameraConstants, ConstantBuffer** cBuffer)
    {
        Graphics::Utility::SetCameraConstants(cameraConstants, cBuffer);
        GraphicsRenderer3D::BindLights();
    }
}
