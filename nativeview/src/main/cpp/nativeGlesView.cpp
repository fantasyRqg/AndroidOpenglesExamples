//
// Created by ranqingguo on 3/9/17.
//

#include <jni.h>
#include <common.h>
#include <memory>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>


#include "GlThread.h"
#include "renderer/PolygonOffsetRenderer.h"
#include "renderer/InstanceRenderer.h"


#undef TAG
#define TAG "NativeGLESView"


struct fields_t {
    jfieldID glThread;
    jfieldID eglWrapper;
};

static fields_t fields;

#define GL_THREAD_JNI_ID "mGlThreadPointer"
#define EGL_WRAPPER_JNI_ID "mEglWrapperPointer"


GlThread *getGlThread(JNIEnv *env, jobject thiz) {
    return (GlThread *) env->GetLongField(thiz, fields.glThread);
}

EGLWrapper *getEglWrapper(JNIEnv *env, jobject thiz) {
    return (EGLWrapper *) env->GetLongField(thiz, fields.eglWrapper);
}

void nativeClassInit(JNIEnv *env, jclass clazz) {
    fields.glThread = env->GetFieldID(clazz, GL_THREAD_JNI_ID, "J");
    fields.eglWrapper = env->GetFieldID(clazz, EGL_WRAPPER_JNI_ID, "J");
}

void surfaceCreated(JNIEnv *env, jobject thiz, jobject surface, jobject assetManager) {
    std::unique_ptr<Renderer> ptr(new InstanceRenderer());

    //java level hold assetManager ref
    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);

    EGLWrapper *egl = new EGLWrapper(
            ANativeWindow_fromSurface(env, surface),
            std::move(ptr),
            mgr);

    GlThread *glThread = new GlThread(egl);


    env->SetLongField(thiz, fields.glThread, (jlong) glThread);
    env->SetLongField(thiz, fields.eglWrapper, (jlong) egl);

}


void surfaceChanged(JNIEnv *env, jobject thiz, jint format, jint width,
                    jint height) {
    GlThread *g = getGlThread(env, thiz);
    if (g) {
        g->surfaceChanged(format, width, height);
    }

}

void surfaceDestroyed(JNIEnv *env, jobject thiz) {
    GlThread *g = getGlThread(env, thiz);
    if (g) {
        g->surfaceDestroyed();

        g->join();

        delete (g);
    }

    EGLWrapper *egl = getEglWrapper(env, thiz);

    if (egl) {
        delete (egl);
    }

}

void surfaceRedrawNeeded(JNIEnv *env, jobject thiz) {
    GlThread *g = getGlThread(env, thiz);
    if (g) {
        g->surfaceRedrawNeeded();
    }
}


void onResume(JNIEnv *env, jobject thiz) {
    GlThread *g = getGlThread(env, thiz);
    if (g) {
        g->onResume();
    }

}

void onPause(JNIEnv *env, jobject thiz) {
    GlThread *g = getGlThread(env, thiz);
    if (g) {
        g->onPause();
    }
}


//------------------------------------------------------------------------------------------------------------
static JNINativeMethod gGLES3JniViewMethods[] = {
        {"nativeSurfaceCreated",      "(Landroid/view/Surface;Landroid/content/res/AssetManager;)V", (void *) surfaceCreated},
        {"nativeSurfaceChanged",      "(III)V",                                                      (void *) surfaceChanged},
        {"nativeSurfaceDestroyed",    "()V",                                                         (void *) surfaceDestroyed},
        {"nativeSurfaceRedrawNeeded", "()V",                                                         (void *) surfaceRedrawNeeded},
        {"nativeClassInit",           "()V",                                                         (void *) nativeClassInit},
        {"nativeOnResume",            "()V",                                                         (void *) onResume},
        {"nativeOnPause",             "()V",                                                         (void *) onPause},
};

static const char *classPathName = "rqg/fantasy/nativeview/NativeGLESView";


/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv *env, const char *className,
                                 JNINativeMethod *gMethods, int numMethods) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}


/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv *env) {
    if (!registerNativeMethods(env, classPathName,
                               gGLES3JniViewMethods,
                               sizeof(gGLES3JniViewMethods) / sizeof(gGLES3JniViewMethods[0]))) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv *env;
    void *venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv *env = NULL;

    LOGI("JNI_OnLoad");
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;
    if (registerNatives(env) != JNI_TRUE) {
        LOGE("ERROR: registerNatives failed");
        goto bail;
    }

    result = JNI_VERSION_1_4;

    bail:
    return result;
}
