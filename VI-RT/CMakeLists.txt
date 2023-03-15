cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(VI-RT)

set(CMAKE_CXX_STANDARD 17)

if  (WIN32)

else(WIN32) #Linux and Mac
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)
    find_package(GLEW REQUIRED)
endif(WIN32)

# Add main executable

add_executable(main src/main.cpp)

# Add subdirectories
add_library(
        LibCamera
        src/Camera/camera.hpp
        src/Camera/perspective.cpp
        src/Camera/perspective.hpp
)
add_library(
        LibImage
        src/Image/image.cpp
        src/Image/ImagePPM.cpp
        src/Image/ImagePPM.hpp
)
#add_library(
#        LibLight
#        src/Light/AmbientLight.hpp
#        src/Light/light.hpp
#)
add_library(
        LibPrimitive
        src/Primitive/BRDF/BRDF.hpp
        src/Primitive/BRDF/Phong.cpp
        src/Primitive/BRDF/Phong.hpp
        src/Primitive/Geometry/geometry.hpp
        src/Primitive/Geometry/mesh.hpp
        src/Primitive/Geometry/mesh.cpp
        src/Primitive/BB.hpp
        src/Primitive/primitive.hpp
)
add_library(
        LibRenderer
        src/Renderer/renderer.hpp
        src/Renderer/StandardRenderer.cpp
        src/Renderer/StandardRenderer.hpp
)
add_library(
        LibScene
        src/Scene/scene.cpp
        src/Scene/scene.hpp
)
add_library(
        LibShader
        src/Shader/AmbientShader.cpp
        src/Shader/AmbientShader.hpp
        src/Shader/shader.hpp
)
add_library(
        LibUtils
        src/utils/AABB.cpp
        src/utils/RGB.hpp
        src/utils/vector.hpp
)

# Link libraries
target_link_libraries(main LibCamera)
target_link_libraries(main LibPrimitive)
target_link_libraries(main LibScene)


