//
// Created by ranqingguo on 3/9/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_RENDERSELF_H
#define ANDROIDOPENGLEXAMPLES_RENDERSELF_H


#include <EGL/egl.h>
#include <common.h>


class RenderSelf {
public:
    RenderSelf();

    ~RenderSelf();

    void resize(int w, int h);

    void render();


private:
    void init();

    void initEGL();

    const EGLContext mEglContext;
    GLuint mProgram;
    GLuint mVertexArray;
    GLuint mVertexBuffer;

};

#endif //ANDROIDOPENGLEXAMPLES_RENDERSELF_H
