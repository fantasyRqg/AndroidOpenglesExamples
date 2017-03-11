//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_EGLCOMMONDEFINE_H
#define ANDROIDOPENGLEXAMPLES_EGLCOMMONDEFINE_H

#define EGL_RESULT_CHECK(X) do { \
                                   EGLint error = eglGetError(); \
                                   if (!(X) || error != EGL_SUCCESS) { \
                                       __android_log_print(ANDROID_LOG_ERROR, TAG, \
                                          "EGL error '%d' at %s:%d", error, __FILE__, __LINE__);\
                                       return false; \
                                    } \
                            } while (0)

#endif //ANDROIDOPENGLEXAMPLES_EGLCOMMONDEFINE_H
