#pragma once

#include "ComputeShader.h"
#include "AssetReferenceManager.h"

#include <memory>

struct ID3D11ComputeShader;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;

namespace Engine
{
	class DirectX11ConstantBuffer;

	struct DirectX11ComputeTexture
	{
		AssetRef<Texture> texture;
		uint32_t index;
	};

	struct DirectX11ComputeBuffer
	{
		std::shared_ptr<ComputeBuffer> computeBuffer;
		ID3D11Buffer* buffer;
		uint32_t viewIndex;

		void Deallocate();
	};

	class DirectX11ComputeShader : public ComputeShader
	{
	public:
		explicit DirectX11ComputeShader();
		~DirectX11ComputeShader();

		void Dispatch(uint32_t x, uint32_t y, uint32_t z) override;
		bool IsValid() const override;

		void SetTexture(const AssetRef<Texture>& texture) override;
		void SetBuffer(const std::shared_ptr<ComputeBuffer>& buffer) override;

	protected:
		bool Load(const wchar_t* fileName) override;
		
	private:
		std::vector<DirectX11ComputeBuffer> m_computeBuffers;
		std::vector<ID3D11ShaderResourceView*> m_inputResources;
		std::vector<ID3D11UnorderedAccessView*> m_outputResources;
		std::vector<DirectX11ComputeTexture> m_textures;
		std::vector<ID3D11Buffer*> m_constantBuffers;
		ID3D11ComputeShader* m_computeShader;
	};
}