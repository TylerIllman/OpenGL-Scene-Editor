# OpenGL Scene Editor

This project involves adding functionalty to a simple scene editor program that allows users to create and light elements in a number of fun ways. This document provides details on the necessary information to install and run the scene editor. Details on how certain features were implemented can be found in the Project_Report.pdf file.

# Install & Run

Step 1. Clone the repo to your local device (skip to step 3 if you already have the repo locally).

```
git clone https://github.com/TylerIllman/OpenGL-Scene-Editor
```

Step 2. `cd` into the newly cloned directory.

```
cd OpenGL-Scene-Editor
```

Step 3. Generate the build files.

```
cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
```

Step 4. Run the build command. This can take some time.

```
cmake --build cmake-build-release
```

Step 5. Once the project has been built run the bellow command to open the scene editor

```
./cits3003_project
```

# OS Specifications

This project was built on hardware using Apple M1 Silicon chips and running Mac OS. It should be able to run on most operating systems.

---

# Part E: Texture Scaling

Texture scalind done directly on coords of texture map in the frag shader .: scales both specular and diffues map (prior to sampling)
Edited struct in BaseLitEntityShader.h to allow for texture scale
scaling done in frag shader (make reasonable argument as to why)
ALSO: Had to edit the update material from json and material to json file ins the SceneElement.cpp file to save and load texture scaling

- Edited EntityElement.cpp to add func for scaling and the IMGUI
- Baselitentityshader
- Editied EntityRender.cpp to parse frag shader scaling details

# Part F: Light Attenuation

edited pointLightElement.cpp: added attenuation vars
editied Lights.h: edited struct to allow for attenuation
Baselitentityshader.cpp: added line in setpoints to update attenuation params
lights.glsl

# Part H: Directional Light Source

- In Lights.h duplicated the representation of point light struct and created a new Directional struct. Also edited the LightScene struct store a list of the directional lights
<!-- - Lights.cpp -->
- Added DirectionalLightElement.h to entity_scene dir
- Added DirectionalLightElement.cpp to entity_scene dir
- MasterRenderScene.h added insert and remove functions to Directional Lights
- CMakeLists.txt added the diractional light files to the build
- MasterRenderScene added funcs to remove and insert directional
- Lights.glsl added struct to store diractional light data,
- lights.cpp added vector to store all directional lights
- Basellitentity shader h and cpp
- EditorScene.cpp add dirctional light to json gens
