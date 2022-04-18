# Simple PBR Raytracer 
This raytracer is heavily overengineered to keep application abstraction from rt kernel (even if there no others kernels yet)  

Project still WIP

**Current features**
 * It supports basic scene loading via json  
 * Supports texture loading  
 * Simple PBR materials  

**How to make it work**
1. Compile  
2. Take any scene from `resources/`, rename it to scene.json and place near executable  
3. In application `Scene->Load "scene.json"`  
4. To render anything `Rendrer->Render Image`  
5. To change some raytracer settings `Windows->Rendrer Configuratiion`  
6. To save output `Rendrer->Save "output.png"`  

**Build for linux (GCC/CLANG)**
```
cmake .
make
```
_And your binary will be in project root folder_

**Build for windows (MSVC)**
```
cmake -S . -B build
cmake --build build --config Release
```
_And you binary will be somethere in build folder (depends on configuration, for Release config it will be in build/Release)_

**Screenshots**
![Scene RTIOW GUIApplication screenshot 0](/img/scene_rtiow_guiapplication_0.png)
![Sample scene GUIApplication screenshot 1](/img/sample_scene_guiapplication_1.png)

P.S.  
Project lives in self-hosted git repository so there no other commits other than 'sync'  
RT Kernel based on https://github.com/RayTracing/raytracing.github.io with some additions or another approaches