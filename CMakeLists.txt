cmake_minimum_required(VERSION 3.10)
project(pbropengl VERSION 0.1.0)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

include_directories(libs/glad)
include_directories(libs/KHR)
include_directories(libs/imgui)
include_directories(libs/glfw/include/GLFW)
include_directories(include)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )

add_subdirectory(libs/glfw)

## Turn ON if intending to use gl.h from glfw.
## ON does not work with LearnOpenGL. Had to add "glad/include" manually.
option( GLFW-CMAKE-STARTER-USE-GLFW-GLAD "Use GLAD from GLFW" OFF )
if( GLFW-CMAKE-STARTER-USE-GLFW-GLAD )
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
endif()

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(pbropengl ${SOURCES})

### Compiler warnings
add_compile_options(-Wall) # Variable improperly initialized
add_compile_options(-Werror) # Variable improperly initialized
add_compile_options(-Wmaybe-uninitialized) # Variable improperly initialized
add_compile_options(-Wunused-variable) # Variable is defined but unused

set(CMAKE_CXX_STANDARD 17) # Sets C++17 as the default standard
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensures the standard is strictly enforced

target_link_libraries(pbropengl PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(pbropengl PRIVATE glfw)
target_link_libraries(pbropengl PRIVATE glm::glm-header-only)

if (!WIN32)
target_link_libraries(pbropengl assimp::assimp)
endif()
