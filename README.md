![DaybreakLogo](Resources/DaybreakLogo.png)
# Daybreak
Daybreak Engine

This is a 2D game engine based of Hazel engine. This uses GLFW and GLAD/OpenGL.


Licenses to check/add => Hazel, Glfw, Glad, ImGui, stb_image, premake
Licenses added via submodules => spdlog


# To Do List
### For Renderer2D:
    Font rendering
    Line rendering
    Framebuffer
    Graphics Context
    Uniform Buffer?
    Shader Library
    Update the OpenGLTexture2D class

### Low Priority:
    Make a weakref system using std::weak_ptr? or maybe std::object_ptr?
    Fix premake files
    Change windowswindow to glwfwindow since glfw is cross platform
    Remove glad dependency for timestep from application.cpp

### Medium Priority:
    Add ability to pass args from sandbox to Daybreak when starting engine. This is stuff like initial window size etc.

### High Priority:
    Add in timing and function profiling 