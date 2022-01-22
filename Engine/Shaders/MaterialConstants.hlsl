#pragma pack_matrix(row_major)

cbuffer MaterialConstants : register(b2)
{
    uint c_materialFlags;
};

#define MaterialFlag_None = 0
#define MaterialFlag_DefaultTexture 1 << 0

#define HAS_MATERIAL_FLAG(flag) c_materialFlags & flag