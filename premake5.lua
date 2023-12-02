workspace "Daybreak"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["glfw"] = "Daybreak/vendor/glfw/include"
IncludeDirs["glad"] = "Daybreak/vendor/glad/include"
IncludeDirs["imgui"] = "Daybreak/vendor/imgui"

include "Daybreak/vendor/glfw"
include "Daybreak/vendor/glad"
include "Daybreak/vendor/imgui"

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
	-- TEMPORARY
	removefiles
	{ 
		"%{prj.name}/src/TestRenderer/**.cpp", "%{prj.name}/src/TestRenderer/**.h",
		"%{prj.name}/src/Daybreak/Math/**.cpp", "%{prj.name}/src/Daybreak/Math/**.h",
	}
	
	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/vendor/imgui",
		-- "%{IncludeDir["glfw"]}",
		-- "Daybreak/vendor/glfw/include",
		"%{prj.name}/src"
	}

	links
	{
		"glad",
		"glfw",
		"Imgui",
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
			"DB_RELEASE",
			"DB_ENABLE_ASSERTS"
		}
		symbols "On"

	-- filter "configurations:Dist"
	-- 	defines
	-- 	{
	-- 		"DB_DIST",
	-- 	}
	

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
		"Daybreak/vendor/imgui",
	}

	links
	{
		"Daybreak",
		-- "imgui",
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