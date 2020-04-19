//
// Created by cw on 2018/7/24.
//

#ifndef EGLDEMO_GLRENDER_H
#define EGLDEMO_GLRENDER_H

#include <android/native_window.h>
#include "./shape/Triangle.h"
#include "EglCore.h"
#include "WindowSurface.h"
#include "../skia/Canvas.h"

class GLRender {

public:
    GLRender();
    ~GLRender();
    void surfaceCreated(ANativeWindow* window);
    void surfaceChanged(int width, int height);
    void surfaceDestroyed();

private:
    WindowSurface* mWindowSurface;
    EglCore* mEglCore;
    Canvas *canvas;
    Triangle *triangle;
};


#endif //EGLDEMO_GLRENDER_H
