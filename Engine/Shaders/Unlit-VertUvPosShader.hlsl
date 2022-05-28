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
    int entityID : ENTITYID;
};

// Material Constants
cbuffer UnlitShader : register(b3)
{
	float4 c_materialColor;
};

#include "TextureConstants.hlsl"
#include "ObjectConstants.hlsl"
#include "CameraConstants.hlsl"
#include "MaterialConstants.hlsl"

VertexShaderOut VS(VertexShaderIn vIn)
{
	VertexShaderOut output;

	float4 worldPos = mul(float4(vIn.pos, 1.0), c_objectToWorld);
	output.worldPos = worldPos.xyz;
	output.pos = mul(worldPos, c_viewProjection);
	
	float4 worldNormal = mul(float4(vIn.normal, 0.0), c_objectToWorld);
	output.normal = mul(worldNormal, c_viewProjection);
	output.uv = vIn.uv;
    output.entityID = GetEntityID();
	return output;
}

struct PSOut
{
    float4 outputColor : SV_Target0;
    int entityID : SV_Target1;
};

PSOut PS(VertexShaderOut psIn) : SV_TARGET
{
    PSOut psOut;
    float4 textureColor = float4(1.0, 1.0, 1.0, 1.0);
	if(HAS_MATERIAL_FLAG(MaterialFlag_DefaultTexture))
    {
        textureColor = DefaultTexture.Sample(DefaultSampler, psIn.uv);
    }
    psOut.outputColor = textureColor * c_materialColor;
    psOut.entityID = psIn.entityID;
    return psOut;
}