#pragma once

#include "Vector.h"

namespace Engine
{
	/**
	 * Directional Light Component.
	 */
	struct DirectionalLightComponent
	{
		MathLib::Vector3 lightColor = MathLib::Vector3::One;
		float lightIntensity = 1.0f;
		bool enabled = true;

		DirectionalLightComponent() { }
		DirectionalLightComponent(const MathLib::Vector3& lightColor)
			: lightColor(lightColor) { }
	};

	/**
	 * The Point light Component.
	 */
	struct PointLightComponent
	{
		MathLib::Vector3 lightColor = MathLib::Vector3::One;
		float innerRadius = 1.0f;
		float outerRadius = 1.0f;
		float lightIntensity = 1.0f;
		bool enabled = true;

		PointLightComponent() { }
		PointLightComponent(float innerRadius, float outerRadius, float lightIntensity)
			: innerRadius(innerRadius), outerRadius(outerRadius), lightIntensity(lightIntensity) { }
		PointLightComponent(const MathLib::Vector3& lightColor)
			: lightColor(lightColor) { }
	};
}
