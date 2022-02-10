struct VertexShaderIn
{
    float3 pos : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

struct VertexShaderOut
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

// Material Constants
cbuffer LitShaderConstants : register(b3)
{
    // Gets multiplied onto the color provided by a texture.
    float4 c_diffuseColor;
    float4 c_specularColor;
    float c_specularPower;
};

#include "TextureConstants.hlsl"
#include "ObjectConstants.hlsl"
#include "CameraConstants.hlsl"
#include "MaterialConstants.hlsl"
#include "LightingConstants.hlsl"

VertexShaderOut VS(VertexShaderIn vIn)
{
    VertexShaderOut output;

    float4 worldPos = mul(float4(vIn.pos, 1.0), c_objectToWorld);
    output.worldPos = worldPos.xyz;
    output.pos = mul(worldPos, c_viewProjection);
	
    float4 worldNormal = mul(float4(vIn.normal, 0.0), c_objectToWorld);
    output.normal = worldNormal.xyz;
    output.uv = vIn.uv;
    return output;
}

float4 PhongLightingCalculation(VertexShaderOut psIn)
{
    float3 ambientCalculated = c_diffuseColor.xyz * c_ambientLight;
    float3 lightColor = float3(0.0, 0.0, 0.0);
    if (c_directionalLight.isEnabled)
    {
        float3 directionToPixelFromLight = normalize(c_directionalLight.direction);
        float3 diffuse = CalculateDiffuseLighting(psIn.normal,
            -directionToPixelFromLight, c_diffuseColor.xyz,
            c_directionalLight.lightColor, c_directionalLight.intensity);
        
        float3 specular = CalculateSpecularLighting(psIn.normal,
                directionToPixelFromLight, c_cameraPosition,
                 psIn.worldPos, c_specularColor.xyz, c_directionalLight.lightColor,
                c_directionalLight.intensity, c_specularPower);
        
        lightColor = diffuse + specular;
    }
    
    // Point Light Implementation
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        GraphicsPointLightData pointLight = c_pointLights[i];
        if (!pointLight.isEnabled)
        {
            break;
        }
        
        float3 difference = pointLight.position - psIn.worldPos;
        float3 diffuseCalculation = CalculateDiffuseLighting(psIn.normal,
            normalize(difference), pointLight.lightColor,
            c_diffuseColor.xyz, pointLight.intensity);
        float3 specularCalculation = CalculateSpecularLighting(psIn.normal,
            -normalize(difference), c_cameraPosition, psIn.worldPos,
            c_specularColor.xyz, pointLight.lightColor, pointLight.intensity, c_specularPower);
        
        float brightness = 1.0 - smoothstep(pointLight.innerRadius,
            pointLight.outerRadius, length(difference));
        float3 combinedOutput = diffuseCalculation * brightness + specularCalculation * brightness;
        lightColor += combinedOutput;
    }
    return float4(ambientCalculated + lightColor, 1.0);
}

float4 PS(VertexShaderOut psIn) : SV_TARGET
{
    psIn.normal = normalize(psIn.normal);
    float4 textureColor = float4(1.0, 1.0, 1.0, 1.0);
    if (HAS_MATERIAL_FLAG(MaterialFlag_DefaultTexture))
    {
        textureColor = DefaultTexture.Sample(DefaultSampler, psIn.uv);
    }
    return textureColor * PhongLightingCalculation(psIn);
}