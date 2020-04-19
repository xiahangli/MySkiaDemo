//
// Created by cw on 2018/7/25.
//

#include "Cube.h"
#include "../GlShaders.h"
#include "../GlUtils.h"

Cube::Cube() {

}

Cube::~Cube() {

}

int Cube::init() {
    const char* vertexShader = SHADER_STRING(
    uniform mat4 uMVPMatrix;
    attribute vec4 aPosition;
    attribute vec4 aColor;
    varying vec4 vColor;
    void main(){
        gl_Position = uMVPMatrix * aPosition;
        vColor = aColor;
        }
    );
    const char* fragmentShader = SHADER_STRING(
    precision mediump float;
    varying vec4 vColor;
    void main(){
        gl_FragColor = vColor;
        }
    );
    programHandle = createProgram(vertexShader, fragmentShader);
    if (programHandle <0){
        return -1;
    }
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return 0;
}

void Cube::onDraw(int width, int height) {
    
}

void Cube::onDestroy() {

}
