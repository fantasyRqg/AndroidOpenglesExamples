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
#include <android/native_window.h>
#include <android/asset_manager.h>


#include "EglCommonDefine.h"
//#include "Renderer.h"


#undef TAG
#define TAG "EGLWrapper"

class Renderer;

class EGLWrapper {
public:
    EGLWrapper(EGLNativeWindowType window, std::unique_ptr<Renderer> &&renders,
               AAssetManager *pManager);


    ~EGLWrapper();


    bool eglSetUp();

    bool eglTearDown();

    bool render(long timestampMills);


    void resize(int format, int width, int height);

    void prepareRenders();

    void destroyRenders();

    int getWindowWidth() const;

    int getWindowHeight() const;

    AAssetManager *getAssetManager() const;


protected:
    bool mOffScreen;
    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLContext mEglContext;
    EGLConfig mGlConfig;
    EGLNativeWindowType mWindow;
    AAssetManager *mAssetManager;

    int mWindowWidth;
    int mWindowHeight;

    std::unique_ptr<Renderer> mRenderer;

    void renderTask(long timestampMills);

};


#endif //ANDROIDOPENGLEXAMPLES_EGLWRAPPER_H
