#pragma pack_matrix(row_major)

struct GraphicsPointLightData
{
    float3 position;
    float3 lightColor;
    float innerRadius;
    float outerRadius;
    float intensity;
    bool isEnabled;
};

struct GraphicsDirectionalLightData
{
    float3 direction;
    float3 lightColor;
    float intensity;
    bool isEnabled;
};

#define MAX_POINT_LIGHTS 32

cbuffer LightingConstantBuffer : register(b4)
{
    float3 c_ambientLight;
    GraphicsDirectionalLightData c_directionalLight;
    GraphicsPointLightData c_pointLights[MAX_POINT_LIGHTS];
};

float3 CalculateDiffuseLighting(float3 normal, float3 directionToLightFromPixel, float3 diffuseColor,
    float3 lightColor, float lightIntensity)
{
    float diffuseOutput = max(0.0, dot(normal, directionToLightFromPixel)) * diffuseColor;
    return diffuseOutput * diffuseColor * (lightColor.xyz * lightIntensity);
}

float3 CalculateSpecularLighting(float3 normal, float3 directionToPixelFromLight, float3 cameraPosition,
    float3 worldPixelPosition, float3 specularColor, float3 lightColor,
    float lightIntensity, float specularPower)
{
    float3 directionToCameraFromPixel = normalize(cameraPosition - worldPixelPosition);
    float3 reflectedVector = normalize(reflect(directionToPixelFromLight, normal));
    float dotToReflectedUnclamped = max(dot(reflectedVector, directionToCameraFromPixel), 0.0);
    float output = pow(dotToReflectedUnclamped, specularPower);
    return output * specularColor * (lightColor * lightIntensity);
}