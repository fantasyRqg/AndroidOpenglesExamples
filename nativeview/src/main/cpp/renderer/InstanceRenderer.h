//
// Created by ranqingguo on 3/16/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_INSTANCERENDERER_H
#define ANDROIDOPENGLEXAMPLES_INSTANCERENDERER_H

#include "../Renderer.h"


class InstanceRenderer : public Renderer {
protected:
    virtual bool setUpInternal() override;

    virtual bool tearDownInternal() override;

    virtual bool renderInternal() override;

private:
    GLuint  mPorgram;
    GLuint mVAO;


};


#endif //ANDROIDOPENGLEXAMPLES_INSTANCERENDERER_H
