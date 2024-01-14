# Daybreak
Daybreak Engine

This is a 2D game engine based of Hazel engine. This uses GLFW and GLAD/OpenGL.


Licenses to check/add => Hazel, Glfw, Glad, ImGui, stb_image, premake
Licenses added via submodules => spdlog


# TODO
For Renderer:
    Font rendering
    Line rendering
    Framebuffer
    Graphics Context
    Uniform Buffer?
    Shader Library
    Update the OpenGLTexture2D class

Low Priority:
    Make a weakref system using std::weak_ptr? or maybe std::object_ptr?
    Fix premake files
    Change windowswindow to glwfwindow since glfw is cross platform

Medium Priority:
    Add ability to pass args from sandbox to Daybreak when starting engine. This is stuff like initial window size etc.