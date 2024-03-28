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
    Finish framebuffer class and add dynamic amounts of framebuffer attachments
    Graphics Context
    Uniform Buffer?
    Shader Library
    Update the OpenGLTexture2D class
    Render layers and ordering -> rework to enum class instead of just numbers like physics collider layers?
    Circle rendering
    RenderAPI pointer
    Rework camera class such that it is no longer just a mat4 and you can set the view matrix in a better way rather then just setting a new mat4

### For Physics2D:
    IsColliding function
    Rework static s_ActiveSim pointer into a project class
    Physics Layers -> rework as enums instead of actual numbers
    Check for memory leaks from box2d userdata and scene ptr. Do I need to delete scene*? Do box2d userdatas need to be manually deleted?

### Project:
    This should own the physics sim, AssetManager, ScriptableEntity registries

### Scene
    Component and Entity active or not
        Includes rendering
    If I am not going to use C# rework ScriptableEntitys to just be scripts
    Scene saving and loading
        SpriteRenderer.Sprite needs to be reworked. Need to be able to save/load component even when no sprite is defined ie. white sprite default
    Add OnScenePlay and OnSceneStop
    Relationship components
    Multiple components per entity?
        Needed for colliders and maybe NativeScripts
    Render layers and ordering -> rework to enum class instead of just numbers like physics collider layers?

### Low Priority:
    Make a weakref system using std::weak_ptr? or maybe std::object_ptr?
    Fix premake files
    Change windowswindow to glwfwindow since glfw is cross platform
    Add input suppport for gamepads

### Medium Priority:
    Make a asset manager system with Daybreak specific files for images, shaders?, animation sources, sound files?
    Rework audio to not be totally scuffed
    Add ability to pass args from sandbox to Daybreak when starting engine. This is stuff like initial window size etc.

### High Priority:
    Add in timing and function profiling 
    Fix local filepaths when not running exe from vscode