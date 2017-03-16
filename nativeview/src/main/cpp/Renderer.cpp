//
// Created by ranqingguo on 3/10/17.
//

#include <stdlib.h>

#include "Renderer.h"


int Renderer::getZOrder() const {
    return mZOrder;
}

void Renderer::setZOrder(int ZOrder) {
    mZOrder = ZOrder;
}

bool Renderer::isEnable() const {
    return mEnable;
}

void Renderer::setEnable(bool enable) {
    mEnable = enable;
}

bool Renderer::setUp() {
    if (!mSetUped)
        mSetUped = setUpInternal();

    return mSetUped;
}

bool Renderer::tearDown() {
    mSetUped = false;
    return tearDownInternal();

}

bool Renderer::render(long timestampNs) {
    if (mEnable) {
        return renderInternal(timestampNs);
    } else {
        return false;
    }
}

bool Renderer::operator=(const Renderer &renderer) const {
    return false;
}

bool Renderer::operator<(const Renderer &renderer) const {
    return false;
}

Renderer::~Renderer() {

}


GLuint Renderer::loadShader(AAssetManager *amgr, const char *fname, GLenum type) {

    GLuint id = 0;
    id = glCreateShader(type);

    //open file
    AAsset *asset = AAssetManager_open(amgr, fname, AASSET_MODE_UNKNOWN);
    if (!asset) {
        LOGE("asset get  failure");
        return 0;
    }
    size_t size = (size_t) AAsset_getLength(asset);
    GLchar *buffer = (GLchar *) malloc(size + 1);
    memset(buffer, 0, size + 1);

    if (!AAsset_read(asset, buffer, size)) {
        LOGE("read shader file error");
        return 0;
    }

    //set shader source
    glShaderSource(id, 1, (const GLchar *const *) &buffer, NULL);

    //close file
    AAsset_close(asset);
    free(buffer);

    //compile shader
    glCompileShader(id);

    //check error
    if (glCommon::checkShaderAndPrint(id)) {
        return id;
    } else {
        LOGE("shader name %s", fname);
        return 0;
    }
}

void Renderer::setEglWrapper(EGLWrapper *wrapper) {
    mEglWrapper = wrapper;

}



