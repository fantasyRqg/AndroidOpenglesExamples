//
// Created by ranqingguo on 3/16/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_INSTANCERENDERER_H
#define ANDROIDOPENGLEXAMPLES_INSTANCERENDERER_H

#include "../Renderer.h"

#define NUM_INSTANCES   100

#undef TAG
#define TAG "InstanceRenderer"

class InstanceRenderer : public Renderer {
protected:
    virtual bool setUpInternal() override;

    virtual bool tearDownInternal() override;

    virtual bool renderInternal(long timestampNs) override;

    int esGenCube(float scale, GLfloat **vertices, GLfloat **normals,
                  GLfloat **texCoords, GLuint **indices);

private:

    void update(long timestampNs);

    GLuint mProgram;
    GLuint mVAO;
    GLuint mMvpVBO;
    GLuint mIndicateVBO;


    GLuint mPositionVBO;


    GLuint mColorVBO;
    GLfloat mAngle[NUM_INSTANCES];

    int mNumIndicateds;
};


#endif //ANDROIDOPENGLEXAMPLES_INSTANCERENDERER_H
