#pragma once

#include <cstdint>
#include <sys/_types/_int16_t.h>

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

    namespace Graphics
    {
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
    }
}
