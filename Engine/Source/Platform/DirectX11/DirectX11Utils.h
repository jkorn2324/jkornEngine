#pragma once

#include "DirectX11FwdDecl.h"
#include "GraphicsFormat.h"

#include <dxgiformat.h>

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

	namespace Utility::DirectX11
	{
		constexpr DXGI_FORMAT ToDXGIFormat(GraphicsFormat graphicsFormat)
		{
			switch (graphicsFormat)
			{
				case GraphicsFormat_RGBA8_Typeless:
					return DXGI_FORMAT_R8G8B8A8_TYPELESS;
				case GraphicsFormat_RGBA8_SInt:
					return DXGI_FORMAT_R8G8B8A8_SINT;
				case GraphicsFormat_RGBA8_UInt:
					return DXGI_FORMAT_R8G8B8A8_UINT;

				case GraphicsFormat_RGBA32_Float:
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
				case GraphicsFormat_RGBA32_Typeless:
					return DXGI_FORMAT_R32G32B32A32_TYPELESS;
				case GraphicsFormat_RGBA32_SInt:
					return DXGI_FORMAT_R32G32B32A32_SINT;
				case GraphicsFormat_RGBA32_UInt:
					return DXGI_FORMAT_R32G32B32A32_UINT;

				case GraphicsFormat_SInt32:
					return DXGI_FORMAT_R32_SINT;
				case GraphicsFormat_UInt32:
					return DXGI_FORMAT_R32_UINT;
				case GraphicsFormat_Float32:
					return DXGI_FORMAT_R32_FLOAT;
				case GraphicsFormat_Typeless32:
					return DXGI_FORMAT_R32_TYPELESS;

				case GraphicsFormat_SInt16:
					return DXGI_FORMAT_R16_SINT;
				case GraphicsFormat_UInt16:
					return DXGI_FORMAT_R16_UINT;
				case GraphicsFormat_Typeless16:
					return DXGI_FORMAT_R16_TYPELESS;
					
				case GraphicsFormat_SInt8:
					return DXGI_FORMAT_R8_SINT;
				case GraphicsFormat_UInt8:
					return DXGI_FORMAT_R8_UINT;
				case GraphicsFormat_Typeless8:
					return DXGI_FORMAT_R8_TYPELESS;

				case GraphicsFormat_D24UNorm_S8UInt:
					return DXGI_FORMAT_D24_UNORM_S8_UINT;
				case GraphicsFormat_D32Float_S8UInt:
					return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
				case GraphicsFormat_D32Float:
					return DXGI_FORMAT_D32_FLOAT;
			}
			return DXGI_FORMAT_UNKNOWN;
		}

		constexpr GraphicsFormat FromDXGIFormat(DXGI_FORMAT dxgiFormat)
		{
			switch (dxgiFormat)
			{
				case DXGI_FORMAT_R8G8B8A8_TYPELESS:
					return GraphicsFormat_RGBA8_Typeless;
				case DXGI_FORMAT_R8G8B8A8_SINT:
					return GraphicsFormat_RGBA8_SInt;
				case DXGI_FORMAT_R8G8B8A8_UINT:
					return GraphicsFormat_RGBA8_UInt;

				case DXGI_FORMAT_R32G32B32A32_FLOAT:
					return GraphicsFormat_RGBA32_Float;
				case DXGI_FORMAT_R32G32B32A32_TYPELESS:
					return GraphicsFormat_RGBA32_Typeless;
				case DXGI_FORMAT_R32G32B32A32_SINT:
					return GraphicsFormat_RGBA32_SInt;
				case DXGI_FORMAT_R32G32B32A32_UINT:
					return GraphicsFormat_RGBA32_UInt;

				case DXGI_FORMAT_R32_SINT:
					return GraphicsFormat_SInt32;
				case DXGI_FORMAT_R32_UINT:
					return GraphicsFormat_UInt32;
				case DXGI_FORMAT_R32_FLOAT:
					return GraphicsFormat_Float32;
				case DXGI_FORMAT_R32_TYPELESS:
					return GraphicsFormat_Typeless32;

				case DXGI_FORMAT_D24_UNORM_S8_UINT:
					return GraphicsFormat_D24UNorm_S8UInt;
				case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
					return GraphicsFormat_D32Float_S8UInt;
				case DXGI_FORMAT_D32_FLOAT:
					return GraphicsFormat_D32Float;
			}
			return GraphicsFormat_Unknown;
		}

		constexpr DXGI_FORMAT ToDXGIFormat(DepthFormat depthFormat)
		{
			return ToDXGIFormat(Graphics::ToGraphicsFormat(depthFormat));
		}

		constexpr DXGI_FORMAT ToDXGIFormat(ColorFormat colorFormat)
		{
			return ToDXGIFormat(Graphics::ToGraphicsFormat(colorFormat));
		}
	}

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