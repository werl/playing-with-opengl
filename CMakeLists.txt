cmake_minimum_required(VERSION 3.5)
project(LearningCpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/GL/gl3w.h ${CMAKE_CURRENT_SOURCE_DIR}/GL/glcorearb.h)

include_directories(include)

# Add GLBinding
add_subdirectory(lib/glbinding-2.1.1)
include_directories(lib/glbinding/source/glbinding-2.1.1/include)

# Add GLFW
add_subdirectory(lib/glfw-3.2.1)
include_directories(lib/glfw-3.2.1/include)

# Add GLM
add_subdirectory(lib/glm)
include_directories(lib/glm/glm)

if (APPLE)
    find_library(COCOA Cocoa)
    find_library(OPENGL OpenGL)
    find_library(IOKIT IOKit)
    find_library(COREVIDEO CoreVideo)
    set(EXTRA_LIBS ${COCOA} ${OPENGL} ${IOKIT} ${COREVIDEO})
endif (APPLE)

file(GLOB SOURCES "src/*.cpp")
add_executable(LearningCpp ${SOURCES} src/shader.cpp src/shader.hpp src/image.cpp src/image.hpp src/controls.cpp src/controls.hpp)
# Transfer shaders to build directory
add_custom_command(TARGET LearningCpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:LearningCpp>/shaders)
# Transfer images to build directory
add_custom_command(TARGET LearningCpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:LearningCpp>/textures)
# Link GL3W
target_link_libraries(LearningCpp glbinding)
# Link GLFW3 and required libs
target_link_libraries(LearningCpp glfw ${EXTRA_LIBS} ${GLFW_LIBRARIES})
# Link GLM
target_link_libraries(LearningCpp glm)
# LuaJIT
