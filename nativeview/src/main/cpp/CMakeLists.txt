cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")

set(EXTERNAL_DIR ../../../../external)


find_library(
        LOG_LIB
        log)



add_library(glCommon STATIC
        ${EXTERNAL_DIR}/common/common.cpp
        )


target_link_libraries(glCommon
        android
        EGL
        GLESv3
        ${LOG_LIB})

########################################################################

add_library(nativeGlesView SHARED
        nativeGlesView.cpp
        GlThread.cpp
        Renderer.cpp
        EGLWrapper.cpp
        renderer/TriangleRenderer.cpp
        renderer/InstanceRenderer.cpp
        )

target_include_directories(nativeGlesView PRIVATE
        ${EXTERNAL_DIR}/common
        ${EXTERNAL_DIR}/glm
        )


target_link_libraries(nativeGlesView
        glCommon
        )