//
// Created by cw on 2018/7/24.
//

#include "EglSurfaceBase.h"
#include "assert.h"
#include "../common/native_log.h"
#include <GLES2/gl2.h>

EglSurfaceBase::EglSurfaceBase(EglCore *eglCore) {
    mEglCore = eglCore;
}

void EglSurfaceBase::createWindowSurface(ANativeWindow *nativeWindow) {
    assert(mEGLSurface == EGL_NO_SURFACE);
    if (mEGLSurface != EGL_NO_SURFACE){
        ALOGD("createWindowSurface: surface already created");
        return;
    }
    mEGLSurface = mEglCore->createWindowSurface(nativeWindow);
}

void EglSurfaceBase::createOffscreenSurface(int width, int height) {
    assert(mEGLSurface == EGL_NO_SURFACE);
    if (mEGLSurface != EGL_NO_SURFACE){
        ALOGD("createOffscreenSurface : surface already created");
        return;
    }
    mEGLSurface = mEglCore->createOffscreenSurface(width,height);
    mWidth = width;
    mHeight = height;
}

int EglSurfaceBase::getHeight() {
    if (mHeight <0){
        return mEglCore->querySurface(mEGLSurface,EGL_HEIGHT);
    }
    return mHeight;
}

int EglSurfaceBase::getWidth() {
    if (mWidth<0){
        return mEglCore->querySurface(mEGLSurface,EGL_WIDTH);
    }
    return mWidth;
}

void EglSurfaceBase::releaseEglSurface() {
    mEglCore->releaseSurface(mEGLSurface);
    mEGLSurface = EGL_NO_SURFACE;
    mWidth = mHeight = -1;
}

void EglSurfaceBase::makeCurrent() {
    mEglCore->makeCurrent(mEGLSurface);
}

void EglSurfaceBase::swapBuffer() {
    return mEglCore->swapBuffers(mEGLSurface);
}

//获取当前像素
char *EglSurfaceBase::getCurrentFrame() {
    char* pixels = NULL;
    glReadPixels(0,0,getWidth(),getHeight(),GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    return pixels;
}
