#pragma once

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
struct ID3D11ComputeShader;
struct ID3D11DepthStencilState;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Resource;

enum D3D11_COMPARISON_FUNC;

struct ID3D10Blob;

namespace Engine
{

	enum class DirectX11BufferType
	{
		Type_RWStructuredBuffer,
		Type_StructuredBuffer
	};

	class DirectX11Utils
	{
	public:
		static bool CompileShader(const wchar_t* fileName, const char* entryPoint, const char* model, ID3D10Blob*& blob);
		static ID3D11DepthStencilState* CreateDepthStencilState(const D3D11_COMPARISON_FUNC& comparisonFunc, ID3D11Device* device);
		
		static ID3D11Buffer* CreateStructuredBuffer(ID3D11Device* device, const void* data, size_t stride, 
			uint32_t numElements, DirectX11BufferType bufferType);
		static ID3D11Buffer* CreateConstantBuffer(ID3D11Device* device, const void* data, size_t stride, uint32_t numElements = 1);

		static ID3D11ShaderResourceView* CreateBufferShaderResourceView(ID3D11Device* device, ID3D11Buffer* buffer);
		static ID3D11UnorderedAccessView* CreateBufferUnorderedAccessView(ID3D11Device* device, ID3D11Buffer* buffer);
	};
}