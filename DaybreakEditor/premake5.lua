project "DaybreakEditor"
    kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir (outputloc .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (outputloc .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	removefiles 
	{ 
		"src/EditorLayer.cpp",
		"src/EditorLayer.h",
		"src/Pannels/HierarchyPannel.cpp",
		"src/Pannels/HierarchyPannel.h",

	}
	
	includedirs
	{
		"../Daybreak/src",
		"../Daybreak/vendor/box2d/include",
		"../Daybreak/vendor/spdlog/include",
		"../Daybreak/vendor/glad/include",
		"../Daybreak/vendor/glfw/include",
		"../Daybreak/vendor/imgui",
		"../Daybreak/vendor/imguizmo",
		"../Daybreak/vendor/glm",
		"../Daybreak/vendor/stb_image",
		"../Daybreak/vendor/entt/include",
		"../Daybreak/vendor/miniaudio",
		"../Daybreak/vendor/yaml-cpp/include",
		"src"
	}

	links
	{
        "Daybreak"
	}
    
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
