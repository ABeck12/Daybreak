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
require("vendor/premake/export-compile-commands")

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
		os.remove("CMakeLists.txt")
		os.remove("./Daybreak/vendor/imgui/ImGui.cmake")
		os.remove("./Daybreak/vendor/glfw/GLFW.cmake")
		os.remove("./Daybreak/vendor/box2d/Box2D.cmake")
		os.remove("./Daybreak/vendor/yaml-cpp/yaml-cpp.cmake")
		os.remove("./Daybreak/Daybreak.cmake")
		os.remove("./Sandbox/Sandbox.cmake")
		os.rmdir("./build")
		os.rmdir("./CmakeFiles")
		os.remove("CmakeCache.txt")

		os.remove("compile_commands.json")
		os.rmdir("./compile_commands")
		os.rmdir("./.cache")
		os.rmdir("./.mypy_cache")

		os.remove("**.code-workspace")
		os.rmdir("./Tasks")
		os.rmdir("./Daybreak/.vscode")
		os.rmdir("./Daybreak/vendor/box2d/.vscode")
		os.rmdir("./Daybreak/vendor/glad/.vscode")
		os.rmdir("./Daybreak/vendor/glfw/.vscode")
		os.rmdir("./Daybreak/vendorimgui/.vscode")
		os.rmdir("./Daybreak/vendor/yaml-cpp/.vscode/c_cpp_properties.json")
		os.rmdir("./Daybreak/vendor/msdf-atlas-gen/.vscode")
		os.rmdir("./Daybreak/vendor/msdf-atlas-gen/msdfgen/.vscode")
		os.rmdir("./Daybreak/vendor/msdf-atlas-gen/msdfgen/freetype/.vscode")
		os.rmdir("./Sandbox/.vscode")
		os.rmdir("./DaybreakEditor/.vscode")
		os.rmdir(".vscode")
		os.rmdir(".vs")

		os.remove("Sandbox/src/ScriptInclude.auto.cpp")
		os.remove("Sandbox/src/ScriptInclude.auto.h")

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
IncludeDirs["yaml-cpp"] = "Daybreak/vendor/yaml-cpp/include"

group "Dependencies"
	include "Daybreak/vendor/box2d"
	include "Daybreak/vendor/glfw"
	include "Daybreak/vendor/glad"
	include "Daybreak/vendor/imgui"
	include "Daybreak/vendor/yaml-cpp"
	include "Daybreak/vendor/msdf-atlas-gen"
group ""

include "Daybreak"
-- include "DaybreakEditor"
include "Sandbox"

