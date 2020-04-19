//
// Created by cw on 2018/7/24.
//

#ifndef EGLDEMO_WINDOWSURFACE_H
#define EGLDEMO_WINDOWSURFACE_H

#include "./EglSurfaceBase.h"

class WindowSurface : public EglSurfaceBase {

public:
    WindowSurface(ANativeWindow* nativeWindow, EglCore* eglCore, bool releaseSurface);
    WindowSurface(ANativeWindow* nativeWindow, EglCore* eglCore);

    void release();

    void recreate(EglCore* eglCore);

private:
    ANativeWindow *mSurface;
    bool mReleaseSurface;
};


#endif //EGLDEMO_WINDOWSURFACE_H
