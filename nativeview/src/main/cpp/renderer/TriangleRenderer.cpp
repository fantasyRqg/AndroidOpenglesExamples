//
// Created by ranqingguo on 3/10/17.
//

#include "TriangleRenderer.h"

#undef TAG
#define TAG "TriangleRenderer"

bool TriangleRenderer::setUpInternal() {

    AAssetManager *mgr = mEglWrapper->getAssetManager();

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    GLuint fragmentShader = loadShader(mgr, "triangle.fs.glsl", GL_FRAGMENT_SHADER);
    GLuint vertexShader = loadShader(mgr, "triangle.vs.glsl", GL_VERTEX_SHADER);


    mProgram = glCreateProgram();
    glAttachShader(mProgram, fragmentShader);
    glAttachShader(mProgram, vertexShader);
    glLinkProgram(mProgram);
    glCommon::checkGlError("glLinkProgram");
    glCommon::checkProgramAndPrint(mProgram);
    glCommon::checkGlError("checkProgramAndPrint");


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ///////////////////////////////////////////



    GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
                           -0.5f, -0.5f, 0.0f,
                           0.5f, -0.5f, 0.0f

    };

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), (const void *) vVertices,
                 GL_STATIC_DRAW);

    glClearColor(1, 1, 1, 1);

    return true;
}

bool TriangleRenderer::tearDownInternal() {
    LOGD("tearDownInternal");

    glDeleteProgram(mProgram);

    glDeleteVertexArrays(1, &mVertexArray);
    glDeleteBuffers(1, &mVertexBuffer);
    return true;
}

bool TriangleRenderer::renderInternal() {
    glUseProgram(mProgram);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    return true;
}

TriangleRenderer::~TriangleRenderer() {

}
