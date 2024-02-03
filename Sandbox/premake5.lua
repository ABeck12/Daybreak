project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir (outputloc .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (outputloc .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}
	
	includedirs
	{
		"../Daybreak/src",
		"../Daybreak/vendor/spdlog/include",
		"../Daybreak/vendor/glad/include",
		"../Daybreak/vendor/glfw/include",
		"../Daybreak/vendor/imgui",
		"../Daybreak/vendor/glm",
		"../Daybreak/vendor/stb_image",
		"../Daybreak/vendor/entt/include",
	}

	links
	{
		"Daybreak",
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
			"DB_RELEASE"
		}
		symbols "On"
		runtime "Release"
		optimize "on"


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