//
// Created by templechen on 2018/7/26.
//

#include <skia/core/SkGraphics.h>
#include <skia/gpu/gl/GrGLInterface.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkSurface.h>
#include <GLES2/gl2.h>
#include "Canvas.h"
#include "../common/native_log.h"
#include "string"

Canvas::Canvas() {

}

Canvas::~Canvas() {

}

void Canvas::init() {



}

void Canvas::onDraw(int width, int height) {
    if (skia_surface == nullptr){
        sk_sp<const GrGLInterface> interface(GrGLMakeNativeInterface());
        context = GrContext::MakeGL(interface);
        SkImageInfo info = SkImageInfo::MakeN32(width,height,kOpaque_SkAlphaType);
        skia_surface = SkSurface::MakeRenderTarget(context.get(),SkBudgeted::kNo,info,0,kBottomLeft_GrSurfaceOrigin,
                                                   nullptr);
    }

    SkCanvas* canvas = skia_surface->getCanvas();

    ALOGD("!!!");
    ALOGD("%x",skia_surface->width());
    ALOGD("%x",skia_surface->height());

    canvas->clear(SK_ColorWHITE);

    SkPaint paint;
    paint.setColor(SK_ColorRED);

    // Draw a rectangle with red paint
    SkRect rect = SkRect::MakeXYWH(10, 10, 128, 128);
    canvas->drawRect(rect, paint);

}

void Canvas::onDestroy() {

}
