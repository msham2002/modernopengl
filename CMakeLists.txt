cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0074 NEW)

project(Raycaster CXX)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:DEBUG>:/MTd>
        $<$<CONFIG:RELEASE>:/MT>
    )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp" "vendor/glm/detail/glm.cpp")

set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL2/cmake)
find_package(SDL2 REQUIRED)

find_package(OpenGL REQUIRED)

add_executable(raycaster ${SOURCES})

target_include_directories(raycaster
    PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLEW/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(raycaster PRIVATE GLEW_STATIC)

target_link_libraries(raycaster
    PRIVATE
    ${SDL2_LIBRARIES}
    # Link GLEW directly by providing the path to the static library
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLEW/lib/Release/x64/glew32s.lib
    ${OPENGL_LIBRARIES}
)

add_custom_command(
    TARGET raycaster POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL2/lib/x64/SDL2.dll"
    $<TARGET_FILE_DIR:raycaster>
)
add_custom_command(
    TARGET raycaster POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/res"
    $<TARGET_FILE_DIR:raycaster>/res
)

if(MSVC)
    set_target_properties(raycaster PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Debug"
    )
endif()