cmake_minimum_required(VERSION 3.20)
project(OpenGLTraining C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

set(GLM_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS        OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL            OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER  ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER  ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        GIT_SHALLOW    TRUE)

FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
        GIT_SHALLOW    TRUE)

FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG        v6.0.2 # ou une version 6.0.x plus récente
        GIT_SHALLOW    TRUE)

FetchContent_MakeAvailable(glfw glm assimp)

find_package(OpenGL REQUIRED)

set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
if(NOT EXISTS ${GLAD_DIR}/src/glad.c)
    message(FATAL_ERROR
            "glad est introuvable : place les fichiers générés dans external/glad "
            "(dossiers include/ et src/).")
endif()

add_executable(OpenGLTraining
        src/main.cpp
        ${GLAD_DIR}/src/glad.c
)

target_include_directories(OpenGLTraining PRIVATE
        src
        ${GLAD_DIR}/include
        external/stb
)

target_link_libraries(OpenGLTraining PRIVATE
        glfw
        glm::glm
        assimp
        OpenGL::GL
)

set_target_properties(OpenGLTraining PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_compile_definitions(OpenGLTraining PRIVATE
        ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/")