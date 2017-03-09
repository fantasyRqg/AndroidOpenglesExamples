//
// Created by ranqingguo on 3/9/17.
//

#include <stdlib.h>

#include "common.h"


namespace glCommon {

#ifndef TAG
#define TAG "glCommon"

    bool checkShaderAndPrint(GLuint shader) {

        GLint compiled = GL_FALSE;
        // Check the compile status
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
            GLint infoLen = 0;

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);

                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                LOGE("Error compiling shader:\n%s\n", infoLog);
                free(infoLog);
            }

            glDeleteShader(shader);
            return false;
        }

        return true;
    }

    bool checkProgramAndPrint(GLuint program) {
        GLint linked;
        // Check the link status
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetProgramInfoLog(program, infoLen, NULL, infoLog);
                LOGE("Error linking program:\n%s\n", infoLog);
                free(infoLog);
            }
            glDeleteProgram(program);
            return false;
        }

        return true;
    }


    bool checkGlError(const char *funcName) {
        GLint err = glGetError();
        if (err != GL_NO_ERROR) {
            LOGE("GL error after %s(): 0x%08x\n", funcName, err);
            return true;
        }
        return false;
    }

#undef TAG
#endif
}
