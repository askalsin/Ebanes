cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(PROJECT_NAME Ebanes)

project(${PROJECT_NAME})

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/renderer/shader_program.cpp
    src/renderer/shader_program.hpp
    src/renderer/texture_2D.cpp
    src/renderer/texture_2D.hpp
    src/renderer/sprite.cpp
    src/renderer/sprite.hpp
    src/resources/resource_manager.cpp
    src/resources/resource_manager.hpp
    src/resources/stb_image.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)

set_target_properties(${PACKAGES_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)