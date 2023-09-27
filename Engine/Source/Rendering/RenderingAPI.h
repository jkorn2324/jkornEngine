#pragma once

namespace MathLib
{
	class Vector4;
}

namespace Engine
{

	enum class RenderingAPIType
	{
		NONE,
        // The directx graphics rendering api.
		DIRECTX11,
        // The metal graphics rendering api.
        METAL
	};

	class RenderingAPI
	{
	public:
		virtual ~RenderingAPI() { }

		virtual bool Initialize(class Window* window) =0;
		virtual void SetViewport(float x, float y, float width, float height)=0;
		virtual void SetResolution(std::uint32_t width, std::uint32_t height)=0;
		virtual void SetClearColor(const MathLib::Vector4& vector4)=0;
		virtual void SwapBuffers()=0;
		virtual void Clear()=0;
		
		virtual void Draw(class VertexArray* vertexArray) =0;
		virtual void Draw(class VertexBuffer* vertexBuffer,
			class IndexBuffer* indexBuffer = nullptr)=0;

		virtual void SetWireframe(bool wireframeMode)=0;
		virtual bool IsWireframe() const=0;

		virtual void ClearTexture(uint32_t slot)=0;

		virtual std::uint32_t GetWidth() const=0;
		virtual std::uint32_t GetHeight() const=0;

	public:
		static inline constexpr RenderingAPIType GetRenderingAPIType()
		{
#if defined(GRAPHICS_API_DIRECTX11)
			return RenderingAPIType::DIRECTX11;
#elif defined(GRAPHICS_API_METAL)
			return RenderingAPIType::METAL;
#else
			return RenderingAPIType::NONE
#endif
		}

		static RenderingAPI* Create();
	};
}
