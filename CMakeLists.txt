cmake_minimum_required(VERSION 3.10)

project(04MVID CXX C)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config types" FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Files for dependencies and platform specific config
include(cmake/deps_files.cmake)
include(cmake/platform.cmake)
include(cmake/compiler.cmake)

#****************************************************
# Engine Library
#****************************************************
#Engine Source Files

set(ENGINE_SRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/camera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/deps.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/shader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/geometry/cube.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/geometry/geometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/geometry/quad.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/engine/geometry/sphere.cpp)

set(ENGINE_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/camera.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/input.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/shader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/texture.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/window.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/geometry/cube.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/geometry/geometry.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/geometry/quad.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/engine/geometry/sphere.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/ PREFIX "Source Files" FILES ${ENGINE_SRC_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/engine PREFIX "Header Files" FILES ${ENGINE_HEADER_FILES})

add_library(EngineLib STATIC ${ENGINE_SRC_FILES} ${ENGINE_HEADER_FILES} ${DEPS_SRC_FILES_COMMON})

target_include_directories(EngineLib PRIVATE ${INCLUDE_PATHS})
target_compile_definitions(EngineLib PRIVATE -D_GLFW_USE_OPENGL=1)
target_compile_features(EngineLib PRIVATE cxx_std_14)

configure_platform(EngineLib)
configure_compiler(EngineLib)

#****************************************************
# Projects
#****************************************************
# List of projects
set(PROJECT_LIST AG01 AG02 AG03 AG04 AG05 AG06 AG07 AG08_01 AG08_02
    AG09_01 AG09_02 AG09_03 AG09_04 AG09_05 AG10 AG11_01 AG11_02 AG11_03 AG12 AG13 AG14
    EJ02_01 EJ02_02 EJ02_03 EJ02_04 EJ02_05 EJ02_06
    EJ03_01 EJ03_02 EJ03_03 EJ03_04
    EJ04_01 EJ04_02 EJ04_03 EJ04_04 EJ04_05
    EJ05_01 EJ05_02 EJ05_03 EJ05_04
    EJ06_01 EJ06_02
    EJ07_01 EJ07_02 EJ07_03 EJ07_04 EJ07_05
    EJ08_01 EJ08_02 EJ08_03 EJ08_04 EJ08_05
    EJ09_01 EJ09_02 EJ09_03 EJ09_04
    EJ10_01
    EJ11_01 EJ11_02 EJ11_03 EJ11_04
    EJ12_01 EJ12_02
    EJ13_01 EJ13_02 EJ13_03
    EJ14_01 EJ14_02
    FINAL)

#Root path of the projects
set(PROJECTS_PATH ${CMAKE_CURRENT_LIST_DIR}/projects)
#Configure All Projects
foreach(PROJECT ${PROJECT_LIST})
    message("Generating Project " ${PROJECT})

    #Create Directory and main.cpp
    file(MAKE_DIRECTORY ${PROJECTS_PATH}/${PROJECT})
    file(TOUCH ${PROJECTS_PATH}/${PROJECT}/main.cpp)

    #Create Project and configure
    add_executable(${PROJECT} ${PROJECTS_PATH}/${PROJECT}/main.cpp ${DEPS_SRC_FILES})
    target_include_directories(${PROJECT} PRIVATE ${INCLUDE_PATHS})
    target_compile_definitions(${PROJECT} PRIVATE -D_GLFW_USE_OPENGL=1)
    target_link_libraries(${PROJECT} PRIVATE EngineLib)
    target_compile_features(${PROJECT} PRIVATE cxx_std_14)
    configure_platform(${PROJECT})
    configure_compiler(${PROJECT})
endforeach()