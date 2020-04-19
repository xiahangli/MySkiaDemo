//
// Created by templechen on 2018/7/26.
//

#ifndef SKIADEMO_CANVAS_H
#define SKIADEMO_CANVAS_H

#include <skia/core/SkRefCnt.h>
#include <skia/core/SkSurface.h>
#include <skia/gpu/GrContext.h>


class Canvas {

public:
    Canvas();
    ~Canvas();
    void init();
    void onDraw(int width, int height);
    void onDestroy();

private:
    sk_sp<SkSurface> skia_surface = nullptr;
    sk_sp<GrContext> context = nullptr;
};


#endif //SKIADEMO_CANVAS_H
