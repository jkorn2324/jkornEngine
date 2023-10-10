-- glfw.lua - Build Library

project "glfw"
	kind "StaticLib"
	language "C"
    cppdialect "C++17"
	cdialect "C99"
    location "%{wks.location}/Engine/Libraries/glfw/"

	targetdir "%{wks.location}/Engine/Libraries/Builds/glfw/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	objdir "%{wks.location}/Engine/Libraries/Builds/glfw/Builds-Int/%{cfg.buildcfg}/%{cfg.platform}/"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
	filter { }

	files
	{
        "%{prj.location}/include/GLFW/glfw3.h",
        "%{prj.location}/include/GLFW/glfw3native.h",

        "%{prj.location}/src/internal.h",
        "%{prj.location}/src/platform.h",
        "%{prj.location}/src/mappings.h",
        "%{prj.location}/src/context.c",
        "%{prj.location}/src/init.c",
        "%{prj.location}/src/input.c",
        "%{prj.location}/src/monitor.c",
        "%{prj.location}/src/platform.c",
        "%{prj.location}/src/vulkan.c",
        "%{prj.location}/src/window.c",
        "%{prj.location}/src/egl_context.c",
        "%{prj.location}/src/osmesa_context.c",
        "%{prj.location}/src/null_platform.h",
        "%{prj.location}/src/null_joystick.h",
        "%{prj.location}/src/null_init.c",
    
        "%{prj.location}/src/null_monitor.c",
        "%{prj.location}/src/null_window.c",
        "%{prj.location}/src/null_joystick.c"
	}

	filter { "system:MacOSx" }
		pic "On"
		staticruntime "off"

		files
		{
			"%{prj.location}/src/cocoa_time.h",
			"%{prj.location}/src/cocoa_time.c",

			"%{prj.location}/src/posix_thread.h",
			"%{prj.location}/src/posix_module.c",
			"%{prj.location}/src/posix_thread.c",

			"%{prj.location}/src/cocoa_platform.h",
			"%{prj.location}/src/cocoa_joystick.h",
			"%{prj.location}/src/cocoa_init.m",
			"%{prj.location}/src/cocoa_joystick.m",
			"%{prj.location}/src/cocoa_monitor.m",
			"%{prj.location}/src/cocoa_window.m",
			"%{prj.location}/src/nsgl_context.m"
		}

		links
		{
			"CoreFoundation.framework",
			"Cocoa.framework",
			"IOKit.framework",
			"CoreVideo.framework"
		}

		defines
		{
			"_GLFW_COCOA"
		}

		buildoptions
		{
			"-framework Cocoa",
			"-framework IOKit",
			"-framework CoreFoundation",
			"-framework CoreVideo"
		}

		linkoptions
		{
			"-framework Cocoa",
			"-framework IOKit",
			"-framework CoreFoundation",
			"-framework CoreVideo"
		}

	filter { "system:Linux" }
		pic "On"
		systemversion "latest"
        staticruntime "off"

		files
		{
			"%{prj.location}/src/x11_init.c",
			"%{prj.location}/src/x11_monitor.c",
			"%{prj.location}/src/x11_window.c",
			"%{prj.location}/src/xkb_unicode.c",
			"%{prj.location}/src/posix_time.c",
			"%{prj.location}/src/posix_thread.c",
			"%{prj.location}/src/glx_context.c",
			"%{prj.location}/src/egl_context.c",
			"%{prj.location}/src/osmesa_context.c",
			"%{prj.location}/src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter { "system:Windows" }
		systemversion "latest"
        staticruntime "off"

		files
		{
            "%{prj.location}/src/win32_init.c",
            "%{prj.location}/src/win32_module.c",
            "%{prj.location}/src/win32_joystick.c",
            "%{prj.location}/src/win32_monitor.c",
            "%{prj.location}/src/win32_time.h",
            "%{prj.location}/src/win32_time.c",
            "%{prj.location}/src/win32_thread.h",
            "%{prj.location}/src/win32_thread.c",
            "%{prj.location}/src/win32_window.c",
            "%{prj.location}/src/wgl_context.c",
            "%{prj.location}/src/egl_context.c",
            "%{prj.location}/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
	filter { }