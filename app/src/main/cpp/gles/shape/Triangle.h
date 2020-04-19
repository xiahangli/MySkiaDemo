//
// Created by cw on 2018/7/24.
//

#ifndef EGLDEMO_TRIANGLE_H
#define EGLDEMO_TRIANGLE_H


#include <GLES2/gl2.h>

class Triangle {
public:
    Triangle();
    ~Triangle();

    int init();
    void onDraw(int width, int height);
    void onDestroy();
private:
    int programHandle;
};


#endif //EGLDEMO_TRIANGLE_H
