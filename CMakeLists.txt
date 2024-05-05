# Initialise cmake script
cmake_minimum_required(VERSION 3.12)
project(cits3003_project)

set(CMAKE_CXX_STANDARD 17)

# Define the executable
add_executable(cits3003_project
        src/main.cpp
        src/rendering/resources/ModelHandle.h
        src/rendering/resources/MeshHierarchy.cpp
        src/rendering/resources/TextureLoader.cpp
        src/rendering/resources/TextureHandle.cpp
        src/rendering/resources/ModelLoader.cpp
        src/rendering/memory/UniformBufferArray.h
        src/rendering/scene/MasterRenderScene.cpp
        src/rendering/scene/Animator.cpp
        src/rendering/scene/RenderedEntity.h
        src/rendering/scene/RenderScene.h
        src/rendering/scene/GlobalData.h
        src/rendering/scene/Lights.cpp
        src/rendering/renders/MasterRenderer.cpp
        src/rendering/renders/shaders/ShaderInterface.cpp
        src/rendering/renders/shaders/BaseEntityShader.cpp
        src/rendering/renders/shaders/BaseLitEntityShader.cpp
        src/rendering/renders/EntityRenderer.cpp
        src/rendering/renders/AnimatedEntityRenderer.cpp
        src/rendering/renders/EmissiveEntityRenderer.cpp
        src/rendering/cameras/CameraInterface.h
        src/rendering/cameras/PanningCamera.cpp
        src/rendering/cameras/FlyingCamera.cpp
        src/rendering/imgui/ImGuiImpl.cpp
        src/rendering/imgui/ImGuiManager.cpp
        src/system_interfaces/Window.cpp
        src/system_interfaces/WindowManager.cpp
        src/utility/PerformanceCounter.cpp
        src/utility/Math.h
        src/utility/OpenGL.cpp
        src/utility/JsonHelper.h
        src/utility/HelperTypes.h
        src/utility/SyncManager.cpp
        src/scene/SceneInterface.h
        src/scene/BasicStaticScene.cpp
        src/scene/BasicStaticScene.h
        src/scene/EditorScene.cpp
        src/scene/EditorScene.h
        src/scene/SceneManager.cpp
        src/scene/SceneManager.h
        src/scene/editor_scene/SceneElement.h
        src/scene/editor_scene/EntityElement.cpp
        src/scene/editor_scene/AnimatedEntityElement.cpp
        src/scene/editor_scene/PointLightElement.cpp
        src/scene/editor_scene/GroupElement.cpp
        src/scene/editor_scene/EmissiveEntityElement.cpp
        src/scene/editor_scene/SceneElement.cpp
)

target_include_directories(cits3003_project PRIVATE src)

# You can uncomment these lines to enable more warnings and enabling warnings as errors, if you want that.
# if(MSVC)
#     target_compile_options(cits3003_project PRIVATE "/W3" "/WX" "/D_CRT_SECURE_NO_WARNINGS")
# else()
#     target_compile_options(cits3003_project PRIVATE "-Wextra" "-Wpedantic" "-Wall" "-Werror" "-Wno-deprecated-declarations")
# endif()

# Build and link libraries


# GLAD
add_subdirectory(lib/glad)
# end GLAD


# GLFW
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

if (APPLE)
    # "-framework OpenGL"
    target_link_libraries(cits3003_project "-framework Cocoa" "-framework IOKit")
endif()
# end GLFW


# glm
add_subdirectory(lib/glm)
#end glm


# assimp
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
add_subdirectory(lib/assimp)
# end assimp


# stb
add_subdirectory(lib/stb)
#end stb


# ImGui
add_subdirectory(lib/imgui)
#end ImGui


# JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(lib/json)
#end JSON


# tinyfiledialogs
add_subdirectory(lib/tinyfiledialogs)
#end tinyfiledialogs


target_link_libraries(cits3003_project glfw glad glm assimp stb imgui nlohmann_json::nlohmann_json tinyfiledialogs)


# Copy executable post build
add_custom_command(TARGET cits3003_project
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cits3003_project> ${CMAKE_SOURCE_DIR})


if (WIN32)
    # Due to symbolic links requiring higher privileges on Windows, manually create Junctions instead

    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/res" _dstResDir)
    file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/res" _srcResDir)

    # Link the res dir into where the build puts the exe
    add_custom_command(TARGET cits3003_project
            POST_BUILD
            COMMAND cmd.exe /c if not exist "${_dstResDir}" mklink /J "${_dstResDir}" "${_srcResDir}")

    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/config" _dstConfigDir)
    file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/config" _srcConfigDir)

    # Link the config dir into where the build puts the exe
    add_custom_command(TARGET cits3003_project
            POST_BUILD
            COMMAND cmd.exe /c if not exist "${_dstConfigDir}" mklink /J "${_dstConfigDir}" "${_srcConfigDir}")
else()
    # Link the res dir into where the build puts the exe
    add_custom_command(TARGET cits3003_project
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)

    # Link the config dir into where the build puts the exe
    add_custom_command(TARGET cits3003_project
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/config ${CMAKE_BINARY_DIR}/config)
endif()