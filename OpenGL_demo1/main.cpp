//
//  main.cpp
//  OpenGL_demo1
//
//  Created by baihuajun on 2020/3/27.
//  Copyright © 2020 baihuajun. All rights reserved.
//

/*
 OpenGL 第三节课
 背面剔除
 深度测试
 多边形
 裁剪
 颜色混合
 */
#include "GLTools.h"
#include "GLBatch.h"
#include "GLMatrixStack.h"
#include "GLFrame.h"
#include "GLFrustum.h"
#include "GLGeometryTransform.h"

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif
GLShaderManager shaderManager;
// 设置角色帧 作为相机
GLFrame viewFrame;
// 使用GLFrustum类 设置透视投影
GLFrustum viewFrustum;

GLTriangleBatch  torusBatch;
GLMatrixStack     modelViewMatix;
GLMatrixStack     projectViewMatix;
GLGeometryTransform transformPipeline;
// 标记深度测试
int isDepth = 0;
// 标记背面剔除
int isCull = 0;


void SetupRC()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0);
    // 初始化着色器管理器
    shaderManager.InitializeStockShaders();
    // 将相机向后移动10个单元
    viewFrame.MoveForward(10.f);
    // 创建甜甜圈
    /*
     gltMakeTorus(GLTriangleBatch& torusBatch, GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor)
     参数1: 容器帮助类
     参数2: 外边缘半径
     参数3: 内边缘半径
     参数4:主半径的细分单元数量
     参数5:从半径的细分单元数
     */
    gltMakeTorus(torusBatch, 1.0f, 0.3f, 52, 26);
    // 点的大小
    glPointSize(2.0f);
}

// 渲染
void RenderScene()
{
    // 第一步 ：清除窗口 和 深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 开启/关闭正背面剔除
    if (isCull) {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    }else{
        glDisable(GL_CULL_FACE);
    }
    // 深度测试
    if (isDepth) {
        glEnable(GL_DEPTH_TEST);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
    
    // 第二步 ：把摄像机矩阵压入模型矩阵中
    modelViewMatix.PushMatrix(viewFrame);
    // 第三步 ：设置颜色
    GLfloat vPink[] = {1.0,105.0/225.0,180.0/255.0,1.0};
    // 第四步 ：使用平面着色器
    /*
     通过光源、阴影体现t立体效果
     GLT_SHADER_DEFAULT_LIGHT: 默认光源着色器
     参数2：模型视图矩阵
     参数3:  投影矩阵
     参数4:  颜色
     */ shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT,transformPipeline.GetModelViewMatrix(),transformPipeline.GetProjectionMatrix(),vPink);
    
    // 第五步 ：绘制
    torusBatch.Draw();
    // 第六步：出栈。 绘制完成。恢复
    modelViewMatix.PopMatrix();
    // 交换缓冲区
    glutSwapBuffers();
}

// 窗口已更改大小，或刚刚创建。无论哪种情况，我们都需要
// 使用窗口维度设置视口和投影矩阵.
void ChangeSize(int w, int h)
{
    // 防止 h 变为0
    if (h == 0) {
        h = 1;
    }
    glViewport(0, 0, w, h);
    // 设置透视模式 初始化透视矩阵
    /*
     SetPerspective 函数的参数是一个从顶点方向去看的角度（用角度值表示）
     */
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 100.0f);
    // 把透视矩阵加载到透视矩阵对阵中
    projectViewMatix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    // 初始化渲染管线
    transformPipeline.SetMatrixStacks(modelViewMatix, projectViewMatix);
}

/// 键位设置   控制Camera的移动改变视口
/// @param key 键位
/// @param x x
/// @param y y
void SpecialKeys(int key, int x, int y){
    if (key == GLUT_KEY_UP) {
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
    }else if (key == GLUT_KEY_LEFT){
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);
    }else if (key == GLUT_KEY_DOWN){
        viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
    }else if (key == GLUT_KEY_RIGHT){
        viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);
    }
    // 重新刷新
    glutPostRedisplay();
}

/// 右击  菜单
/// @param value 菜单选中值
void ProcessMenu(int value){
    switch (value) {
        case 1:
            isDepth = !isDepth;
            break;
        case 2:
            isCull = !isCull;
            break;
        case 3:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 4:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 5:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        default:
            break;
    }
    glutPostRedisplay();
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
    glutCreateWindow("GL_POINTS");
    //注册回调函数（改变尺寸）
    glutReshapeFunc(ChangeSize);
    //显示函数
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    // 添加右击菜单栏
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Toggle depth test",1);
    glutAddMenuEntry("Toggle cull backface",2);
    glutAddMenuEntry("Set Fill Mode", 3);
    glutAddMenuEntry("Set Line Mode", 4);
    glutAddMenuEntry("Set Point Mode", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    //判断一下是否能初始化glew库，确保项目能正常使用OpenGL 框架
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    //绘制
    SetupRC();
    //runloop运行循环
    glutMainLoop();
    return 0;
}
