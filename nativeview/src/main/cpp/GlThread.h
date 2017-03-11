//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_GLTHREAD_H
#define ANDROIDOPENGLEXAMPLES_GLTHREAD_H

#include <thread>
#include <condition_variable>
#include <mutex>

#include "EGLWrapper.h"

class GlThread : public std::thread {

public:
    GlThread(EGLWrapper *eglWrapper);

    void surfaceChanged(int format, int width, int height);

    void onPause();

    void onResume();

private:
    void run();

    EGLWrapper *mEglWrapper;

    std::mutex mMutex;

    std::condition_variable mConditionVariable;

    bool mRun;
    bool mRequestPause;

    bool mSufaceChanged;

    int mWidth;
    int mHeight;
    int mColorFormat;

};


#endif //ANDROIDOPENGLEXAMPLES_GLTHREAD_H
