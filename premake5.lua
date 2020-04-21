workspace "Optimus"
	architecture "x64"
	startproject "Sandbox"

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
IncludeDir["imgui"] = "Optimus/ThirdParty/imgui"
IncludeDir["Vulkan"] = "Optimus/ThirdParty/VulkanSDK/Vulkan1/Include"
IncludeDir["GLAD"] = "Optimus/ThirdParty/GLAD/include"
IncludeDir["GLM"] = "Optimus/ThirdParty/GLM/glm"
IncludeDir["stb_image"] = "Optimus/ThirdParty/stb_image"


include "Optimus/ThirdParty/GLFW"
include "Optimus/ThirdParty/imgui"
include "Optimus/ThirdParty/GLAD"


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
		
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"ImGui",
		"Optimus/ThirdParty/VulkanSDK/Vulkan1/Lib/vulkan-1.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines
	{
		"OP_PLATFORM_WINDOWS",
		"OP_BUILD_DLL",
		"GLFW_INCLUDE_NONE",
		"NOMINMAX"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	}

	filter "configurations:Debug"
		defines "OP_DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "OP_RELEASE"
		optimize "On"
		buildoptions "/MDd"

	filter "configurations:Distribution"
		defines "OP_DIST"
		optimize "On"
		buildoptions "/MDd"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.GLM}"
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
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "OP_RELEASE"
		optimize "On"
		buildoptions "/MDd"

	filter "configurations:Distribution"
		defines "OP_DIST"
		optimize "On"
		buildoptions "/MDd"