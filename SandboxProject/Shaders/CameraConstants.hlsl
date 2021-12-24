#pragma pack_matrix(row_major)

cbuffer CameraConstants : register(b0)
{
    float4x4 c_viewProjection;
    float3 c_cameraPosition;
};