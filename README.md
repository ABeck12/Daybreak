<!-- ![DaybreakLogo](Resources/DaybreakLogo.png) -->

<img src="Resources/DaybreakLogo.png" alt="drawing" width="300"/>

# Daybreak
Daybreak Engine

```git clone https://github.com/ABeck12/Daybreak.git --recurse-submodules```

This is a 2D game engine based of Hazel engine.



Fix pch file in premake

# To Do List
### For Renderer2D:
    Font rendering
    Framebuffer
    Graphics Context
    Uniform Buffer?
    Shader Library
    Update the OpenGLTexture2D class
    Texture atlas

### For Physics2D:
    Raycasts
    IsColliding function
    Adding of forces/impulse

### Low Priority:
    Make a weakref system using std::weak_ptr? or maybe std::object_ptr?
    Fix premake files
    Change windowswindow to glwfwindow since glfw is cross platform

### Medium Priority:
    Make a asset manager system
    Rework audio to not be totally scuffed
    Add ability to pass args from sandbox to Daybreak when starting engine. This is stuff like initial window size etc.

### High Priority:
    Add in timing and function profiling 
    Animation component