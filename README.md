# Debug Build Command

```
cmake --build cmake-build-debug
```

# Part E: Texture Scaling

Texture scalind done directly on coords of texture map in the frag shader .: scales both specular and diffues map (prior to sampling)
Edited struct in BaseLitEntityShader.h to allow for texture scale

- Edited EntityElement.cpp to add func for scaling and the IMGUI
- Baselitentityshader
- Editied EntityRderere.cpp to parse frag shader scaling details
