project(DVRstudy)

set(CMAKE_CXX_FLAGS -Ofast)
add_library(DVRstudyAppBase include/DVRstudyAppBase.hpp src/DVRstudyAppBase.cpp)

target_include_directories(DVRstudyAppBase PUBLIC include PRIVATE src)
target_link_libraries(DVRstudyAppBase GLEngine glfw)