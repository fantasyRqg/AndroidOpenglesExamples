#include <jni.h>
#include <string>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/string_cast.hpp>

extern "C"
JNIEXPORT jstring JNICALL
Java_rqg_fantasy_androidopenglexamples_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    glm::mat4 I(1.0f);

    std::string hello = "Hello from C++" + glm::to_string(I);
    return env->NewStringUTF(hello.c_str());
}


#undef GLM_ENABLE_EXPERIMENTAL
