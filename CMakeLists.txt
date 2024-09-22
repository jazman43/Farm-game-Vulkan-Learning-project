cmake_minimum_required(VERSION 3.5.0)
project(Project_Farm VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/*.h)

add_executable(Project_Farm src/Headers/FarmApp.h ${SOURCES} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)



target_link_libraries(Project_Farm 
    glfw
    vulkan
    )