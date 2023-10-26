#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace Engine
{
    /**
     * @brief The camera constants.
     */
    struct CameraConstants
	{
		MathLib::Matrix4x4 c_viewProjection;
		MathLib::Vector3 c_cameraPosition;

	private:
		float pad;
	};

    namespace Graphics::Utility
    {
        /**
		 * @brief Set the Camera Constants.
		 * @param cameraConstants 
		 */
        static void SetCameraConstants(const CameraConstants& cameraConstants);

        /**
         * @brief Begins the scene.
         * @param cameraConstants 
         */
        void BeginRenderScene(const CameraConstants& cameraConstants);

        /**
         * @brief Ends rendering the scene.
         */
        static void EndRenderScene() { }
    }
}
