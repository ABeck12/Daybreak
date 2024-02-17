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
    Render layers and ordering
    Circle rendering
    RenderAPI pointer

### For Physics2D:
    IsColliding function
    Adding of forces/impulse
    Physics Layers
    Circle colliders

### Scene
    Component and Entity active or not
        Includes rendering and physics too
    Scene saving and loading
    Add OnScenePlay and OnSceneStop
    Resolve memory leak from not having NativeScriptComponents delete their instances in on scene stop
    Relationship components
    Multiple components per entity?
        Needed for colliders
    Circle collidercomponent

### Low Priority:
    Make a weakref system using std::weak_ptr? or maybe std::object_ptr?
    Fix premake files
    Change windowswindow to glwfwindow since glfw is cross platform

### Medium Priority:
    Make a asset manager system with Daybreak specific files for images, shaders?, animation sources, sound files?
    Rework audio to not be totally scuffed
    Add ability to pass args from sandbox to Daybreak when starting engine. This is stuff like initial window size etc.

### High Priority:
    Add in timing and function profiling 
    Animation component