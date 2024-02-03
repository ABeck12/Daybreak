workspace "Daybreak"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		-- "Dist"
	}

require("vendor/premake/premake-vscode")

outputloc = _WORKING_DIR 
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

newaction
{
	trigger = "clean",
	description = "Remove all binaries, intermediate binaries, vs files, and makefiles",
	execute = function()
		print("Removing binaries")
		os.rmdir("./bin")
		print("Removing intermediate binaries")
		os.rmdir("./bin-int")
		print("Removing project files")
		os.remove("**.sln")
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")
		os.remove("**.vcxproj.user")
		os.remove("**Makefile")

		os.remove("**.code-workspace")
		os.rmdir("./Tasks")
		os.rmdir("./Daybreak/.vscode")
		os.rmdir("./Daybreak/box2d/.vscode")
		os.rmdir("./Daybreak/glad/.vscode")
		os.rmdir("./Daybreak/glfw/.vscode")
		os.rmdir("./Daybreak/imgui/.vscode")
		os.rmdir("./Sandbox/.vscode")
		print("Done")
	end
}

IncludeDirs = {}
IncludeDirs["box2d"] = "Daybreak/vendor/box2d/include"
IncludeDirs["glfw"] = "Daybreak/vendor/glfw/include"
IncludeDirs["glad"] = "Daybreak/vendor/glad/include"
IncludeDirs["imgui"] = "Daybreak/vendor/imgui"
IncludeDirs["glm"] = "Daybreak/vendor/glm"
IncludeDirs["stb_image"] = "Daybreak/vendor/stb_image"
IncludeDirs["entt"] = "Daybreak/vendor/entt/include"

-- include "Daybreak/vendor/box2d"
-- include "Daybreak/vendor/glfw"
-- include "Daybreak/vendor/glad"
-- include "Daybreak/vendor/imgui"

group "Dependencies"
	include "Daybreak/vendor/box2d"
	include "Daybreak/vendor/glfw"
	include "Daybreak/vendor/glad"
	include "Daybreak/vendor/imgui"
group ""

include "Daybreak"
include "Sandbox"

