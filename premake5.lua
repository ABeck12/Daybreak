workspace "Daybreak"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["glfw"] = "Daybreak/vendor/glfw/include"
IncludeDirs["glad"] = "Daybreak/vendor/glad/include"

include "Daybreak/vendor/glfw"
include "Daybreak/vendor/glad"

project "Daybreak"
	location "Daybreak"
	kind "StaticLib"
	language "C++"

	targetdir ( "bin/" .. outputdir .. "/%{prj.name}" )
	objdir ( "bin-int/" .. outputdir .. "/%{prj.name}" )

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/glfw/include",
		-- "%{IncludeDir["glfw"]}",
		-- "Daybreak/vendor/glfw/include",
		"%{prj.name}/src"
	}

	links
	{
		"glad",
		"glfw",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DB_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines 
		{
			"DB_DEBUG",
			"DB_ENABLE_ASSERTS"
		}
		symbols "On"


	filter "configurations:Release"
		defines 
		{
			"DB_RELEASE"
		}
		symbols "On"

	buildoptions
	{
        "/MT"
    }

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ( "bin/" .. outputdir .. "/%{prj.name}" )
	objdir ( "bin-int/" .. outputdir .. "/%{prj.name}" )

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Daybreak/src",
		"Daybreak/vendor/spdlog/include",
		"Daybreak/vendor/glad/include",
		"Daybreak/vendor/glfw/include",
	}

	links
	{
		"Daybreak"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DB_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines 
		{
			"DB_DEBUG",
			"DB_ENABLE_ASSERTS"
		}
		symbols "On"


	filter "configurations:Release"
		defines 
		{
			"DB_RELEASE"
		}
		symbols "On"