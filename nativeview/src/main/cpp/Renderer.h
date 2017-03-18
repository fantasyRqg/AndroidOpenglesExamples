//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_RENDERER_H
#define ANDROIDOPENGLEXAMPLES_RENDERER_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <common.h>


#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>

#include "EGLWrapper.h"

#undef TAG
#define TAG "Renderer"

class EGLWrapper;

class Renderer {
public:


    virtual ~Renderer();

    int getZOrder() const;

    void setZOrder(int ZOrder);

    bool isEnable() const;

    void setEnable(bool enable);

    bool setUp();

    bool tearDown();

    bool render(long timestampMills);

    bool operator=(const Renderer &renderer) const;

    bool operator<(const Renderer &renderer) const;

    GLuint loadShader(AAssetManager *amgr, const char *fname, GLenum type);

    void setEglWrapper(EGLWrapper *wrapper);

private:
    int mZOrder;
    bool mEnable = true;
    bool mSetUped;


protected:
    EGLWrapper *mEglWrapper;


    virtual bool setUpInternal() = 0;

    virtual bool tearDownInternal() = 0;

    virtual bool renderInternal(long timestampMills) = 0;

    float getAspect();

};


#endif //ANDROIDOPENGLEXAMPLES_RENDERER_H
