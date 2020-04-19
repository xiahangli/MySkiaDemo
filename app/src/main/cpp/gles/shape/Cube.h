//
// Created by cw on 2018/7/25.
//

#ifndef EGLDEMO_CUBE_H
#define EGLDEMO_CUBE_H


#include <stdlib.h>

class Cube {
public:
    Cube();
    ~Cube();
    int init();
    void onDraw(int width,int height);
    void onDestroy();
private:
    int programHandle;
};


#endif //EGLDEMO_CUBE_H
