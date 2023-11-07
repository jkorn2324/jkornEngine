#pragma once

#include <cstdint>

namespace Engine
{
    /**
     * @brief Creates a graphics format type that can be used.
     */
    enum GraphicsFormat
    {
		GraphicsFormat_Unknown,
        
		// 1 Byte Per Color Channel, 4 Bytes Total, Unknown Type, Red,Green,Blue,Alpha
		GraphicsFormat_RGBA8_Typeless,
        GraphicsFormat_RGBA8_SInt,
        GraphicsFormat_RGBA8_UInt,
        
		// 1 Float Per Color Channel, 16 Bytes Total, 4 Floats
		GraphicsFormat_RGBA32_Float,
        GraphicsFormat_RGBA32_SInt,
        GraphicsFormat_RGBA32_UInt,
        GraphicsFormat_RGBA32_Typeless,
        
		// 4 Bytes Per Color Channel, 16 Bytes Total, 4 Floats
		GraphicsFormat_ARGB32_Float = GraphicsFormat_RGBA32_Float,
        GraphicsFormat_ARGB32_SInt = GraphicsFormat_RGBA32_SInt,
        GraphicsFormat_ARGB32_UInt = GraphicsFormat_RGBA32_UInt,
        GraphicsFormat_ARGB32_Typeless = GraphicsFormat_RGBA32_Typeless,
        
		// 32 Bit Representations
		GraphicsFormat_SInt32,
        GraphicsFormat_UInt32,
		GraphicsFormat_Float32,
        GraphicsFormat_Typeless32,

		// 16 Bit Representations
		GraphicsFormat_SInt16,
        GraphicsFormat_UInt16,
        GraphicsFormat_Typeless16,
		
        // 8 Bit Representations,
		GraphicsFormat_SInt8,
        GraphicsFormat_UInt8,
        GraphicsFormat_Typeless8,

        // Depth + Stencil Representations,

        // Depth = 24 Bits, Stencil = 8 Bits
        GraphicsFormat_D24UNorm_S8UInt,
        // Depth = 32 Bits, Stencil = 8 Bits
        GraphicsFormat_D32Float_S8UInt,
        // Depth = 32 Bits
        GraphicsFormat_D32Float
    };

    /**
     * The Graphics Color Format. 
     */
    enum ColorFormat
    {
        ColorFormat_Unknown,
        ColorFormat_None = ColorFormat_Unknown,

        ColorFormat_RGBA32_Float,
        ColorFormat_RGBA32_UInt,
        ColorFormat_RGBA32_SInt,

        ColorFormat_ARGB32_Float = ColorFormat_RGBA32_Float,
        ColorFormat_ARGB32_UInt = ColorFormat_RGBA32_UInt,
        ColorFormat_ARGB32_SInt = ColorFormat_RGBA32_SInt,

        ColorFormat_RGBA8_UInt,
        ColorFormat_RGBA8_SInt,

        ColorFormat_UInt32,
        ColorFormat_SInt32,
        ColorFormat_Float32,

        ColorFormat_UInt16,
        ColorFormat_SInt16,

        ColorFormat_UInt8,
        ColorFormat_SInt8
    };

    enum DepthFormat
    {
        DepthFormat_Unknown,
        DepthFormat_None = DepthFormat_Unknown,

        DepthFormat_D24UNorm_S8UInt,
        DepthFormat_D32Float_S8UInt,
        DepthFormat_D32Float
    };

    namespace Graphics
    {

        constexpr GraphicsFormat ToGraphicsFormat(ColorFormat colorFormat)
        {
            switch (colorFormat)
            {
                case ColorFormat_RGBA32_Float:
                    return GraphicsFormat_RGBA32_Float;
                case ColorFormat_RGBA32_UInt:
                    return GraphicsFormat_RGBA32_UInt;
                case ColorFormat_RGBA32_SInt:
                    return GraphicsFormat_RGBA32_SInt;

                case ColorFormat_RGBA8_UInt:
                    return GraphicsFormat_RGBA8_UInt;
                case ColorFormat_RGBA8_SInt:
                    return GraphicsFormat_RGBA8_SInt;

                case ColorFormat_UInt32:
                    return GraphicsFormat_UInt32;
                case ColorFormat_SInt32:
                    return GraphicsFormat_SInt32;
                case ColorFormat_Float32:
                    return GraphicsFormat_Float32;

                case ColorFormat_UInt16:
                    return GraphicsFormat_UInt16;
                case ColorFormat_SInt16:
                    return GraphicsFormat_SInt16;

                case ColorFormat_UInt8:
                    return GraphicsFormat_UInt8;
                case ColorFormat_SInt8:
                    return GraphicsFormat_SInt8;
            }
            return GraphicsFormat_Unknown;
        }

