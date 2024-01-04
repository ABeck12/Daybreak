workspace "Daybreak"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		-- "Dist"
	}

outputloc = _WORKING_DIR 
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["glfw"] = "Daybreak/vendor/glfw/include"
IncludeDirs["glad"] = "Daybreak/vendor/glad/include"
IncludeDirs["imgui"] = "Daybreak/vendor/imgui"
IncludeDirs["glm"] = "Daybreak/vendor/glm"

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
		"%{prj.name}/vendor/glm",
		"%{prj.name}/src"
	}

	links
	{
		"glad",
		"glfw",
		"Imgui",
		"opengl32.lib"
	}

	--pchheader "%{prj.name}/src/dbpch.h"
	--pchsource "%{prj.name}/src/hzpch.cpp"

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
		runtime "Debug"

	filter "configurations:Release"
		defines 
		{
			"DB_RELEASE",
			"DB_ENABLE_ASSERTS"
		}
		symbols "On"
		runtime "Release"

	-- filter "configurations:Dist"
	-- 	defines
	-- 	{
	-- 		"DB_DIST",
	-- 	}
	-- 	symbols "On"
	

	-- buildoptions
	-- {
    --     "/MT"
    -- }

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
		"Daybreak/vendor/glm",
	}

	links
	{
		"Daybreak",
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
		runtime "Debug"


	filter "configurations:Release"
		defines 
		{
			"DB_RELEASE"
		}
		symbols "On"
		runtime "Release"

	-- filter "configurations:Dist"
	-- 	defines 
	-- 	{
	-- 		"DB_DIST"
	-- 	}
	-- 	symbols "On"

	-- buildoptions
	-- {
    --     "/MT"
    -- }