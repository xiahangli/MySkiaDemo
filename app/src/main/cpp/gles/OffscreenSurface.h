//
// Created by cw on 2018/7/24.
//

#ifndef EGLDEMO_OFFSCREENSURFACE_H
#define EGLDEMO_OFFSCREENSURFACE_H

#include "./EglSurfaceBase.h"

class OffscreenSurface : public EglSurfaceBase{

public:
    OffscreenSurface(EglCore* eglCore,int width, int height);
    void release();

};


#endif //EGLDEMO_OFFSCREENSURFACE_H
