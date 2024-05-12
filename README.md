# Debug Build Command

```
cmake --build cmake-build-debug
```

# Part E: Texture Scaling

Texture scalind done directly on coords of texture map in the frag shader .: scales both specular and diffues map (prior to sampling)
Edited struct in BaseLitEntityShader.h to allow for texture scale
scaling done in frag shader (make reasonable argument as to why)

- Edited EntityElement.cpp to add func for scaling and the IMGUI
- Baselitentityshader
- Editied EntityRderere.cpp to parse frag shader scaling details
