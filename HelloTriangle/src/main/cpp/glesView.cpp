


#include <jni.h>
#include <string>
#include <android/log.h>
#include "gles3jni.h"


#define TAG "GLES3VIEW"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)


struct fields_t {
    jfieldID jniRenderer;
};

static fields_t fields;

static void printGlString(const char *name, GLenum s);


#define JNI_RENDEERER_JNI_ID "mJniRenderer"


Renderer *getJniRenerer(JNIEnv *env, jobject thiz) {
    return (Renderer *) env->GetLongField(thiz, fields.jniRenderer);
}

void nativeClassInit(JNIEnv *env, jclass clazz) {
    fields.jniRenderer = env->GetFieldID(clazz, JNI_RENDEERER_JNI_ID, "J");
}


void printGlString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    ALOGV("GL %s: %s\n", name, v);
}

void init(JNIEnv *env, jobject thiz) {
    Renderer *pRender = getJniRenerer(env, thiz);
    if (pRender) {
        delete (pRender);
        pRender = NULL;
    }

//    printGlString("Version", GL_VERSION);
//    printGlString("Vendor", GL_VENDOR);
//    printGlString("Renderer", GL_RENDERER);
//    printGlString("Extensions", GL_EXTENSIONS);

    pRender = createES3Renderer();

    LOGI("init %d", pRender);

    env->SetLongField(thiz, fields.jniRenderer, (jlong) pRender);
}

void step(JNIEnv *env, jobject thiz) {

    Renderer *p = getJniRenerer(env, thiz);

    if (p) {
        p->render();
    }
}


void destroy(JNIEnv *env, jobject thiz) {
    Renderer *p = getJniRenerer(env, thiz);


    LOGI("destroy %d", p);

    if (p) {
        delete (p);
        p = NULL;

        env->SetLongField(thiz, fields.jniRenderer, (jlong) NULL);
    }
}

void resize(JNIEnv *env, jobject thiz, jint width, jint height) {
    Renderer *p = getJniRenerer(env, thiz);

    if (p) {
        p->resize(width, height);
    }
}


static JNINativeMethod gGLES3JniViewMethods[] = {
        {"init",            "()V",   (void *) init},
        {"step",            "()V",   (void *) step},
        {"destroy",         "()V",   (void *) destroy},
        {"resize",          "(II)V", (void *) resize},
        {"nativeClassInit", "()V",   (void *) nativeClassInit},
};

static const char *classPathName = "rqg/fantasy/androidopenglexamples/GLES3JniView";


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

