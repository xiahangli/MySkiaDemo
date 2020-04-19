//
// Created by cw on 2018/7/24.
//

#include "OffscreenSurface.h"

OffscreenSurface::OffscreenSurface(EglCore *eglCore, int width, int height) : EglSurfaceBase(eglCore){
    createOffscreenSurface(width,height);
}

void OffscreenSurface::release() {
    releaseEglSurface();
}
