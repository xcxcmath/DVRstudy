project(DVRstudy)

set(CMAKE_CXX_FLAGS -Ofast)

set(AutoGen_src src/main.cpp src/DVRstudyAppBase.cpp src/AutoGenApp.cpp src/VolumeTextureMaterial.cpp src/TextureRenderMaterial.cpp)
set(AutoGen_include include/DVRstudyAppBase.hpp include/AutoGenApp.hpp include/VolumeTextureMaterial.hpp include/TextureRenderMaterial.hpp)

add_executable(AutoGen ${AutoGen_include} ${AutoGen_src})

target_include_directories(AutoGen PUBLIC include PRIVATE src)
target_link_libraries(AutoGen GLEngine glfw GLEW::GLEW ${OPENGL_LIBRARY} glm)