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

float4 PS(VertexShaderOut input) : SV_TARGET
{
	return input.color;
}