        constexpr GraphicsFormat ToGraphicsFormat(DepthFormat depthFormat)
        {
            switch (depthFormat)
            {
                case DepthFormat_D24UNorm_S8UInt:
                    return GraphicsFormat_D24UNorm_S8UInt;
                case DepthFormat_D32Float_S8UInt:
                    return GraphicsFormat_D32Float_S8UInt;
                case DepthFormat_D32Float:
                    return GraphicsFormat_D32Float;
            }
            return GraphicsFormat_Unknown;
        }

        constexpr DepthFormat GetSystemDefaultDepthFormat()
        {
            // Gets the system default depth format.
            return DepthFormat_D24UNorm_S8UInt;
        }

        constexpr size_t SizeOfFormat(GraphicsFormat format)
        {
            switch (format)
            {
                // Size of an integer.
                case GraphicsFormat_RGBA8_SInt:
                case GraphicsFormat_RGBA8_UInt:
                case GraphicsFormat_RGBA8_Typeless:
                {
                    return sizeof(int32_t);
                }

                case GraphicsFormat_RGBA32_SInt:
                case GraphicsFormat_RGBA32_UInt:
                case GraphicsFormat_RGBA32_Float:
                case GraphicsFormat_RGBA32_Typeless:
                {
                    return sizeof(uint32_t) * 4;
                }
                case GraphicsFormat_SInt32:
                case GraphicsFormat_UInt32:
                case GraphicsFormat_Float32:
                case GraphicsFormat_Typeless32:
                {
                    return sizeof(uint32_t);
                }
                case GraphicsFormat_SInt16:
                case GraphicsFormat_UInt16:
                case GraphicsFormat_Typeless16:
                {
                    return sizeof(uint16_t);
                }
                case GraphicsFormat_SInt8:
                case GraphicsFormat_UInt8:
                case GraphicsFormat_Typeless8:
                {
                    return sizeof(uint8_t);
                }

                case GraphicsFormat_D32Float:
                case GraphicsFormat_D24UNorm_S8UInt:
                {
                    return sizeof(uint32_t);
                }
                // 64 Bits bc it uses 40 bits, 24 are unused.
                case GraphicsFormat_D32Float_S8UInt:
                {
                    return sizeof(float) + sizeof(uint32_t);
                }
                case GraphicsFormat_Unknown:
                {
                    return 0;
                }
            }
            return 0;
        }

        constexpr bool IsTypeless(GraphicsFormat format)
        {
            switch (format)
            {
                case GraphicsFormat_RGBA32_Typeless:
                case GraphicsFormat_RGBA8_Typeless:
                case GraphicsFormat_Typeless32:
                case GraphicsFormat_Typeless16:
                case GraphicsFormat_Typeless8:
                    return true;
            }
            return false;
        }

        constexpr bool IsValidDepthFormat(GraphicsFormat format)
        {
            return format == GraphicsFormat_D24UNorm_S8UInt
                || format == GraphicsFormat_D32Float_S8UInt
                || format == GraphicsFormat_D32Float;
        }

        constexpr bool IsValidStencilFormat(GraphicsFormat format)
        {
            return format == GraphicsFormat_D24UNorm_S8UInt
                || format == GraphicsFormat_D32Float_S8UInt;
        }

        constexpr GraphicsFormat GetDefaultDepthStencilFormat()
        {
            // TODO: Implementation for multiple platforms.
            return GraphicsFormat_D24UNorm_S8UInt;
        }
    }
}
