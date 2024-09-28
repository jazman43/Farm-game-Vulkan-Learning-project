cmake_minimum_required(VERSION 3.5.0)
project(Project_Farm VERSION 0.1.0 LANGUAGES C CXX)

# Gather all source and header files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/Headers/*.h)

add_executable(Project_Farm ${HEADERS} ${SOURCES})

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

# Define directories for shaders
set(SHADER_SRC_DIR "${PROJECT_SOURCE_DIR}/src/Shaders")
set(SHADER_BINARY_DIR "${CMAKE_BINARY_DIR}/Bin/Shaders")

# Ensure the shader output directory exists
file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})

# Find all GLSL shader files
file(GLOB_RECURSE GLSL_SHADERS "${SHADER_SRC_DIR}/*.glsl")

# Function to compile GLSL to SPIR-V
function(compile_shader SHADER)
message("Compiling shader: ${SHADER}")
    
# Get the file name without directory and extension
get_filename_component(FILE_NAME ${SHADER} NAME_WE) # Name without extension
get_filename_component(FILE_EXT ${SHADER} EXT)     # File extension

# Set output file path
set(SPIRV_OUTPUT "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")

# Determine shader stage based on file name or extension
if(FILE_NAME MATCHES ".*_vert.*" OR FILE_EXT STREQUAL ".vert")
    set(SHADER_STAGE "vert")
elseif(FILE_NAME MATCHES ".*_frag.*" OR FILE_EXT STREQUAL ".frag")
    set(SHADER_STAGE "frag")
elseif(FILE_NAME MATCHES ".*_geom.*" OR FILE_EXT STREQUAL ".geom")
    set(SHADER_STAGE "geom")
elseif(FILE_NAME MATCHES ".*_comp.*" OR FILE_EXT STREQUAL ".comp")
    set(SHADER_STAGE "comp")
else()
    message(FATAL_ERROR "Unknown shader stage for ${SHADER}")
endif()

# Add custom command to compile the shader with the correct stage
add_custom_command(
    OUTPUT ${SPIRV_OUTPUT}
    COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V -S ${SHADER_STAGE} "${SHADER}" -o "${SPIRV_OUTPUT}"
    DEPENDS ${SHADER}
    COMMENT "Compiling ${SHADER} to SPIR-V"
    VERBATIM
)

# Append the output file to the list of compiled shaders
set(SPIRV_SHADERS ${SPIRV_SHADERS} ${SPIRV_OUTPUT} PARENT_SCOPE)
endfunction()

# Compile each shader
set(SPIRV_SHADERS "")
foreach(SHADER ${GLSL_SHADERS})
    compile_shader(${SHADER})
    #list(APPEND SPIRV_SHADERS ${SPIRV_OUTPUT})
endforeach()

message("Compiling shader stage: ${SHADER_STAGE}")
message("SPIR-V output: ${SPIRV_OUTPUT}")
# Add shaders as a custom target
add_custom_target(Shaders ALL DEPENDS ${SPIRV_SHADERS})

# Ensure shaders are compiled before building the project
add_dependencies(Project_Farm Shaders)

# CPack setup (for packaging)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

