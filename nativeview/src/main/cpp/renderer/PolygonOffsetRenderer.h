//
// Created by ranqingguo on 3/18/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_POLYGONOFFSET_H
#define ANDROIDOPENGLEXAMPLES_POLYGONOFFSET_H

#include "../Renderer.h"

#undef TAG
#define TAG "PolygonOffsetRenderer"

class PolygonOffsetRenderer : public Renderer {
protected:
    virtual bool setUpInternal();

    virtual bool tearDownInternal();

    virtual bool renderInternal(long timestampMills);

private:
    GLuint mProgram;
    GLuint mVertexArray;
    GLuint mVbo;
    GLuint mIndicatesVbo[2];

    GLint mMvpLocation;

    glm::mat4 mProjMatrix;
    glm::mat4 mViewMatrix;
};


#endif //ANDROIDOPENGLEXAMPLES_POLYGONOFFSET_H
