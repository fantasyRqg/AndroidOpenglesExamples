//
// Created by ranqingguo on 3/10/17.
//

#include <algorithm>

#include "EGLWrapper.h"
#include <EGL/eglext.h>
#include "Renderer.h"

EGLWrapper::EGLWrapper(EGLNativeWindowType window,
                       std::unique_ptr<Renderer> &&renders,
                       AAssetManager *pManager)
        : mEglDisplay(EGL_NO_DISPLAY), mEglSurface(EGL_NO_SURFACE), mEglContext(EGL_NO_CONTEXT) {


    mWindow = window;
    mAssetManager = pManager;
    mRenderer = std::move(renders);

}

EGLWrapper::~EGLWrapper() {
}

bool EGLWrapper::render(long timestampMills) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    renderTask(timestampMills);

    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "GLError %d in draw", err);
        return false;
    }

    EGL_RESULT_CHECK(eglSwapBuffers(mEglDisplay, mEglSurface));
    return true;

}


bool EGLWrapper::eglSetUp() {

    static const EGLint DEFAULT_CONTEXT_ATTRIBS[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE};

    static const EGLint DEFAULT_CONFIG_ATTRIBS[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,


#ifdef EGL_KHR_create_context
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
#else
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#endif

            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_STENCIL_SIZE, 0,
            EGL_NONE};

    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGL_RESULT_CHECK(mEglDisplay != EGL_NO_DISPLAY);

    EGLint major;
    EGLint minor;
    EGL_RESULT_CHECK(eglInitialize(mEglDisplay, &major, &minor));

    EGLint numConfigs = 0;
    EGL_RESULT_CHECK(
            eglChooseConfig(mEglDisplay, DEFAULT_CONFIG_ATTRIBS, &mGlConfig, 1, &numConfigs)
            && (numConfigs > 0));

    mEglSurface = eglCreateWindowSurface(mEglDisplay, mGlConfig, mWindow, NULL);
    EGL_RESULT_CHECK(mEglSurface != EGL_NO_SURFACE);

    mEglContext = eglCreateContext(mEglDisplay, mGlConfig, EGL_NO_CONTEXT, DEFAULT_CONTEXT_ATTRIBS);
    EGL_RESULT_CHECK(mEglContext != EGL_NO_CONTEXT);

    EGL_RESULT_CHECK(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext));
    EGL_RESULT_CHECK(eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &mWindowWidth));
    EGL_RESULT_CHECK(eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &mWindowHeight));


    LOGI("%s", glGetString(GL_VERSION));
    LOGI("%s", glGetString(GL_VENDOR));
    LOGI("%s", glGetString(GL_RENDERER));
    LOGI("%s", glGetString(GL_EXTENSIONS));

    glViewport(0, 0, mWindowWidth, mWindowHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    prepareRenders();
    return true;
}

bool EGLWrapper::eglTearDown() {
    destroyRenders();

    eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);


    if (mEglContext != EGL_NO_CONTEXT) {
        eglDestroyContext(mEglDisplay, mEglContext);
    }

    if (mEglDisplay != EGL_NO_DISPLAY) {

        if (mEglSurface != EGL_NO_SURFACE) {
            eglDestroySurface(mEglDisplay, mEglSurface);
        }

        eglTerminate(mEglDisplay);
    }


    mEglContext = EGL_NO_CONTEXT;
    mEglSurface = EGL_NO_SURFACE;
    mEglDisplay = EGL_NO_DISPLAY;

    ANativeWindow_release(mWindow);
    return true;
}

void EGLWrapper::renderTask(long timestampMills) {
    mRenderer->render(timestampMills);
}

void EGLWrapper::destroyRenders() {
    mRenderer->tearDown();
}

void EGLWrapper::prepareRenders() {

    mRenderer->setEglWrapper(this);
    mRenderer->setUp();

}

void EGLWrapper::resize(int format, int width, int height) {
    mWindowWidth = width;
    mWindowHeight = height;
    glViewport(0, 0, width, height);

}

int EGLWrapper::getWindowWidth() const {
    return mWindowWidth;
}

AAssetManager *EGLWrapper::getAssetManager() const {
    return mAssetManager;
}

int EGLWrapper::getWindowHeight() const {
    return mWindowHeight;
}

