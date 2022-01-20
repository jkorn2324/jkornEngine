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

cbuffer UnlitShader : register(b2)
{
	float4 c_materialColor;
};

#include "TextureConstants.hlsl"
#include "ObjectConstants.hlsl"
#include "CameraConstants.hlsl"

VertexShaderOut VIn(VertexShaderIn vIn)
{
	VertexShaderOut output;

	float4 worldPos = mul(float4(vIn.pos, 1.0), c_objectToWorld);
	output.worldPos = worldPos.xyz;
	output.pos = mul(worldPos, c_viewProjection);
	
	float4 worldNormal = mul(float4(vIn.normal, 0.0), c_objectToWorld);
	output.normal = mul(worldNormal, c_viewProjection);
	output.uv = vIn.uv;
	return output;
}

float4 VOut(VertexShaderOut vIn)
{
	return DefaultTexture.Sample(DefaultSampler, vIn.uv) * c_materialColor;

}