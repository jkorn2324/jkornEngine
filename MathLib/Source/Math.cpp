#include "MathPCH.h"
#include "Math.h"

namespace MathLib
{

    float Sqrt(float input)
    {
        return std::sqrtf(input);
    }

    double Sqrt(double input)
    {
        return std::sqrt(input);
    }

    template<typename T>
    T Sqrt(T input)
    {
        using InputType = T;
        ASSERT_IS_ARITHMETIC(InputType);
        return input;
    }

    template<>
    float Sqrt(float input)
    {
        return Sqrt(input);
    }

    template<>
    double Sqrt(double input)
    {
        return Sqrt(input);
    }

    template<typename T>
    T Abs(T input)
    {
        using InputType = T;
        ASSERT_IS_ARITHMETIC(InputType);
        const InputType negativeOne = static_cast<InputType>(-1);
        const InputType zero = static_cast<InputType>(0);
        return input < zero ? input * negativeOne : input;
    }

#if false
    template<>
    float Abs(float input)
    {
        return std::fabsf(input);
    }
#endif

    template<>
    double Abs(double input)
    {
        return std::abs(input);
    }

    float Sin(float input, bool inDegrees)
    {
        float theta = inDegrees ? DEG2RAD * input : input;
        return std::sinf(theta);
    }

    float Cos(float input, bool inDegrees)
    {
        float theta = inDegrees ? DEG2RAD * input : input;
        return std::cosf(theta);
    }

    float Tan(float input, bool inDegrees)
    {
        float theta = inDegrees ? DEG2RAD * input : input;
        return std::tanf(theta);
    }

    float ACos(float input, bool inDegrees)
    {
        float theta = inDegrees ? DEG2RAD * input : input;
        return std::acosf(theta);
    }

    float ATan2(float y, float x)
    {
        return std::atan2f(y, x);
    }

    float ASin(float input, bool inDegrees)
    {
        float theta = inDegrees ? DEG2RAD * input : input;
        return std::asinf(theta);
    }
}
