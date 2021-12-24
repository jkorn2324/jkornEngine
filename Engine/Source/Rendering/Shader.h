#pragma once

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

namespace Engine
{
	class BufferLayout;

	class Shader
	{
	public:
		explicit Shader();
		~Shader();

		bool IsValid() const;
		bool Load(const wchar_t* fileName, const BufferLayout& bufferLayout);

	private:
		void Bind() const;

	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_inputLayout;

		friend class GraphicsRenderer;
	};
}