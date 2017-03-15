//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_RENDERER_H
#define ANDROIDOPENGLEXAMPLES_RENDERER_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <android/asset_manager.h>
#include <common.h>

#undef TAG
#define TAG "Renderer"

class Renderer {
public:


    virtual ~Renderer();

    int getZOrder() const;

    void setZOrder(int ZOrder);

    bool isEnable() const;

    void setEnable(bool enable);

    bool setUp(AAssetManager *mgr);

    bool tearDown();

    bool render();

    bool operator=(const Renderer &renderer) const;

    bool operator<(const Renderer &renderer) const;

    GLuint loadShader(AAssetManager *amgr, const char *fname, GLenum type);

private:
    int mZOrder;
    bool mEnable = true;
    bool mSetUped;


protected:


    virtual bool setUpInternal(AAssetManager *mgr) = 0;

    virtual bool tearDownInternal() = 0;

    virtual bool renderInternal() = 0;

};


#endif //ANDROIDOPENGLEXAMPLES_RENDERER_H
