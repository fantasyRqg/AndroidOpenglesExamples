//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_TRIANGLERENDERER_H
#define ANDROIDOPENGLEXAMPLES_TRIANGLERENDERER_H


#include "../Renderer.h"

class TriangleRenderer : public Renderer {
protected:
    virtual bool setUpInternal() override;

    virtual bool tearDownInternal() override;

    virtual bool renderInternal(long timestampMills) override;

public:
    virtual ~TriangleRenderer() override;


private:
    GLuint mProgram;
    GLuint mVertexArray;
    GLuint mVertexBuffer;
};


#endif //ANDROIDOPENGLEXAMPLES_TRIANGLERENDERER_H
