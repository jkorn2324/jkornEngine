#pragma once

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
struct ID3D11ComputeShader;
struct ID3D11DepthStencilState;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Resource;
struct ID3D11Texture2D;

enum D3D11_COMPARISON_FUNC;

struct ID3D10Blob;

namespace Engine
{

	enum DirectX11TextureRWFlags
	{
		CPU_Access_Read = 1 << 0,
		CPU_Access_Write = 1 << 1,
		GPU_Access_Read = 1 << 2,
		GPU_Access_Write = 1 << 3
	};

	enum class DirectX11BufferType
	{
		Type_RWStructuredBuffer,
		Type_StructuredBuffer
	};

	class DirectX11Utils
	{
	private:
		static const int c_defaultFlags = GPU_Access_Write | GPU_Access_Read;

	public:
		static bool CompileShader(const wchar_t* fileName, const char* entryPoint, const char* model, ID3D10Blob*& blob);
		static ID3D11DepthStencilState* CreateDepthStencilState(const D3D11_COMPARISON_FUNC& comparisonFunc, ID3D11Device* device);
		
		static ID3D11Buffer* CreateStructuredBuffer(ID3D11Device* device, const void* data, size_t stride, 
			uint32_t numElements, DirectX11BufferType bufferType);
		static ID3D11Buffer* CreateConstantBuffer(ID3D11Device* device, const void* data, size_t stride, uint32_t numElements = 1);

		static ID3D11ShaderResourceView* CreateBufferShaderResourceView(ID3D11Device* device, ID3D11Buffer* buffer);
		static ID3D11UnorderedAccessView* CreateBufferUnorderedAccessView(ID3D11Device* device, ID3D11Buffer* buffer);

		static bool LoadTextureFromFile(ID3D11Device* device, const wchar_t* filePath, uint32_t rwFlags,
			ID3D11Resource** resource, ID3D11ShaderResourceView** shaderResourceView, UINT bindFlags = 8);

		static ID3D11Texture2D* CreateTexture2D(ID3D11Device* device, UINT width, UINT height, UINT bindFlags, int format, 
			UINT quality, int textureRWFlags = c_defaultFlags);
		static ID3D11ShaderResourceView* CreateTextureShaderResourceView(ID3D11Device* device, ID3D11Texture2D* texture2D, int format = 0);
	};
}