//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_EGLWRAPPER_H
#define ANDROIDOPENGLEXAMPLES_EGLWRAPPER_H


#include <vector>
#include <set>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <common.h>
#include <memory>


#include "EglCommonDefine.h"
#include "Renderer.h"


#define TAG "EGLWrapper"


class EGLWrapper {
public:

    EGLWrapper(EGLNativeWindowType window, std::vector<std::unique_ptr<Renderer>> renders);

    ~EGLWrapper();


    bool eglSetUp();

    void eglTearDown();

    bool render();


    void resize(int format, int width, int height);

    void prepareRenders();

    void destroyRenders();


protected:
    bool mOffScreen;
    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLContext mEglContext;
    EGLConfig mGlConfig;
    EGLNativeWindowType mWindow;

    int mWindowWidth;
    int mWindowHeight;

    std::vector<std::unique_ptr<Renderer>> mRenderers;

    void renderTask();

};


#endif //ANDROIDOPENGLEXAMPLES_EGLWRAPPER_H
