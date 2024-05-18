# Debug Build Command

```
cmake --build cmake-build-debug
```

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

QUESTION: should the pitch yaw calcs be done in shader (DirectionalLightElement.h)
