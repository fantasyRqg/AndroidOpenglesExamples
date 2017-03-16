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

    auto startTime = std::chrono::steady_clock::now() + frameInterval;

    LOGD("Loop in");

    while (!mSurfaceDestroyed && mRun) {
        //----------------------event handle------------------------------

        if (mSurfaceChanged) {
            mEglWrapper->resize(mFormat, mWidth, mHeight);
            mSurfaceChanged = false;
        }


        //----------------------------------------------------

        auto endTime = std::chrono::steady_clock::now();
        auto diffTime = endTime - startTime;

        if (diffTime < frameInterval && !mRequestPause && !mSurfaceDestroyed) {
            std::unique_lock<std::mutex> lk(mPauseMutex);

            auto wt = frameInterval - diffTime;


            mPauseCV.wait_for(lk, wt);
            lk.unlock();
        }


        startTime = std::chrono::steady_clock::now();


        if (mRequestPause && !mSurfaceDestroyed) {
            mRequestPause = false;

            std::unique_lock<std::mutex> lk(mPauseMutex);
            mPauseCV.wait(lk);
            lk.unlock();
            continue;
        }


        using namespace std::chrono;
        auto tm = time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count();

        mEglWrapper->render(tm);


    }

    mEglWrapper->eglTearDown();
}


void GlThread::onPause() {
    LOGD("onPause");
    mRequestPause = true;
    mPauseCV.notify_all();
}

void GlThread::onResume() {
    LOGD("onResume");
    mRequestPause = false;
    mPauseCV.notify_all();
}

void GlThread::surfaceChanged(int format, int width, int height) {
    LOGD("surfaceChanged");
    mFormat = format;
    mWidth = width;
    mHeight = height;

    mSurfaceChanged = true;

    mPauseCV.notify_all();
}

void GlThread::surfaceDestroyed() {
    LOGD("surfaceDestroyed");
    mSurfaceDestroyed = true;

    mPauseCV.notify_all();
}

void GlThread::surfaceRedrawNeeded() {

    mPauseCV.notify_all();
}



