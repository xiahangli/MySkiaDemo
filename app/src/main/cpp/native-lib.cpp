#include <jni.h>
#include "MyLooper.h"
#include <android/native_window_jni.h>
#include <android/bitmap.h>
#include <skia/core/SkPaint.h>
#include <skia/core/SkRefCnt.h>
#include <skia/core/SkSurface.h>
#include <skia/core/SkCanvas.h>
#include <skia/effects/SkDiscretePathEffect.h>
#include <skia/effects/SkGradientShader.h>
#include <linux/fb.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <native_log.h>

//fb
#ifndef __ANDROID__
#define FRAMEBUFFER "/dev/fb0"
#define OUT_DIR "/tmp"
#else
#define FRAMEBUFFER "/dev/graphics/fb0"
#define OUT_DIR "/sdcard/"
#endif


struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *frameBuffer = 0;
#define SKIA_SCALE 0
//打印fb驱动中fix结构信息，注：在fb驱动加载后，fix结构不可被修改。
void printFixedInfo()
{
    ALOGD("Fixed screen info:\n"
            "\tid: %s\n"
            "\tsmem_start: 0x%lx\n"
            "\tsmem_len: %d\n"
            "\ttype: %d\n"
            "\ttype_aux: %d\n"
            "\tvisual: %d\n"
            "\txpanstep: %d\n"
            "\typanstep: %d\n"
            "\tywrapstep: %d\n"
            "\tline_length: %d\n"
            "\tmmio_start: 0x%lx\n"
            "\tmmio_len: %d\n"
            "\taccel: %d\n"
            "\n", finfo.id, finfo.smem_start, finfo.smem_len, finfo.type,
            finfo.type_aux, finfo.visual, finfo.xpanstep, finfo.ypanstep,
            finfo.ywrapstep, finfo.line_length, finfo.mmio_start,
            finfo.mmio_len, finfo.accel);
}

//打印fb驱动中var结构信息，注：fb驱动加载后，var结构可根据实际需要被重置
void printVariableInfo()
{
    ALOGD("Variable screen info:\n"
            "\txres: %d\n"
            "\tyres: %d\n"
            "\txres_virtual: %d\n"
            "\tyres_virtual: %d\n"
            "\tyoffset: %d\n"
            "\txoffset: %d\n"
            "\tbits_per_pixel: %d\n"
            "\tgrayscale: %d\n"
            "\tred: offset: -, length: -, msb_right: -\n"
            "\tgreen: offset: -, length: -, msb_right: -\n"
            "\tblue: offset: -, length: -, msb_right: -\n"
            "\ttransp: offset: -, length: -, msb_right: -\n"
            "\tnonstd: %d\n"
            "\tactivate: %d\n"
            "\theight: %d\n"
            "\twidth: %d\n"
            "\taccel_flags: 0x%x\n"
            "\tpixclock: %d\n"
            "\tleft_margin: %d\n"
            "\tright_margin: %d\n"
            "\tupper_margin: %d\n"
            "\tlower_margin: %d\n"
            "\thsync_len: %d\n"
            "\tvsync_len: %d\n"
            "\tsync: %d\n"
            "\tvmode: %d\n"
            "\n", vinfo.xres, vinfo.yres, vinfo.xres_virtual,
            vinfo.yres_virtual, vinfo.xoffset, vinfo.yoffset,
            vinfo.bits_per_pixel, vinfo.grayscale, vinfo.red.offset,
            vinfo.red.length, vinfo.red.msb_right, vinfo.green.offset,
            vinfo.green.length, vinfo.green.msb_right, vinfo.blue.offset,
            vinfo.blue.length, vinfo.blue.msb_right, vinfo.transp.offset,
            vinfo.transp.length, vinfo.transp.msb_right, vinfo.nonstd,
            vinfo.activate, vinfo.height, vinfo.width, vinfo.accel_flags,
            vinfo.pixclock, vinfo.left_margin, vinfo.right_margin,
            vinfo.upper_margin, vinfo.lower_margin, vinfo.hsync_len,
            vinfo.vsync_len, vinfo.sync, vinfo.vmode);
}
//画大小为width*height的同色矩阵，8alpha+8reds+8greens+8blues
void drawRect_rgb32(int x0, int y0, int width, int height, int color)
{
    const int bytesPerPixel = 4;
    const int stride = finfo.line_length / bytesPerPixel;

    int *dest = (int *) (frameBuffer) + (y0 + vinfo.yoffset) * stride
            + (x0 + vinfo.xoffset);

    int x, y;
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            dest[x] = color;
        }
        dest += stride;
    }
}

