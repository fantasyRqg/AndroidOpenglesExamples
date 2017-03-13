//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_GLTHREAD_H
#define ANDROIDOPENGLEXAMPLES_GLTHREAD_H

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "EGLWrapper.h"


class GlThread : public std::thread {

public:
    GlThread(EGLWrapper *eglWrapper);


    void onPause();

    void onResume();

    void surfaceChanged(int format, int width, int height);

    void surfaceDestroyed();

    void surfaceRedrawNeeded();

    void surfaceCreated();

private:
    void run();


    EGLWrapper *mEglWrapper;

    std::mutex mPauseMutex;

    std::condition_variable mPauseCV;


    bool mRun;

    bool mRequestPause;
    bool mSurfaceChanged;
    bool mSurfaceDestroyed;


    int mFormat;
    int mWidth;
    int mHeight;
};


#endif //ANDROIDOPENGLEXAMPLES_GLTHREAD_H
