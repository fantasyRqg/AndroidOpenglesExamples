//
// Created by ranqingguo on 3/10/17.
//

#ifndef ANDROIDOPENGLEXAMPLES_RENDERER_H
#define ANDROIDOPENGLEXAMPLES_RENDERER_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>

class Renderer {
public:

    int getZOrder() const;

    void setZOrder(int ZOrder);

    bool isEnable() const;

    void setEnable(bool enable);

    bool setUp();

    bool tearDown();

    bool render();

    bool operator=(const Renderer &renderer) const;

    bool operator<(const Renderer &renderer) const;

private:
    int mZOrder;
    bool mEnable;
    bool mSetUped;


protected:

    virtual bool setUpInternal() = 0;

    virtual bool tearDownInternal() = 0;

    virtual bool renderInternal() = 0;

};


#endif //ANDROIDOPENGLEXAMPLES_RENDERER_H
