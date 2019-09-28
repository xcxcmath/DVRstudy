project(DVRstudy)

set(CMAKE_CXX_FLAGS -Ofast)

set(BasicHolo_src src/main.cpp src/BasicHoloApp.cpp)
set(BasicHolo_include include/BasicHoloApp.hpp)

add_executable(BasicHolo ${BasicHolo_include} ${BasicHolo_src})

target_include_directories(BasicHolo PUBLIC include PRIVATE src)
target_link_libraries(BasicHolo DVRstudyAppBase)