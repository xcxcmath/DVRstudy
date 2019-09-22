project(openGL_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLEngine_SRC_BASE
        src/Base/Exception.cpp
        src/Base/Shader.cpp
        src/Base/Program.cpp
        src/Base/Transform.cpp
        src/Base/Mesh.cpp
        src/Base/Material.cpp
        )
set(GLEngine_INCLUDE_BASE
        include/GLEngine/Base/Exception.hpp
        include/GLEngine/Base/Shader.hpp
        include/GLEngine/Base/Program.hpp
        include/GLEngine/Base/Transform.hpp
        include/GLEngine/Base/Mesh.hpp
        include/GLEngine/Base/Material.hpp
        )

set(GLEngine_SRC
        ${GLEngine_SRC_BASE}
        src/Camera.cpp
        src/RenderObject.cpp
        )

set(GLEngine_INCLUDE
        ${GLEngine_INCLUDE_BASE}
        include/GLEngine/Camera.hpp
        include/GLEngine/RenderObject.hpp
        )

add_library(GLEngine ${GLEngine_SRC} ${GLEngine_INCLUDE})

target_include_directories(GLEngine PUBLIC include PRIVATE src)
target_link_libraries(GLEngine GLEW::GLEW ${OPENGL_LIBRARY} glm)
