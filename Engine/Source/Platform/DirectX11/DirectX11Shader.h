#pragma once

#include "Shader.h"
#include <d3d11.h>

namespace Engine
{

	class DirectX11Shader : public Shader
	{
	public:
		explicit DirectX11Shader();
		~DirectX11Shader();

		bool IsValid() const override;

	protected:
		void Bind() const;
		bool Load(const wchar_t* shaderPath, const class BufferLayout& bufferLayout) override;

	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_inputLayout;
	};
}