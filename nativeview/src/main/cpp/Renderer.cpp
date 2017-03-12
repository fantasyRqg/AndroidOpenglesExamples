//
// Created by ranqingguo on 3/10/17.
//

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

bool Renderer::render() {
    if (mEnable) {
        return renderInternal();
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



