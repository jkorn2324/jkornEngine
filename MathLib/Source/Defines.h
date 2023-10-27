#pragma once

// The defines for the Math Library.

#define ASSERT_IS_ARITHMETIC(T) static_assert(std::is_arithmetic<T>::value, "The (T) value must be a number or arithmetic value.")
