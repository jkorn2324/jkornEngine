struct VertexShaderIn
{
    float3 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct VertexShaderOut
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION0;
    float2 uv : TEXCOORD0;
};

#include "TextureConstants.hlsl"
#include "CameraConstants.hlsl"
#include "ObjectConstants.hlsl"
#include "MaterialConstants.hlsl"

cbuffer SpriteConstants : register(b3)
{
    float4 c_spriteColor;
};

VertexShaderOut VS(VertexShaderIn input)
{
    VertexShaderOut output;
    
    float4 worldPos = mul(float4(input.position, 1.0), c_objectToWorld);
    output.worldPosition = worldPos.xyz;
    output.position = mul(worldPos, c_viewProjection);
    output.uv = input.uv;
    
    return output;
}

float4 PS(VertexShaderOut input) : SV_TARGET
{
    float4 textureColor = float4(1.0, 1.0, 1.0, 1.0);
    if (HAS_MATERIAL_FLAG(MaterialFlag_DefaultTexture))
    {
        textureColor = DefaultTexture.Sample(DefaultSampler, input.uv);
    }
    return textureColor * c_spriteColor;
}
