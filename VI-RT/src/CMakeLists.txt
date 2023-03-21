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

add_executable(main main.cpp)

# Add subdirectories
add_library(
        LibCamera
        Camera/camera.hpp
        Camera/perspective.cpp
        Camera/perspective.hpp
)
add_library(
        LibImage
        Image/image.hpp
        Image/ImagePPM.cpp
        Image/ImagePPM.hpp
)
#add_library(
#        LibLight
#        src/Light/AmbientLight.hpp
#        src/Light/light.hpp
#)
add_library(
        LibPrimitive
        Primitive/BRDF/BRDF.hpp
        Primitive/BRDF/Phong.cpp
        Primitive/BRDF/Phong.hpp
        Primitive/Geometry/geometry.hpp
        Primitive/Geometry/mesh.hpp
        Primitive/Geometry/mesh.cpp
        Primitive/BB.hpp
        Primitive/primitive.hpp
)
add_library(
        LibRenderer
        Renderer/renderer.hpp
        Renderer/StandardRenderer.cpp
        Renderer/StandardRenderer.hpp
)
add_library(
        LibScene
        Scene/scene.cpp
        Scene/scene.hpp
)
add_library(
        LibShader
        Shader/AmbientShader.cpp
        Shader/AmbientShader.hpp
        Shader/shader.hpp
)
add_library(
        LibUtils
        utils/AABB.cpp
        utils/RGB.hpp
        utils/vector.hpp
)

# Link libraries
target_link_libraries(main LibCamera)
target_link_libraries(main LibPrimitive)
target_link_libraries(main LibScene)
target_link_libraries(main LibShader)
target_link_libraries(main LibRenderer)
target_link_libraries(main LibImage)
