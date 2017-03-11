//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_TRIANGLERENDERER_H
#define ANDROIDOPENGLEXAMPLES_TRIANGLERENDERER_H


#include "../Renderer.h"

class TriangleRenderer : public Renderer {
protected:
    virtual bool setUpInternal();

    virtual bool tearDownInternal();

    virtual bool renderInternal();

private:
    GLuint mProgram;
    GLuint mVertexArray;
    GLuint mVertexBuffer;
};


#endif //ANDROIDOPENGLEXAMPLES_TRIANGLERENDERER_H
