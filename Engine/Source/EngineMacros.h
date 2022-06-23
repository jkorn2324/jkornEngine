#pragma once

namespace Engine
{

#define ENABLE_THREADING 0

// Default Macros.
#define NAMEOF(v) #v

// Binds functions to a std::function based on its name.
#define BIND_STATIC_FUNCTION(func) [](auto&&...args) -> decltype(auto) { return func(std::forward<decltype(args)>(args)...); }
#define BIND_CLASS_FUNCTION(func) [this](auto&&...args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

}