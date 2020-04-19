//
// Created by cw on 2018/7/24.
//

#include "WindowSurface.h"
#include "../common/native_log.h"

WindowSurface::WindowSurface(ANativeWindow *nativeWindow, EglCore *eglCore, bool releaseSurface):EglSurfaceBase(eglCore) {
    mSurface = nativeWindow;
    createWindowSurface(mSurface);
    mReleaseSurface = releaseSurface;
}

WindowSurface::WindowSurface(ANativeWindow *nativeWindow, EglCore *eglCore) :EglSurfaceBase(eglCore){
    mSurface = nativeWindow;
    createWindowSurface(mSurface);
}

void WindowSurface::release() {
    releaseEglSurface();
    if (mSurface != NULL){
        ANativeWindow_release(mSurface);
        mSurface = NULL;
    }
}

void WindowSurface::recreate(EglCore *eglCore) {
    if (mSurface == NULL){
        ALOGD("not yet implements ANativeWindow");
        return;
    }
    mEglCore = eglCore;
    createWindowSurface(mSurface);
}
