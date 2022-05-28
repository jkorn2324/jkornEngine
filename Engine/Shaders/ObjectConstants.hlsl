#pragma pack_matrix(row_major)

cbuffer PerObjectConstants : register(b1)
{
    float4x4 c_objectToWorld;
    int c_entityID;
};

static int GetEntityID()
{
    return c_entityID + 1;
}