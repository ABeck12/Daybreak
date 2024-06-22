<!-- ![DaybreakLogo](Resources/DaybreakLogo.png) -->

<img src="Resources/DaybreakLogo.png" alt="drawing" width="300"/>

# Daybreak Engine
This is a 2D and 2.5D game engine based of Hazel engine. I am developing Daybreak in my spare time to learn how to make applications and game engines as well as how to use C++. 

Daybreak is intented to be cross platform eventually but has only been tested with windows. While all dependent libraries should be cross platform  an error will be thrown when trying to compile on mac or linux.

## Setup
Daybreak uses C++ 20 and can be built using premake. Premake binaries are included as well as a python script to make the calls for ease of use (I made this so that I can work with .sln files in vscode. I dont like visual studio). 

- Step 1: Cloning\
Clone recursivly from github to get all required submodules
```git clone https://github.com/ABeck12/Daybreak.git --recurse-submodules```

- Step 2: Build project files\
Generate project or make files using the ```build.py``` script. Call ```python build.py vs``` for Visual Studio .sln files and ```python build.py make``` for makefiles.
NOTE: Currently the makefiles generated from premake do not compile properly!

- Step 3: Compiling and running\
If you aren't using visual studio then calling ```python build.py compile``` will compile either the .sln or makefiles. Calling ```python build.py run``` will run the game. 


## To Do List
### Renderer and Renderer2D:
- Graphics Context class
- Shader Library? This is partly implemented with the additon of the asset manager. But that also needs a rework
- Fix render layers and ordering -> rework to enum class instead of just numbers like physics collider layers?
- Actually use RenderAPI pointer instead of just hardcoding all classes to use opengl implementation. (Low priority)
- Rework camera class such that it is no longer just a mat4 and you can set the view matrix in a better way rather then just setting a new mat4
    - Also includes CameraComponent struct
- Add some framebuffer and renderering pipeline. Render passes
    - Some way to pipe the output from one frame buffer to another that is more automatic
    - This includes actually fixing lighting
- Add framebuffer clear function in class instead of just binding and then calling clear

- Multithreaded renderering
    - Command queue
- Post proscessing. Needs framebuffer and render pipeline/renderpass implementation
    - Bloom
    - Color correction
    - Arbitrary shader execution on final buffer?
- Better performance tracking with # of draw calls, etc.
- Culling
- GPU soring for depth instead of on CPU in scene class
    - Or if using CPU add culling for the sorting
- Swap chain for framebuffers
- Shadows
- Lighting
    - Polygon lights
    - Spot lights
- Paricle system with ParticleComponent
- Pre-compiling of shader binaries

### Physics2D:
- IsColliding function
- Rework static s_ActiveSim pointer into a project class
- Physics Layers -> rework as enums instead of actual numbers
- Rework physics update function in scene class such that we dont have to destroy and recreate the fixtures every frame
- Line colliders?

### Project:
- This should own the physics sim, AssetManager, ScriptableEntity registries

### Scene:
- Multiple of the same components per entity?
    - Needed for colliders and maybe NativeScripts
- Trim down class it feels very bloated right now
- Some way to keep entities loaded between scene transitions
- Circle renderer component? Or add circles to sprite renderer?
- Particle components

### Asset Manager:
- Rework to allow for better loading and unloading
- Unloading of assets once they are not used anymore. Probably happens on scene changes
- Rework animation and controller naming for serializing
- Rework asset manager pipeline to be less manual
    - Perhaps some importer for aesprite or krita?
- Asset packing and binary formats

### Misc:
- Make a weakref system using std::weak_ptr? or maybe std::object_ptr?
- Change windowswindow to glwfwindow since glfw is cross platform
- Go over all code to make sure it is optimized
- Add ability to pass arbitrary args from sandbox to Daybreak when starting engine. This is stuff like initial window size etc.
- Reduce the use of static singleton variables if possible? PhysicsSim2D, ScriptRegistry, and others. 
- Fix controller input for all gamepad types, ie. ps5 and switch (xbox works for now)

### Audio:
- Add support
