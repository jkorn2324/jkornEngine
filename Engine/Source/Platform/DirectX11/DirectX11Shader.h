#pragma once

#include "Shader.h"
#include <d3d11.h>

namespace Engine
{

	class DirectX11Shader : public Shader
	{
	public:
		explicit DirectX11Shader();
		explicit DirectX11Shader(const BufferLayout& bufferLayout);
		~DirectX11Shader();

		bool IsValid() const override;
		void Bind() const;

	protected:
		bool LoadFromFile_Internal(const wchar_t* shaderPath) override;

	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_inputLayout;
	};
}