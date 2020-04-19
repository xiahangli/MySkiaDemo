//
// Created by cw on 2018/7/24.
//

#include "Triangle.h"
#include "../GlShaders.h"
#include "../GlUtils.h"

const GLint COORDS_PER_VERTEX = 3;
const GLint COLOR_PER_VERTEX = 4;
const GLint vertex_stride = COORDS_PER_VERTEX * 4;
const GLint color_stride = COLOR_PER_VERTEX * 4;

Triangle::Triangle() {

}

Triangle::~Triangle() {

}

int Triangle::init() {
    const char* vertexShader = SHADER_STRING(
        attribute vec4 aPosition;
        attribute vec4 aColor;
        varying vec4 vColor;
        void main(){
            gl_Position = aPosition;
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
    programHandle = createProgram(vertexShader,fragmentShader);
    if (programHandle<0){
        return -1;
    }
    glClearColor(0.0f,1.0f,0.0f,1.0f);
    return 0;
}

void Triangle::onDraw(int width, int height) {
    GLfloat vertex[] = {
            0.0f,0.5f,0.0f,
            -0.5f,-0.5f,0.0f,
            0.5f,-0.5f,0.0f
    };
    GLfloat color[] = {
            1.0f,0.0f,0.0f,1.0f
    };

    GLint vertexCount = sizeof(vertex)/(sizeof(vertex[0]) * COORDS_PER_VERTEX);

    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programHandle);

    GLint positionHandler = glGetAttribLocation(programHandle,"aPosition");
    glVertexAttribPointer(positionHandler,COORDS_PER_VERTEX,GL_FLOAT,GL_FALSE,vertex_stride,vertex);
    glEnableVertexAttribArray(positionHandler);
    glVertexAttrib4fv(1,color);

//    ESMatrix *mvpMatrix = new ESMatrix();
//    perspectiveM(mvpMatrix,0,0,width/height,1.0f,10.0f);
//    GLint mvpMatrixHandler = glGetUniformLocation(programHandle,"uMVPMatrix");
//    glUniformMatrix4fv(mvpMatrixHandler,1, false,mvpMatrix->m);

    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    glDisableVertexAttribArray(positionHandler);
//    delete mvpMatrix;

}

void Triangle::onDestroy() {
    if (programHandle >0){
        glDeleteProgram(programHandle);
    }
}
