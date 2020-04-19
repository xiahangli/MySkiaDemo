//
// Created by cw on 2018/7/24.
//

#include "MyLooper.h"

MyLooper::MyLooper() {
    mGLRender = new GLRender();
}

MyLooper::~MyLooper() {
    delete mGLRender;
}

void MyLooper::handleMessage(LooperMessage *msg) {
    switch (msg && msg->what){
        case kMsgSurfaceCreated:{
            mGLRender->surfaceCreated(static_cast<ANativeWindow *>(msg->obj));
            break;
        }
        case kMsgSurfaceChanged:{
            mGLRender->surfaceChanged(msg->arg1,msg->arg2);
            break;
        }
        case kMsgSurfaceDestroyed:{
            mGLRender->surfaceDestroyed();
            break;
        }
        default:
            break;
    }
}
