project "Daybreak"
	kind "StaticLib"
	language "C++"

	targetdir (outputloc .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (outputloc .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		-- "%{prj.name}/src/**.hpp",
        "vendor/stb_image/stb_image.h",
        "vendor/stb_image/stb_image.cpp",
	}
	
	includedirs
	{
		"vendor/box2d/include",
		"vendor/spdlog/include",
		"vendor/glad/include",
		"vendor/glfw/include",
		"vendor/imgui",
		"vendor/glm",
		"vendor/stb_image",
		"vendor/entt/include",
		"src"
	}

	links
	{
		"Box2d",
		"glad",
		"glfw",
		"Imgui",
		"opengl32.lib"
	}

	pchheader "dbpch.h"
	pchsource "src/dbpch.cpp"

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
		optimize "on"


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

