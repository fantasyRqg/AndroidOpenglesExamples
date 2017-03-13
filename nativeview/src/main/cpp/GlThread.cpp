//
// Created by ranqingguo on 3/10/17.
//

#include <iostream>
#include "GlThread.h"

#undef  TAG
#define TAG "GLThread"

#define FRAME_RATE 30

GlThread::GlThread(EGLWrapper *eglWrapper) :
        thread(&GlThread::run, this) {

    mEglWrapper = eglWrapper;

}

void GlThread::run() {


    if (!mEglWrapper->eglSetUp()) {
        LOGE("EGL Init Failure");
    }

    mRun = true;
    mSurfaceChanged = false;
    mSurfaceDestroyed = false;
//    mSurfaceRedrawNeeded = false;
//    mSurfaceCreated = false;


    auto frameInterval = std::chrono::microseconds(1000000 / FRAME_RATE);

    auto lastTS = std::chrono::steady_clock::now();


    while (!mSurfaceDestroyed && mRun) {
        //----------------------event handle------------------------------

        if (mSurfaceChanged) {
            mEglWrapper->resize(mFormat, mWidth, mHeight);
            mSurfaceChanged = false;
        }


        //----------------------------------------------------

        auto now = std::chrono::steady_clock::now();

        auto diffTime = now - lastTS;
        lastTS = now;

        if (diffTime < frameInterval) {
            std::unique_lock<std::mutex> lk(mPauseMutex);
            mPauseCV.wait_for(lk, frameInterval - diffTime);
            lk.unlock();
        }


        if (mRequestPause) {
            mRequestPause = false;

            std::unique_lock<std::mutex> lk(mPauseMutex);
            mPauseCV.wait(lk);
            lk.unlock();
            continue;
        }


        mEglWrapper->render();
    }

    mEglWrapper->eglTearDown();
}


void GlThread::onPause() {
    mRequestPause = true;
    mPauseCV.notify_all();
}

void GlThread::onResume() {
    mRequestPause = false;
    mPauseCV.notify_all();
}

void GlThread::surfaceChanged(int format, int width, int height) {

}

void GlThread::surfaceDestroyed() {

}

void GlThread::surfaceRedrawNeeded() {

}

void GlThread::surfaceCreated() {
    mSurfaceChanged = false;
}

