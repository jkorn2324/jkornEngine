struct VertexShaderIn
{
	float3 position : POSITION0;
	float4 color : COLOR0;
};

struct VertexShaderOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

VertexShaderOut VS(VertexShaderIn input)
{
	VertexShaderOut output;
    output.position = float4(input.position, 1.0);
	output.color = input.color;
	return output;
}


struct PixelShaderOutput
{
    float4 color : SV_Target0;
};

PixelShaderOutput PS(VertexShaderOut input) : SV_TARGET
{
    PixelShaderOutput output;
    output.color = input.color;
	return output;
}
