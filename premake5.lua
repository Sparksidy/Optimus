workspace "Optimus"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Optimus"
	location "Optimus"
	kind "SharedLib"
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
		"%{prj.name}/ThirdParty/spdlog/include"
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
		"Optimus/src"
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