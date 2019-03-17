workspace "Optimus"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Dependencies in a IncludeDir structure
IncludeDir = {}
IncludeDir["GLFW"] = "Optimus/ThirdParty/GLFW/include"

include "Optimus/ThirdParty/GLFW"

project "Optimus"
	location "Optimus"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Optimus/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/ThirdParty/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines
	{
		"OP_PLATFORM_WINDOWS",
		"OP_BUILD_DLL"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	}

	filter "configurations:Debug"
		defines "OP_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "OP_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "OP_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Optimus/ThirdParty/spdlog/include",
		"Optimus/src",
		"%{IncludeDir.GLFW}"
	}
	
	links
	{
		"Optimus"
	}


	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"OP_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "OP_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "OP_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "OP_DIST"
		optimize "On"