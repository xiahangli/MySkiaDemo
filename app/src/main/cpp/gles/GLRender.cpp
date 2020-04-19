//
// Created by cw on 2018/7/24.
//

#include <assert.h>
#include "GLRender.h"

GLRender::GLRender() {
    mEglCore = NULL;
    mWindowSurface = NULL;
}

GLRender::~GLRender() {
    if (mEglCore){
        mEglCore->release();
        delete mEglCore;
        mEglCore = NULL;
    }
}

void GLRender::surfaceCreated(ANativeWindow *window) {
    if (mEglCore == NULL){
        mEglCore = new EglCore(NULL,FLAG_RECORDABLE);
        mWindowSurface = new WindowSurface(window,mEglCore, false);
        assert(mWindowSurface != NULL && mEglCore != NULL);
        mWindowSurface->makeCurrent();


        canvas = new Canvas();
        glClearColor(0.0f,0.0f,1.0f,1.0f);
    }
}

void GLRender::surfaceChanged(int width, int height) {
    mWindowSurface->makeCurrent();
    canvas->onDraw(width,height);
    mWindowSurface->swapBuffer();
}

void GLRender::surfaceDestroyed() {
    if (mWindowSurface){
        mWindowSurface->release();
        delete mWindowSurface;
        mWindowSurface = NULL;
    }
    if (mEglCore){
        mEglCore->release();
        delete mEglCore;
        mEglCore = NULL;
    }
    if (canvas){
        canvas->onDestroy();
        delete canvas;
        canvas = nullptr;
    }
}
