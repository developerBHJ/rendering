//
//  main.cpp
//  OpenGL_demo1
//
//  Created by baihuajun on 2020/3/27.
//  Copyright © 2020 baihuajun. All rights reserved.
//

#include "GLTools.h"
#include "GLBatch.h"

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
GLShaderManager shaderManager;

// 召唤场景
void RenderScene()
{
    // 设置背景色
    glClearColor(0.0f, 0.0f, 1.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    // 现在剪成小红色分区
    // 1、设置裁剪区域颜色为红色
    glClearColor(1.0f, 0.0f, 0.0f, 0.0);
    // 2、设置裁剪尺寸
    glScissor(100, 100, 600, 400);
    // 3、开启裁剪测试
    glEnable(GL_SCISSOR_TEST);
    // 4、开启清屏，执行裁剪
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 裁剪出一个绿色的小矩形
    glScissor(100, 100, 200, 200);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
}

// 窗口已更改大小，或刚刚创建。无论哪种情况，我们都需要
// 使用窗口维度设置视口和投影矩阵.
void ChangeSize(int w, int h)
{
    if (h == 0) {
        h = 1;
    }
    glViewport(0, 0, w, h);
}


int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    //申请一个颜色缓存区、深度缓存区、双缓存区、模板缓存区
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    //设置window 的尺寸
    glutInitWindowSize(800, 600);
    //创建window的名称
    glutCreateWindow("OpenGL Scissor");
    //注册回调函数（改变尺寸）
    glutReshapeFunc(ChangeSize);
    //显示函数
    glutDisplayFunc(RenderScene);
    //判断一下是否能初始化glew库，确保项目能正常使用OpenGL 框架
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    // runloop运行循环
    glutMainLoop();
    return 0;
}
