#pragma once
#include <iostream>
#include <string>
#include <EGL/eglplatform.h>
#include <EGL/egl.h>
// 这个头文件必须放这里 否则这个头文件会报一大堆类型定义的错误
#include <EGL/eglext.h>
#include "ION_DRM/ion.h"
#include "ION_DRM/drm_fourcc.h"

using namespace std;

typedef struct fbdev_window
{
    unsigned short width;
    unsigned short height;
} fbdev_window1;

typedef struct
{
    unsigned long int phy; /* physical address */
    unsigned char *virt;   /* virtual address */
    unsigned long size;    /* ion buffer size */
    int dmafd;             /* ion dmabuf fd */
    int handle;            /* ion handle */
} ion_mem;

typedef struct
{
    unsigned int TexFormat;              // pixel format of texture: ARGB888
    unsigned int tex_width; // size of texture
    unsigned int tex_height;
    unsigned int view_width; // size of window
    unsigned int view_height;
    // int ion_fd;
} texture_config;

typedef enum
{
    E_CAMERA_RENDER = 0,
    E_VGA_HDMI_RENDER
} ERender_Type;

typedef struct
{
    EGLint egl_major, egl_minor;
    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;
} EGL_TYPE;

// 图形处理类 我们这里主要是使用其来实现分割模式
class CT507Graphics
{
public:
    static CT507Graphics *getInstance();
    // 打印EGL功能信息
    void EGLInfoDump();
    // 调用此接口将每一帧yuv图片load到TexMem.virt对应的地址空间中
    int load_texture(void *yuv_virAddr, int w, int h);
    // int setDrawType(int drawTypeTmp);
    int setupTexture();
    int initOpenGL();
    int picDraw();
    int dma_alloc(ion_mem *mem, int size);

private:
    CT507Graphics();
    ~CT507Graphics();
    CT507Graphics(const CT507Graphics &graphicsTmp){};

    int initEGL();
    int readShaderSource(string &vShader, string &fShader, int index);
    int initShader(const char *vertexShader, const char *fragmentShader, int index);
    int createShader(unsigned int *shader_id, int shaderType, const char *shaderSource);
    int create_texture();
    // 分割模式设置函数
    int fullScreenMode();

private:
    // EGL变量
    EGL_TYPE m_egl;
    // 着色器变量
    unsigned int vs_id;
    unsigned int fs_id;
    uint32_t *shader_program;
    // ion内存: texture dma buffer
    ion_mem TexMem;
    bool bRender; // 这个宏在多线程操作的时候会用到，该demo中无作用
    // 纹理变量
    unsigned int tex;
    unsigned int fboColorBufTex;
    unsigned int fbo;
    texture_config texConfig;
    unsigned int pic_width;
    unsigned int pic_height;
    // 预览与回放绑定不同EGLImage的标志项 0表示预览 1表示回放
    // EGLImage变化标志位
    // bool bEGLImage;

    int vTexSamplerHandler;
    // 拷贝数据互斥锁
    pthread_mutex_t dataMutex;
    pthread_mutex_t RenderMutex;
    pthread_cond_t RenderCond;
};
