# Daybreak
Daybreak Engine

This is a 2D game engine based of Hazel engine. This uses GLFW and GLAD/OpenGL.


Licenses to check/add => Hazel, Glfw, Glad, ImGui, stb_image (if used), premake (if used)
Licenses added via submodules => spdlog


# TODO
For Renderer:
    Font rendering
    Framebuffer
    Graphics Context
    Uniform Buffer?

Update Input classes off of defines and move to enum classes

Low Priority:
    Change implementation of keycodes and mousecodes to use an enum class instead of cpp defines
    Change Layerstack implemtation to match Hazel system to avoid crashing
    Change implementation of imgui layers to match Hazel system in videos

Medium Priority:
    Add ability to pass args from sandbox to Daybreak when starting engine. This is stuff like initial window size etc.