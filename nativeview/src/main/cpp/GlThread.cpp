//
// Created by ranqingguo on 3/10/17.
//

#include "GlThread.h"
#include <iostream>

#define TAG "GLThread"

#define FRAME_RATE 30

GlThread::GlThread(EGLWrapper *eglWrapper) :
        mEglWrapper(eglWrapper),
        thread(&GlThread::run, this) {

}

void GlThread::run() {

    mSufaceChanged = false;
    mRequestPause = false;

    if (!mEglWrapper->eglSetUp()) {
        LOGE("EGL Init Failure");
    }


    auto frameInterval = std::chrono::microseconds(1000000 / FRAME_RATE);

    auto lastTS = std::chrono::steady_clock::now();

    while (mRun) {

        auto now = std::chrono::steady_clock::now();

        auto diffTime = now - lastTS;
        lastTS = now;

        if (diffTime < frameInterval) {
            std::unique_lock<std::mutex> lk(mMutex);
            mConditionVariable.wait_for(lk, frameInterval - diffTime);
            lk.unlock();
        }


        if (mSufaceChanged) {
            mEglWrapper->resize(mColorFormat, mWidth, mHeight);
        }

        if (mRequestPause) {
            mRequestPause = false;

            std::unique_lock<std::mutex> lk(mMutex);
            mConditionVariable.wait(lk);
            lk.unlock();
            continue;
        }


    }
}

void GlThread::surfaceChanged(int format, int width, int height) {
    mSufaceChanged = true;

    mColorFormat = format;
    mWidth = width;
    mHeight = height;
}

void GlThread::onPause() {
    mRequestPause = true;
    mConditionVariable.notify_all();
}

void GlThread::onResume() {
    mRequestPause = false;
    mConditionVariable.notify_all();
}
