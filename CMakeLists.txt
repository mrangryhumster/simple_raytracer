cmake_minimum_required (VERSION 3.8)
#----------------------------------------------------------------
project ("Raytracing")
#----------------------------------------------------------------
# compiler flags
set(CMAKE_CXX_STANDARD 14)
#----------------------------------------------------------------
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
#----------------------------------------------------------------
# main source
set(SOURCES_CPP
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/ConsoleApplication.cpp
    ${PROJECT_SOURCE_DIR}/GUIApplication.cpp
    ${PROJECT_SOURCE_DIR}/raytracer/utils/RaytracerLogger.cpp
    ${PROJECT_SOURCE_DIR}/raytracer/filesystem/FileSystem.cpp
    ${PROJECT_SOURCE_DIR}/raytracer/filesystem/formats/ImageFormat.cpp
    ${PROJECT_SOURCE_DIR}/raytracer/filesystem/formats/SceneFormat.cpp
    ${PROJECT_SOURCE_DIR}/raytracer/scene/RaytracerSceneContainer.cpp
    #null
    ${PROJECT_SOURCE_DIR}/raytracer/api/null/NullRaytracerTarget.cpp
    #OpenGL
    ${PROJECT_SOURCE_DIR}/raytracer/api/opengl/OpenGLRaytracerTarget.cpp
    #Software RT
    ${PROJECT_SOURCE_DIR}/raytracer/software/SoftwareRaytracer.cpp
    ${PROJECT_SOURCE_DIR}/raytracer/software/kernel/Kernel.cpp
    ${PROJECT_SOURCE_DIR}/raytracer/software/kernel/scene/SceneHierarchyBVH.cpp
    ${PROJECT_SOURCE_DIR}/raytracer/software/kernel/scene/SceneHierarchyBVH_RTIOW.cpp
)
#----------------------------------------------------------------
# main include dirs
set(INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/raytracer
    ${PROJECT_SOURCE_DIR}/raytracer/math
    ${PROJECT_SOURCE_DIR}/raytracer/threading
    ${PROJECT_SOURCE_DIR}/raytracer/utils
    ${PROJECT_SOURCE_DIR}/raytracer/filesystem
    ${PROJECT_SOURCE_DIR}/raytracer/filesystem/formats
    ${PROJECT_SOURCE_DIR}/raytracer/api
    ${PROJECT_SOURCE_DIR}/raytracer/scene
    #null
    ${PROJECT_SOURCE_DIR}/raytracer/api/null
    #OpenGL
    ${PROJECT_SOURCE_DIR}/raytracer/api/opengl
    #Software RT
    ${PROJECT_SOURCE_DIR}/raytracer/software
    ${PROJECT_SOURCE_DIR}/raytracer/software/kernel
    ${PROJECT_SOURCE_DIR}/raytracer/software/kernel/utils
    ${PROJECT_SOURCE_DIR}/raytracer/software/kernel/objects
    ${PROJECT_SOURCE_DIR}/raytracer/software/kernel/scene
)
#----------------------------------------------------------------
#third party
include("${PROJECT_SOURCE_DIR}/third_party/CMakeLists.txt")

#################################################################
#################################################################
#################################################################

add_executable (${PROJECT_NAME} ${SOURCES_CPP} ${SOURCES_C} ${EXT_SOURCES_CPP} ${EXT_SOURCES_C})
#----------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXT_LIBRARY})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})
#----------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC ${EXT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
#----------------------------------------------------------------