//画大小为width*height的同色矩阵，5reds+6greens+5blues
void drawRect_rgb16(int x0, int y0, int width, int height, int color)
{
    const int bytesPerPixel = 2;
    const int stride = finfo.line_length / bytesPerPixel;
    const int red = (color & 0xff0000) >> (16 + 3);
    const int green = (color & 0xff00) >> (8 + 2);
    const int blue = (color & 0xff) >> 3;
    const short color16 = blue | (green << 5) | (red << (5 + 6));

    short *dest = (short *) (frameBuffer) + (y0 + vinfo.yoffset) * stride
            + (x0 + vinfo.xoffset);

    int x, y;
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            dest[x] = color16;
        }
        dest += stride;
    }
}

//画大小为width*height的同色矩阵，5reds+5greens+5blues
void drawRect_rgb15(int x0, int y0, int width, int height, int color)
{
    const int bytesPerPixel = 2;
    const int stride = finfo.line_length / bytesPerPixel;
    const int red = (color & 0xff0000) >> (16 + 3);
    const int green = (color & 0xff00) >> (8 + 3);
    const int blue = (color & 0xff) >> 3;
    const short color15 = blue | (green << 5) | (red << (5 + 5)) | 0x8000;

    short *dest = (short *) (frameBuffer) + (y0 + vinfo.yoffset) * stride
            + (x0 + vinfo.xoffset);

    int x, y;
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            dest[x] = color15;
        }
        dest += stride;
    }
}

void drawRect(int x0, int y0, int width, int height, int color)
{
    switch (vinfo.bits_per_pixel)
    {
    case 32:
        drawRect_rgb32(x0, y0, width, height, color);
        break;
    case 16:
        drawRect_rgb16(x0, y0, width, height, color);
        break;
    case 15:
        drawRect_rgb15(x0, y0, width, height, color);
        break;
    default:
        printf("Warning: drawRect() not implemented for color depth %i\n",
                vinfo.bits_per_pixel);
        break;
    }
}

#define PERFORMANCE_RUN_COUNT 5
void performSpeedTest(void *fb, int fbSize)
{
    int i, j, run;
    struct timeval startTime, endTime;
    unsigned long long results[PERFORMANCE_RUN_COUNT];
    unsigned long long average;
    unsigned int *testImage;

    unsigned int randData[17] =
    { 0x3A428472, 0x724B84D3, 0x26B898AB, 0x7D980E3C, 0x5345A084, 0x6779B66B,
            0x791EE4B4, 0x6E8EE3CC, 0x63AF504A, 0x18A21B33, 0x0E26EB73,
            0x022F708E, 0x1740F3B0, 0x7E2C699D, 0x0E8A570B, 0x5F2C22FB,
            0x6A742130 };

    printf("Frame Buffer Performance test...\n");
    for (run = 0; run < PERFORMANCE_RUN_COUNT; ++run)
    {

        testImage = (unsigned int *) malloc(fbSize);
        j = run;
        for (i = 0; i < (int) (fbSize / sizeof(int)); ++i)
        {
            testImage[i] = randData[j];
            j++;
            if (j >= 17)
                j = 0;
        }

        gettimeofday(&startTime, NULL);
        memcpy(fb, testImage, fbSize);
        gettimeofday(&endTime, NULL);

        long secsDiff = endTime.tv_sec - startTime.tv_sec;
        results[run] = secsDiff * 1000000
                + (endTime.tv_usec - startTime.tv_usec);

        free(testImage);
    }

    average = 0;
    for (i = 0; i < PERFORMANCE_RUN_COUNT; ++i)
        average += results[i];
    average = average / PERFORMANCE_RUN_COUNT;

    printf(" Average: %llu usecs\n", average);
    printf(" Bandwidth: %.03f MByte/Sec\n",
            (fbSize / 1048576.0) / ((double) average / 1000000.0));
    printf(" Max. FPS: %.03f fps\n\n", 1000000.0 / (double) average);

    memset(fb, 0, fbSize);
}


//fbe
MyLooper *myLooper = nullptr;
ANativeWindow *nativeWindow = nullptr;

void draw(SkCanvas* canvas) ;

void drawColor(SkCanvas* canvas) ;

void drawPath(SkCanvas* canvas) ;

void drawTransform(SkCanvas* canvas) ;

void drawText(SkCanvas* canvas) ;

void drawBezier(SkCanvas* canvas) ;

