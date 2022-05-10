#include "EnginePCH.h"
#include "DirectX11Utils.h"

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "DirectX11RenderingAPI.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace Engine
{
	bool DirectX11Utils::CompileShader(const wchar_t* fileName, const char* entryPoint, const char* model, ID3D10Blob*& blob)
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* errorBlob = nullptr;
		HRESULT result = D3DCompileFromFile(fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, model, dwShaderFlags, 0, &blob, &errorBlob);
		if (result != S_OK)
		{
			if (errorBlob != nullptr)
			{
				static wchar_t buffer[4096];
				_snwprintf_s(buffer, 4096, _TRUNCATE,
					L"%hs", (char*)errorBlob->GetBufferPointer());
				OutputDebugString(buffer);

#ifdef PLATFORM_WINDOWS
				MessageBox(nullptr, buffer, L"Error", MB_OK);
#endif
				errorBlob->Release();
			}
			return false;
		}
		// Releases the error blob.
		if (errorBlob != nullptr)
		{
			errorBlob->Release();
		}
		return true;
	}
	
	ID3D11DepthStencilState* DirectX11Utils::CreateDepthStencilState(const D3D11_COMPARISON_FUNC& func, ID3D11Device* device)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = func;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xff;
		depthStencilDesc.StencilWriteMask = 0xff;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		ID3D11DepthStencilState* outDepthStencilState;
		device->CreateDepthStencilState(&depthStencilDesc, &outDepthStencilState);
		return outDepthStencilState;
	}

	ID3D11Buffer* DirectX11Utils::CreateStructuredBuffer(ID3D11Device* device,
		const void* data, size_t stride, uint32_t numElements, DirectX11BufferType bufferType)
	{
		ID3D11Buffer* outputBuffer = nullptr;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		switch (bufferType)
		{
		case DirectX11BufferType::Type_RWStructuredBuffer:
		{
			desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			break;
		}
		case DirectX11BufferType::Type_StructuredBuffer:
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			break;
		}
		}

		desc.ByteWidth = (uint32_t)stride * numElements;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = (uint32_t)stride;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = device->CreateBuffer(&desc, nullptr, &outputBuffer);
		DebugAssert(hr == S_OK, "Unable to create Structured Buffer");
		return outputBuffer;
	}

	ID3D11Buffer* DirectX11Utils::CreateConstantBuffer(ID3D11Device* device, const void* data, size_t stride, uint32_t numElements)
	{
		ID3D11Buffer* outBuffer = nullptr;
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.ByteWidth = (UINT)stride * (UINT)numElements;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = data;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT result = device->CreateBuffer(&bufferDesc,
			&subresourceData, &outBuffer);
		DebugAssert(result == S_OK, "Failed to create a constant buffer.");
		return outBuffer;
	}

	ID3D11ShaderResourceView* DirectX11Utils::CreateBufferShaderResourceView(ID3D11Device* device, ID3D11Buffer* buffer)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		buffer->GetDesc(&bufferDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

		ID3D11ShaderResourceView* srv = nullptr;
		HRESULT result = device->CreateShaderResourceView(buffer, &srvDesc, &srv);
		DebugAssert(result == S_OK, "Failed to create buffer shader resource");
		return srv;
	}
	
	ID3D11UnorderedAccessView* DirectX11Utils::CreateBufferUnorderedAccessView(ID3D11Device* device, ID3D11Buffer* buffer)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		buffer->GetDesc(&bufferDesc);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

		ID3D11UnorderedAccessView* uav = nullptr;
		HRESULT result = device->CreateUnorderedAccessView(buffer, &uavDesc, &uav);
		DebugAssert(result == S_OK, "Failed to create unordered access view resource.");
		return uav;
	}
	
	ID3D11Texture2D* DirectX11Utils::CreateTexture2D(ID3D11Device* device, 
		UINT width, UINT height, UINT bindFlags, int format, UINT quality, int textureRWFlags)
	{
		ID3D11Texture2D* texture2D = nullptr;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = (DXGI_FORMAT)format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = quality;

		if (textureRWFlags & CPU_Access_Read)
		{
			desc.Usage = D3D11_USAGE_STAGING;
		}
		else if(textureRWFlags & CPU_Access_Write)
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
		}
		else if (textureRWFlags & GPU_Access_Read)
		{
			if (textureRWFlags & GPU_Access_Write)
			{
				desc.Usage = D3D11_USAGE_DEFAULT;
			}
			else
			{
				desc.Usage = D3D11_USAGE_IMMUTABLE;
			}
		}
		else
		{
			desc.Usage = D3D11_USAGE_DEFAULT;
		}
		desc.BindFlags = bindFlags;
		
		desc.CPUAccessFlags = 0;
		if (textureRWFlags & CPU_Access_Read)
		{
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
		}
		if (textureRWFlags & CPU_Access_Write)
		{
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		}
		desc.MiscFlags = 0;
		HRESULT result = device->CreateTexture2D(&desc, nullptr, &texture2D);
		DebugAssert(result == S_OK, "Failed to create texture2d.");
		return texture2D;
	}
	
	ID3D11ShaderResourceView* DirectX11Utils::CreateTextureShaderResourceView(ID3D11Device* device, ID3D11Texture2D* texture2D, int format)
	{
		ID3D11ShaderResourceView* shaderResource = nullptr;
		HRESULT result;
		if (format != DXGI_FORMAT_UNKNOWN)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
			ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			viewDesc.Texture2D.MipLevels = 1;
			viewDesc.Format = (DXGI_FORMAT)format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			result = device->CreateShaderResourceView(texture2D,
				&viewDesc, &shaderResource);
		}
		else
		{
			result = device->CreateShaderResourceView(texture2D,
				nullptr, &shaderResource);
		}
		DebugAssert(result == S_OK, "Shader Resource failed to be created when creating view target.");
		return shaderResource;
	}
}