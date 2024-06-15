project "Daybreak"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir (outputloc .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (outputloc .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
        "vendor/stb_image/stb_image.cpp",
        "vendor/miniaudio/miniaudio.cpp",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"
	}
	
	includedirs
	{
		"vendor/box2d/include",
		"vendor/spdlog/include",
		"vendor/glad/include",
		"vendor/glfw/include",
		"vendor/imgui",
		"vendor/imguizmo",
		"vendor/glm",
		"vendor/stb_image",
		"vendor/entt/include",
		"vendor/miniaudio",
		"vendor/yaml-cpp/include",
		"vendor/msdf-atlas-gen/msdfgen",
		"vendor/msdf-atlas-gen/msdfgen/include",
		"vendor/msdf-atlas-gen/msdf-atlas-gen",
		"src"
	}

	links
	{
		"Box2d",
		"glad",
		"glfw",
		"Imgui",
		"yaml-cpp",
		"msdf-atlas-gen",
		"opengl32.lib"
	}

	pchheader "dbpch.h"
	pchsource "src/dbpch.cpp"

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
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
		optimize "on"