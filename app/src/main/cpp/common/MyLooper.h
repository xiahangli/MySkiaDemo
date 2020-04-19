//
// Created by cw on 2018/7/24.
//

#ifndef EGLDEMO_MYLOOPER_H
#define EGLDEMO_MYLOOPER_H


#include "looper.h"
#include "../gles/GLRender.h"

enum {
    kMsgSurfaceCreated,
    kMsgSurfaceChanged,
    kMsgSurfaceDestroyed
};

class MyLooper : public Looper {

public:
    MyLooper();
    virtual ~MyLooper();
    virtual void handleMessage(LooperMessage *msg);

private:
    GLRender *mGLRender;

};


#endif //EGLDEMO_MYLOOPER_H
