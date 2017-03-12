//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_GLTHREAD_H
#define ANDROIDOPENGLEXAMPLES_GLTHREAD_H

#include <thread>
#include <condition_variable>
#include <mutex>

#include "EGLWrapper.h"


#define GLVIEW_ACTION_SURFACE_CHANGE 0;
#define GLVIEW_ACTION_SURFACE_DESTROY 1;
#define GLVIEW_ACTION_REDRAW_NEED 2;
#define GLVIEW_ACTION_PAUSE 3;
#define GLVIEW_ACTION_RESUME 4;


class GlThread : public std::thread {

public:
    GlThread(EGLWrapper *eglWrapper);

    void postEvent();

    void onPause();

    void onResume();

private:
    void run();

    void eventHandler();

    std::mutex mEventMutex;

    EGLWrapper *mEglWrapper;

    std::mutex mPauseMutex;

    std::condition_variable mPauseCV;

    bool mRun;

    bool mRequestPause;
};


#endif //ANDROIDOPENGLEXAMPLES_GLTHREAD_H
