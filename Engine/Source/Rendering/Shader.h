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

		static Shader* StaticLoad(const std::wstring& fileName, const BufferLayout& bufferLayout);

	private:
		bool Load(const wchar_t* fileName, const BufferLayout& bufferLayout);
		void Bind() const;

	private:
		ID3D11VertexShader* m_vertexShader = nullptr;
		ID3D11PixelShader* m_pixelShader = nullptr;
		ID3D11InputLayout* m_inputLayout = nullptr;

		friend class GraphicsRenderer;
	};
}