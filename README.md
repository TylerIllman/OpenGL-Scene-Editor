# CITSf31003 OpenGl Scene Editor Project

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

# TODO

---

# Report Scratch

### Part E: Texture Scaling

In order to allow the textures applied to elements within scene editor to be scaled, a number of changes had to be made. Firstly, a field had to be added to the `BaseLitEntityMaterial` struct with in the BaseLitEntityShader.h file, which can be seen below. By doing so, each entity now had a vec2 associated with them containing it's `x` and `y` texture scale factor. In order to pass these details between the program and the shaders, a location within the protected material data struct was also added.

```c++
struct BaseLitEntityMaterial {
    glm::vec4 diffuse_tint;
    glm::vec4 specular_tint;
    glm::vec4 ambient_tint;
    float shininess;
    glm::vec2 texture_scale; // Added for texture sclaing
};
```

In order to then make the texture scaling interactive, changes had to be made within the `SceneElement.cpp` file. Specifically, a `DragFloat2` element had to be added within the `...imgui_edit_section(...)` function to allow the user to adjust the scale using the GUI, this can be seen below.

```c++
void EditorScene::LitMaterialComponent::add*material_imgui_edit_section(MasterRenderScene& /\_render_scene*/, const SceneContext& /_scene_context_/) {
bool material_changed = false;
ImGui::Text("Material");

    ...

    material_changed |= ImGui::DragFloat("Ambient Factor", &material.ambient_tint[3], 0.01f, 0.0f, FLT_MAX);
    material_changed |= ImGui::DragFloat("Shininess", &material.shininess, 0.3f, 0.0f, FLT_MAX);

    // Added for texture scaling
    material_changed |= ImGui::DragFloat2("Texture Scale", &material.texture_scale.x, 0.01f, 0, FLT_MAX);

    ...
}
```

Also within the `SceneElement.cpp` file, the functions responsible for loading and saving to the json save files had to be edited to include saving the texture scaling data. These changes had to be made for each of the emissive material components as well.

```C++
void EditorScene::LitMaterialComponent::update_material_from_json(const json& json) {
auto m = json["material"];
material.diffuse_tint = m["diffuse_tint"];
material.specular_tint = m["specular_tint"];
material.ambient_tint = m["ambient_tint"];
material.shininess = m["shininess"];
// ADDED FOR TEXTURE SCALING
material.texture_scale = m["texture_scale"];
}

json EditorScene::LitMaterialComponent::material_into_json() const {
return {"material", {
{"diffuse_tint", material.diffuse_tint},
{"specular_tint", material.specular_tint},
{"ambient_tint", material.ambient_tint},
{"shininess", material.shininess},
// ADDED TO STORE TEXTURE SCALING
{"texture_scale", material.texture_scale}
}};
}
```

Finally, the texutre scale details had to be used to actually scale the textures. This was done within each element types corresponding fragment shaders. To do this the fragment shader had to be adjusted to receive the texture_scale as a uniform vec2. Then to actually scale the texture, simply had to multiply the standard texture coordinates by the texture scale factors. These changes can be seen below. These changes had to be made individually for each of the different element type's corresponding fragment shader files.

```C++
...

uniform vec2 texture_scale;

...

vec2 scaled_coords = frag_in.texture_coordinate \* texture_scale;

...
```
