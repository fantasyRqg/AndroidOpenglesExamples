//
// Created by ranqingguo on 3/9/17.
//

#include "RenderSelf.h"

RenderSelf::RenderSelf()
        : mEglContext(eglGetCurrentContext()) {

    init();

}

RenderSelf::~RenderSelf() {
    if (eglGetCurrentContext() != mEglContext)
        return;
    glDeleteProgram(mProgram);
    glDeleteVertexArrays(1, &mVertexArray);
    glDeleteBuffers(1, &mVertexBuffer);
}

void RenderSelf::init() {


    char vertexShaderStr[] = "#version 300 es\n"
            "\n"
            "layout(location = 0) in vec3 vPosition;\n"
            "\n"
            "void main() {\n"
            "    gl_Position = vec4(vPosition,1.0);\n"
            "}";

    char fShaderStr[] = "#version 300 es\n"
            "precision mediump float;\n"
            "\n"
            "out vec4 fragColor;\n"
            "\n"
            "\n"
            "void main() {\n"
            "    fragColor = vec4(1.0,0.0,0.0,1.0);\n"
            "}";

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    GLuint vertexSahder;
    GLuint fragmentSahder;

    vertexSahder = glCreateShader(GL_VERTEX_SHADER);
    const char *pTmpv = vertexShaderStr;
    glShaderSource(vertexSahder, 1, (const GLchar *const *) &pTmpv, NULL);
    glCompileShader(vertexSahder);
    glCommon::checkShaderAndPrint(vertexSahder);

    const char *pTmpf = fShaderStr;

    fragmentSahder = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentSahder, 1, (const GLchar *const *) &pTmpf, NULL);
    glCompileShader(fragmentSahder);
    glCommon::checkShaderAndPrint(fragmentSahder);


    mProgram = glCreateProgram();
    glAttachShader(mProgram, vertexSahder);
    glAttachShader(mProgram, fragmentSahder);

    glLinkProgram(mProgram);
    glCommon::checkProgramAndPrint(mProgram);


    ///////////////////////////////////////////



    GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
                           -0.5f, -0.5f, 0.0f,
                           0.5f, -0.5f, 0.0f

    };

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), (const void *) vVertices,
                 GL_STATIC_DRAW);


    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);


}


void RenderSelf::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mProgram);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glCommon::checkGlError("glDrawArrays");
}


void RenderSelf::initEGL() {


}

void RenderSelf::resize(int w, int h) {
    glViewport(0, 0, w, h);
}