extern "C" JNIEXPORT void JNICALL
Java_com_example_myskiademo_MainActivity1_initFB(JNIEnv *env, jobject instance) {
 const char *devfile = FRAMEBUFFER;
    long int screensize = 0;
    int fbFd = 0;
    fbFd = open(devfile, O_RDWR);
//    fbFd =  open(devfile,O_RDONLY);
    if (fbFd == -1)
    {
        ALOGE("Error: cannot open framebuffer device");
        exit(1);
    }

    //获取finfo信息并显示
    if (ioctl(fbFd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        ALOGE("Error reading fixed information");
        exit(2);
    }
    printFixedInfo();
    //获取vinfo信息并显示
    if (ioctl(fbFd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        ALOGE("Error reading variable information");
        exit(3);
    }
    printVariableInfo();

    screensize = finfo.smem_len;

    frameBuffer = (char *) mmap(0, screensize, PROT_READ | PROT_WRITE,
            MAP_SHARED,//与其它所有映射这个对象的进程共享映射空间
            // 。对共享区的写入，相当于输出到文件
            // 。直到msync()或者munmap()被调用，文件实际上不会被更新。
            fbFd, 0);
//    int error =1000000;
    if (frameBuffer == MAP_FAILED )
    {
        ALOGE("framebuffer失败：%s",strerror(errno));

        ALOGE("Error: Failed to map framebuffer device to memory");
//        exit(4);
    }

    //测试virt fb的性能
    performSpeedTest(frameBuffer, screensize);
    memset(frameBuffer, 0, screensize);

    ALOGD("Will draw 3 rectangles on the screen,\n"
            "they should be colored red, green and blue (in that order).\n");
    drawRect(vinfo.xres / 8, vinfo.yres / 8, vinfo.xres / 4, vinfo.yres / 4,
            0xffff0000);
    drawRect(vinfo.xres * 3 / 8, vinfo.yres * 3 / 8, vinfo.xres / 4,
            vinfo.yres / 4, 0xff00ff00);
    drawRect(vinfo.xres * 5 / 8, vinfo.yres * 5 / 8, vinfo.xres / 4,
            vinfo.yres / 4, 0xff0000ff);

    sleep(5);
    ALOGD(" Done.\n");
    memset(frameBuffer, 0, screensize);
}



extern "C" JNIEXPORT void JNICALL
Java_com_example_myskiademo_MainActivity_nativeInit(JNIEnv *env, jobject instance) {
    myLooper = new MyLooper();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myskiademo_MainActivity_nativeSurfaceCreated(JNIEnv *env,
                                                                       jobject instance,
                                                                       jobject surface) {
    if (nativeWindow != nullptr){
        ANativeWindow_release(nativeWindow);
        nativeWindow = nullptr;
    }
    nativeWindow = ANativeWindow_fromSurface(env,surface);
    if (myLooper!= nullptr){
        myLooper->postMessage(kMsgSurfaceCreated,nativeWindow);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myskiademo_MainActivity_nativeSurfaceChanged(JNIEnv *env,
                                                                       jobject instance, jint width,
                                                                       jint height) {
    if (myLooper != nullptr){
        myLooper->postMessage(kMsgSurfaceChanged,width,height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myskiademo_MainActivity_nativeSurfaceDestroyed(JNIEnv *env,
                                                                         jobject instance) {
    if (myLooper != nullptr){
        myLooper->postMessage(kMsgSurfaceDestroyed);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myskiademo_MainActivity_nativeDestroy(JNIEnv *env, jobject instance) {
    if (myLooper != nullptr){
        myLooper->quit();
        delete myLooper;
        myLooper = nullptr;
    }
    if (nativeWindow != nullptr){
        ANativeWindow_release(nativeWindow);
        nativeWindow = nullptr;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myskiademo_MainActivity1_drawIntoBitmap(JNIEnv *env, jobject instance,
                                                                      jobject dstBitmap,
                                                                      jlong elapsedTime) {
    // Grab the dst bitmap info and pixels
    AndroidBitmapInfo dstInfo;
    void* dstPixels;

    AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

    SkImageInfo info = SkImageInfo::MakeN32Premul(dstInfo.width, dstInfo.height);

    // Create a surface from the given bitmap
    sk_sp<SkSurface> surface(SkSurface::MakeRasterDirect(info, dstPixels, dstInfo.stride));
    SkCanvas* canvas = surface->getCanvas();

    drawPath(canvas);

    draw(canvas);

    drawColor(canvas);

    drawText(canvas);

    drawBezier(canvas);

    drawTransform(canvas);
    // Unlock the dst's pixels
    AndroidBitmap_unlockPixels(env, dstBitmap);
}

SkPath star() {
    const SkScalar R = 115.2f, C = 128.0f*5;
    SkPath path;
    path.moveTo(C + R, C);
    for (int i = 1; i < 8; ++i) {
        SkScalar a = 2.6927937f * i;
        path.lineTo(C + R * cos(a), C + R * sin(a));
    }
    return path;
}
void draw(SkCanvas* canvas) {
    SkPaint paint;
    paint.setPathEffect(SkPathEffect::MakeSum(
            SkDiscretePathEffect::Make(10.0f, 4.0f),
            SkDiscretePathEffect::Make(10.0f, 4.0f, 1245u)
    ));
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(2.0f);
    paint.setAntiAlias(true);
    paint.setColor(0xff4281A4);
    SkPath path(star());
    canvas->drawPath(path, paint);
}

SkPath starColor() {
    const SkScalar R = 60.0f, C = 128.0f*4;
    SkPath path;
    path.moveTo(C + R, C);
    for (int i = 1; i < 15; ++i) {
        SkScalar a = 0.44879895f * i;
        SkScalar r = R + R * (i % 2);
        path.lineTo(C + r * cos(a), C + r * sin(a));
    }
    return path;
}


void drawColor(SkCanvas* canvas) {
    SkPaint paint;
    paint.setPathEffect(SkDiscretePathEffect::Make(10.0f, 4.0f));
    SkPoint points[2] = {
            SkPoint::Make(0.0f, 0.0f),
            SkPoint::Make(256.0f, 256.0f)
    };
    SkColor colors[2] = {SkColorSetRGB(0xEA, 0xD2, 0xAC), SkColorSetRGB(0x42, 0x81, 0xA4)};
    paint.setShader(SkGradientShader::MakeLinear(
            points, colors, NULL, 2,
            SkShader::kClamp_TileMode, 0, NULL));
    paint.setAntiAlias(true);
    SkPath path(starColor());
    canvas->drawPath(path, paint);
}

void drawPath(SkCanvas* canvas) {
    canvas->drawColor(SK_ColorWHITE);

    SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setAntiAlias(true);
    paint.setStrokeWidth(4);
    paint.setColor(0xffFE938C);

    SkRect rect = SkRect::MakeXYWH(10, 10, 100, 160);
    canvas->drawRect(rect, paint);

    SkRRect oval;
    oval.setOval(rect);
    oval.offset(40, 80);
    paint.setColor(0xffE6B89C);
    canvas->drawRRect(oval, paint);

    paint.setColor(0xff9CAFB7);
    canvas->drawCircle(180, 50, 25, paint);

    rect.offset(80, 50);
    paint.setColor(0xff4281A4);
    paint.setStyle(SkPaint::kStroke_Style);
    canvas->drawRoundRect(rect, 10, 10, paint);
}

void drawTransform(SkCanvas* canvas) {
    canvas->translate(128, 0);
    canvas->rotate(60);
    SkRect rect = SkRect::MakeXYWH(300, 0, 200, 100);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(0xff4281A4);
    canvas->drawRect(rect, paint);

    canvas->rotate(SkIntToScalar(20));
    paint.setColor(0xffFE938C);
    canvas->drawRect(rect, paint);
}

void drawText(SkCanvas* canvas) {

    SkPaint paint1, paint2, paint3;

    paint1.setTextSize(64.0f);
    paint1.setAntiAlias(true);
    paint1.setColor(0xff4281A4);
    paint1.setStyle(SkPaint::kFill_Style);

    paint2.setTextSize(64.f);
    paint2.setAntiAlias(true);
    paint2.setColor(0xff9CAFB7);
    paint2.setStyle(SkPaint::kStroke_Style);
    paint2.setStrokeWidth(SkIntToScalar(3));

    paint3.setTextSize(64.0f);
    paint3.setAntiAlias(true);
    paint3.setColor(0xffE6B89C);
    paint3.setTextScaleX(SkFloatToScalar(1.5f));

    const char text[] = "Skia";
    canvas->drawText(text, strlen(text), 500.0f, 64.0f,  paint1);
    canvas->drawText(text, strlen(text), 500.0f, 144.0f, paint2);
    canvas->drawText(text, strlen(text), 500.0f, 224.0f, paint3);
}

void drawBezier(SkCanvas* canvas) {

    SkPaint paint;
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(8);
    paint.setColor(0xff1f78b4);
    paint.setAntiAlias(true);
    paint.setStrokeCap(SkPaint::kRound_Cap);

    SkPath path;
    path.moveTo(10, 10);
    path.quadTo(256, 64, 128, 128);
    path.quadTo(10, 192, 250, 250);
    canvas->drawPath(path, paint);
}


