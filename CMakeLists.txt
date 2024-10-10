cmake_minimum_required(VERSION 3.5.0)
project(Project_Farm VERSION 0.1.0 LANGUAGES C CXX)

# Gather all source and header files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/Headers/*.h)

add_executable(Project_Farm ${HEADERS} ${SOURCES})

add_dependencies(Project_Farm Shaders)

# Find Vulkan and GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Link libraries
target_link_libraries(Project_Farm
    glfw
    Vulkan::Vulkan
)

# Find glslangValidator (Vulkan SDK)
find_program(Vulkan_GLSLANG_VALIDATOR_EXECUTABLE NAMES glslangValidator)
if(NOT Vulkan_GLSLANG_VALIDATOR_EXECUTABLE)
    message(FATAL_ERROR "glslangValidator not found! Make sure it's installed and available in your PATH.")
endif()

message("glslangValidator found at: ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}")


# CPack setup (for packaging)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

############## Build SHADERS #######################

# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)

# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/src/Shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/src/Shaders/*.vert"
)

#message("found Source files ${GLSL_SOURCE_FILES}")

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/src/Shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL}
    COMMENT "shaders compiled ${SPIRV}"
    )
    
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

message("shaders ${SPIRV_BINARY_FILES}")

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)