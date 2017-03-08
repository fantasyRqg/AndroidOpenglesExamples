cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")

set(EXTERNAL_DIR ../../../../external)


find_library(
        LOG_LIB

        log)

add_library(app-glue STATIC
        ${EXTERNAL_DIR}/native_app_glue
        ${EXTERNAL_DIR}/native_app_glue/android_native_app_glue.c)

add_library(es-utils STATIC
        ${EXTERNAL_DIR}/esUtils/esShader.c
        ${EXTERNAL_DIR}/esUtils/esShapes.c
        ${EXTERNAL_DIR}/esUtils/esTransform.c
        ${EXTERNAL_DIR}/esUtils/esUtil.c
        ${EXTERNAL_DIR}/esUtils/main.c
        )

target_include_directories(es-utils PRIVATE
        ${EXTERNAL_DIR}/native_app_glue
        ${EXTERNAL_DIR}/esUtils
        )


target_link_libraries(es-utils
        android
        EGL
        GLESv3
        app-glue
        ${LOG_LIB})


include_directories(
        ${EXTERNAL_DIR}/glm
)


add_library(native-lib SHARED
        native-lib.cpp
        Hello_Triangle.c
        )

target_include_directories(native-lib PRIVATE
        ${EXTERNAL_DIR}/native_app_glue
        ${EXTERNAL_DIR}/esUtils
        )


target_link_libraries(native-lib
        es-utils
        ${LOG_LIB})