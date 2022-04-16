#pragma pack_matrix(row_major)

struct ComputeStruct
{
    
};

SamplerState DefaultSampler : register(s0);
Texture2D InitialTexture : register(t0);

[numthreads(1, 1, 1)]
void CSMain(uint3 threadID : SV_DispatchThreadID)
{
    // TODO: Implementation
}