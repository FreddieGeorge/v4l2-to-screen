/**
 * @file my_opengl.cpp
 * @author Flork (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "my_opengl.h"
#include "sunxiMemInterface.h"
#include "sv_common.h"

#include "common.h"

static CT507Graphics *singleGraphics = NULL;
static EGLint const config_attribute_list[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_BUFFER_SIZE, 32,
    EGL_STENCIL_SIZE, 0,
    EGL_DEPTH_SIZE, 0,
    EGL_SAMPLE_BUFFERS, 1,
    EGL_SAMPLES, 4,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_NONE};

static const EGLint context_attribute_list[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE};

static EGLint window_attribute_list[] = {
    EGL_NONE};

// 在此数组中添加你着色器文件的名称
string g_shaderName[1][2] = {
    {"/root/vertex_origin.vs", "/root/fragment_origin.fs"}};

// 顶点坐标: 原像
static GLfloat vertex_coord[] =
{
    -1.0f, 1.0f, 0.0f,  // A点
    1.0f, 1.0f, 0.0f,   // B点
    1.0f, -1.0f, 0.0f,  // C点
    -1.0f, -1.0f, 0.0f, // D点
};


// 垂直翻转
static GLfloat vertex_coordFlip[] = 
{
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
};

// 左旋90度: 这个相当于是将原始的四个顶点坐标右转了90度 那为什么显示效果还是画面向左旋转了90度呢?
// 原因是因为顶点坐标与纹理坐标是按照顺序一一对应的 对于原始的纹理坐标为A' B' C' D' 他们之间的对应
// 关系就是A-A' B-B' C-C' D-D' 如果顶点坐标旋转之后 对应关系就变成了D-A' A-B' C-D' D-A' 那么就相当于
// 纹理图像(也就是视频画面)被左旋转了90度了
static GLfloat vertex_coordL90[] = 
{
	-1.0f, -1.0f, 0.0f,  // D点
	-1.0f, 1.0f, 0.0f,   // A点
	1.0f, 1.0f, 0.0f,    // B点
	1.0f, -1.0f, 0.0f,   // C点
};
static GLfloat hei_vertex_coordL90[] = 
{
	-0.342f, -1.0f, 0.0f,  // D点
	-0.342f, 1.0f, 0.0f,   // A点
	0.342f, 1.0f, 0.0f,    // B点
	0.342f, -1.0f, 0.0f,   // C点
};

// 旋180°
static GLfloat vertex_coord180[] = 
{
     1.0f, -1.0f, 0.0f,   // A点
    -1.0f, -1.0f, 0.0f,   // B点
    -1.0f,  1.0f, 0.0f,   // C点
     1.0f,  1.0f, 0.0f,   // D点
};
// 右旋90度
static GLfloat vertex_coordR90[] = 
{
	1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
};

// 镜像加翻转
static GLfloat vertex_coordMF[] = 
{
	1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
};

// 纹理坐标
// static GLfloat tex_coord1088p[] =
// {
//     0.0f, 0.0f,
//     1.0f,0.0f,
//     1.0f,1.0f,
//     0.0f,1.0f,
// };

static GLfloat tex_coord1080p[] =
{
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,0.992f,
    0.0f,0.992f,
};

static GLfloat tex_coord720p[] =
{
    0.0f,0.0f,
    0.666f,0.0f,
    0.666f,0.661f,
    0.0f,0.661f,
};
// 单画面旋转90度后不拉伸的裁剪
// static GLfloat tex_coord1088p_r90[] =
// {
//     0.0f, 0.0f,
//     1.0f,0.0f,
//     1.0f,1.0f,
//     0.0f,1.0f,
// };

static GLfloat tex_coord1080p_r90[] =
{
    0.395f,0.0f,
    0.605f,0.0f,
    0.605f,0.992f,
    0.395f,0.992f,
};

static GLfloat tex_coord720p_r90[] =
{
    0.0f,0.0f,
    0.666f,0.0f,
    0.666f,0.661f,
    0.0f,0.661f,
};


struct fbdev_window native_window;

// 将枚举值本身转换成字符串输出的方法
static const char *glErrorString(EGLint code)
{
#define MYERRCODE(x) \
    case x:          \
        return #x;
    switch (code)
    {
        MYERRCODE(GL_NO_ERROR);
        MYERRCODE(GL_INVALID_ENUM);
        MYERRCODE(GL_INVALID_VALUE);
        MYERRCODE(GL_INVALID_OPERATION);
        MYERRCODE(GL_STACK_OVERFLOW_KHR);
        MYERRCODE(GL_STACK_UNDERFLOW_KHR);
        MYERRCODE(GL_OUT_OF_MEMORY);
    default:
        return "unknown";
    }
#undef MYERRCODE
}

static const char *eglErrorString(EGLint code)
{
#define MYERRCODE(x) \
    case x:          \
        return #x;
    switch (code)
    {
        MYERRCODE(EGL_SUCCESS)
        MYERRCODE(EGL_NOT_INITIALIZED)
        MYERRCODE(EGL_BAD_ACCESS)
        MYERRCODE(EGL_BAD_ALLOC)
        MYERRCODE(EGL_BAD_ATTRIBUTE)
        MYERRCODE(EGL_BAD_CONTEXT)
        MYERRCODE(EGL_BAD_CONFIG)
        MYERRCODE(EGL_BAD_CURRENT_SURFACE)
        MYERRCODE(EGL_BAD_DISPLAY)
        MYERRCODE(EGL_BAD_SURFACE)
        MYERRCODE(EGL_BAD_MATCH)
        MYERRCODE(EGL_BAD_PARAMETER)
        MYERRCODE(EGL_BAD_NATIVE_PIXMAP)
        MYERRCODE(EGL_BAD_NATIVE_WINDOW)
        MYERRCODE(EGL_CONTEXT_LOST)
    default:
        return "unknown";
    }
#undef MYERRCODE
}

sv_s32 getEGL_GL_ErrInfo()
{
    EGLint code;
    code = eglGetError();
    if (code != EGL_SUCCESS)
    {
        printf("creatEglImage failed: err_description = %s, code = %d\n", eglErrorString(code), code);
        return -1;
    }

    code = glGetError();
    if (code != GL_NO_ERROR)
    {
        printf("glGetError: err_description = %s, code = %d\n", glErrorString(code), code);
        return -1;
    }

    return 0;
}

sv_s32 getImageSize(sv_u32 u32format, sv_u32 u32width, sv_u32 u32height)
{
    printf("getImageSize: width = %d, height = %d.\n", u32width, u32height);
    switch (u32format)
    {
    case DRM_FORMAT_ARGB8888:
    case DRM_FORMAT_RGBA8888:
    case DRM_FORMAT_BGRA8888:
        return u32width * u32height * 4;
    case DRM_FORMAT_NV21:
    case DRM_FORMAT_NV12:
        return u32width * u32height * 3 / 2;
    }

    return 0;
}

static bool checkEglExtension(const char *extensions, const char *extension)
{
    size_t extlen = strlen(extension);
    const char *end = extensions + strlen(extensions);

    while (extensions < end)
    {
        size_t n = 0;

        /* Skip whitespaces, if any */
        if (*extensions == ' ')
        {
            extensions++;
            continue;
        }
        n = strcspn(extensions, " ");
        /* Compare strings */
        if (n == extlen && strncmp(extension, extensions, n) == 0)
            return true; /* Found */

        extensions += n;
    }

    /* Not found */
    return false;
}

// 创建EGL帧(EGL image)
static EGLImageKHR creatEglImage(EGLDisplay dpy, int dma_fd, unsigned int width, unsigned int height, unsigned int format)
{
    int atti = 0;
    EGLint attribs0[30];

    // set the image's size
    attribs0[atti++] = EGL_WIDTH;
    attribs0[atti++] = width;
    attribs0[atti++] = EGL_HEIGHT;
    attribs0[atti++] = height;

    // set pixel format
    attribs0[atti++] = EGL_LINUX_DRM_FOURCC_EXT;
    attribs0[atti++] = format;

    switch (format)
    {
    case DRM_FORMAT_ARGB8888:
    case DRM_FORMAT_RGBA8888:
    case DRM_FORMAT_BGRA8888:
        attribs0[atti++] = EGL_DMA_BUF_PLANE0_FD_EXT;
        attribs0[atti++] = dma_fd;
        attribs0[atti++] = EGL_DMA_BUF_PLANE0_OFFSET_EXT;
        attribs0[atti++] = 0;
        attribs0[atti++] = EGL_DMA_BUF_PLANE0_PITCH_EXT;
        attribs0[atti++] = width * 4;
        break;
    case DRM_FORMAT_NV21:
    case DRM_FORMAT_NV12:
        // set buffer fd, offset, pitch for y component
        attribs0[atti++] = EGL_DMA_BUF_PLANE0_FD_EXT;
        attribs0[atti++] = dma_fd;
        attribs0[atti++] = EGL_DMA_BUF_PLANE0_OFFSET_EXT;
        attribs0[atti++] = 0;
        attribs0[atti++] = EGL_DMA_BUF_PLANE0_PITCH_EXT;
        attribs0[atti++] = width;

        // set buffer fd, offset ,pitch
        attribs0[atti++] = EGL_DMA_BUF_PLANE1_FD_EXT;
        attribs0[atti++] = dma_fd;
        attribs0[atti++] = EGL_DMA_BUF_PLANE1_OFFSET_EXT;
        attribs0[atti++] = width * height;
        attribs0[atti++] = EGL_DMA_BUF_PLANE1_PITCH_EXT;
        attribs0[atti++] = width;
        break;
    default:
        printf("format not support, format = %d\n", format);
    };

    // set color space and color range
    attribs0[atti++] = EGL_YUV_COLOR_SPACE_HINT_EXT;
    attribs0[atti++] = EGL_ITU_REC709_EXT;
    attribs0[atti++] = EGL_SAMPLE_RANGE_HINT_EXT;
    attribs0[atti++] = EGL_YUV_FULL_RANGE_EXT;
    attribs0[atti++] = EGL_NONE;

    // Notice the target: EGL_LINUX_DMA_BUF_EXT
    return eglCreateImageKHR(dpy, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, 0, attribs0);
}

CT507Graphics::CT507Graphics()
{
    if (IonAllocOpen() < 0)
    {
        printf("IonAllocOpen failed\n");
    }

    int ret = 0;

    TexMem.virt = NULL;
    TexMem.virt = NULL;
    pthread_mutex_init(&dataMutex, NULL);

    shader_program = new uint32_t[sizeof(g_shaderName) / sizeof(g_shaderName[0])];

    texConfig.view_width = SCREEN_W;
    texConfig.view_height = SCREEN_H;
    texConfig.TexFormat = DRM_FORMAT_NV21;

    pthread_mutex_init(&RenderMutex, NULL);
    pthread_cond_init(&RenderCond, NULL);
    bRender = false;
}

CT507Graphics::~CT507Graphics()
{
    IonFree(TexMem.virt);
    IonFree(TexMem.virt);
}

CT507Graphics *CT507Graphics::getInstance()
{
    if (singleGraphics == NULL)
        singleGraphics = new CT507Graphics;
    return singleGraphics;
}

int CT507Graphics::readShaderSource(string &vShader, string &fShader, int s32Index)
{
    int s32ReadSize = 0;
    char s8ReadBuf[1024] = {0};

    vShader.clear();
    fShader.clear();
    for (int i = 0; i < sizeof(g_shaderName[0]) / sizeof(g_shaderName[0][0]); i++)
    {
        FILE *pFp = fopen(g_shaderName[s32Index][i].c_str(), "rb");
        if (NULL == pFp)
        {
            printf("open shader failed. \n");
            return -1;
        }

        while ((s32ReadSize = fread(s8ReadBuf, 1, 1024, pFp)) > 0)
        {
            if (0 == i)
                vShader.append(s8ReadBuf); // 读取顶点着色器GLSL程序
            else if (1 == i)
                fShader.append(s8ReadBuf); // 读取片段着色器GLSL程序
            memset(s8ReadBuf, 0, 1024);
        }
    }
    // cout<<endl;
    // cout<<vShader<<endl<<endl;
    // cout<<fShader<<endl<<endl;

    return 0;
}

int CT507Graphics::initOpenGL()
{
    int ret = 0;
    string vertexShader = "";
    string fragmentShader = "";
    // init EGL
    if (initEGL() < 0)
    {
        printf("init EGL failed! \n");
        return -1;
    }
    // init shader
    // 读取着色器程序源码
    ret = readShaderSource(vertexShader, fragmentShader, 0);
    if (initShader(vertexShader.c_str(), fragmentShader.c_str(), 0) < 0)
    {
        printf("init Shader failed, index = %d. \n", 0);
        return -1;
    }

    // 创建存储纹理贴图数据的缓存区
    ret = create_texture();

    if (ret < 0)
    {
        printf("create texture buf failed! \n");
        return -1;
    }

    setupTexture();

    return 0;
}

int CT507Graphics::initEGL()
{
    EGLConfig config;
    EGLint num_config;
    const char *extensions;

    m_egl.egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_egl.egl_display == EGL_NO_DISPLAY)
    {
        printf("no display found!\n");
        return -1;
    }

    if (!eglInitialize(m_egl.egl_display, &(m_egl.egl_major), &(m_egl.egl_minor)))
    {
        printf("eglInitialize failed!\n");
        return -1;
    }
    printf("EGL Version: \"%s\" \n", eglQueryString(m_egl.egl_display, EGL_VERSION));
    printf("EGL Vendor: \"%s\" \n", eglQueryString(m_egl.egl_display, EGL_VENDOR));
    extensions = eglQueryString(m_egl.egl_display, EGL_EXTENSIONS);
    printf("EGL Extensions: \"%s\" \n\n", extensions);

    if (!checkEglExtension(extensions, "EGL_EXT_image_dma_buf_import"))
    {
        printf("egl extension image can not be imported by dma buffer. \n");
        return -1;
    }

    if (!eglBindAPI(EGL_OPENGL_ES_API))
    {
        printf("failed to bind api EGL_OPENGL_ES_API. \n");
        return -1;
    }

    eglChooseConfig(m_egl.egl_display, config_attribute_list, &config, 1, &num_config);
    m_egl.egl_context = eglCreateContext(m_egl.egl_display, config, EGL_NO_CONTEXT, context_attribute_list);
    if (m_egl.egl_context == EGL_NO_CONTEXT)
    {
        EGLint code;
        code = eglGetError();
        if (code != EGL_SUCCESS)
            printf("eglCreateContext failed: err_description = %s, code = %d\n", eglErrorString(code), code);
        else
            printf("eglCreateContext failed, do not get err description. \n");
    }
    GLint maxSamples = -1;
    glGetIntegerv(GL_MAX_SAMPLES_IMG, &maxSamples);
    printf("GL_MAX_SAMPLES_IMG:: maxSamples =  %d \n", maxSamples);

    GLint width, height;
    // obtain an surface(buffer) that can be display diractly
    native_window.width = texConfig.view_width;
    native_window.height = texConfig.view_height;

    printf("native_window.width = %d, native_window.height = %d \n", native_window.width, native_window.height);
    m_egl.egl_surface = eglCreateWindowSurface(m_egl.egl_display, config, (NativeWindowType)&native_window, window_attribute_list);
    if (m_egl.egl_surface == EGL_NO_SURFACE)
    {
        printf("egl no surface. \n");
        EGLint code;
        code = eglGetError();
        if (code != EGL_SUCCESS)
        {
            printf("alloc egl surface failed: %s, %d\n", eglErrorString(code), code);
            return -1;
        }
        else
            printf("alloc egl surface failed, no reason! \n");
    }

    if (!eglQuerySurface(m_egl.egl_display, m_egl.egl_surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(m_egl.egl_display, m_egl.egl_surface, EGL_HEIGHT, &height))
    {
        printf("check error 1. \n");
        return -1;
    }
    printf("Surface size: %d * %d\n", width, height);

    if (!eglMakeCurrent(m_egl.egl_display, m_egl.egl_surface, m_egl.egl_surface, m_egl.egl_context))
    {
        printf("check error 2. \n");
        return -1;
    }
    eglSwapInterval(m_egl.egl_display, 1);

    return 0;
}

void CT507Graphics::EGLInfoDump()
{
    printf("GL Vendor: \"%s\"\n", glGetString(GL_VENDOR));
    printf("GL Renderer: \"%s\"\n", glGetString(GL_RENDERER));
    printf("GL Version: \"%s\"\n", glGetString(GL_VERSION));
    printf("GL Extensions: \"%s\"\n", glGetString(GL_EXTENSIONS));
    printf("\n\n\n");
}

int CT507Graphics::initShader(const char *vertexShader, const char *fragmentShader, int index)
{
    GLint ret;
    // 创建顶点着色器
    if (createShader(&vs_id, GL_VERTEX_SHADER, vertexShader) < 0)
    {
        printf("createShader vertex shader failed! \n");
        return 0;
    }
    // 创建片段着色器
    if (createShader(&fs_id, GL_FRAGMENT_SHADER, fragmentShader) < 0)
    {
        printf("createShader fragment shader failed!\n");
        return 0;
    }

    shader_program[index] = glCreateProgram();
    if (!shader_program[index])
    {
        printf("failed to create shader_program!\n");
        return -1;
    }
    // 绑定并链接着色器
    glAttachShader(shader_program[index], vs_id);
    glAttachShader(shader_program[index], fs_id);
    // 指明顶点着色器中的顶点属性位置
    glBindAttribLocation(shader_program[index], 0, "aPosition");
    glBindAttribLocation(shader_program[index], 1, "aTexCoord");
    // glBindAttribLocation(shader_program, 2, "aColor"); // used for testing
    glLinkProgram(shader_program[index]);

    // 判断链接是否成功 若失败则获取失败原因
    glGetProgramiv(shader_program[index], GL_LINK_STATUS, &ret);
    if (!ret)
    {
        char infoLog[1024] = {0};
        glGetProgramInfoLog(shader_program[index], 1024, NULL, infoLog);
        printf("shader_program link failed, errInfo = %s\n", infoLog);
        return -1;
    }
    glUseProgram(shader_program[index]);

    return 0;
}

int CT507Graphics::createShader(unsigned int *shader_id, int shaderType, const char *shaderSource)
{
    GLint ret;

    *shader_id = glCreateShader(shaderType);
    if (*shader_id == 0)
    {
        int code;
        code = glGetError();
        if (code != GL_NO_ERROR)
            printf("glCreateShader failed: err_description = %s, code = %d\n", glErrorString(code), code);
        else
            printf("glCreateShader failed, do not get err description. \n");

        return -1;
    }

    glShaderSource(*shader_id, 1, &shaderSource, NULL);
    glCompileShader(*shader_id);
    // 判断编译是否成功 若失败则获取失败原因
    glGetShaderiv(*shader_id, GL_COMPILE_STATUS, &ret);
    if (!ret)
    {
        char infoLog[1024] = {0};
        glGetShaderInfoLog(*shader_id, 1024, NULL, infoLog);
        printf("shader compilation failed, type = %d, errInfo = %s\n", shaderType, infoLog);

        return -1;
    }

    return 0;
}

int CT507Graphics::create_texture()
{
    void *addr_file;
    int dma_fd = -1;
    int file_fd;
    unsigned int size = 0;

    texConfig.tex_width = 1920;
    texConfig.tex_height = 1080;

    size = getImageSize(texConfig.TexFormat, texConfig.tex_width, texConfig.tex_height);
    dma_fd = dma_alloc(&TexMem, size);
    if (dma_fd < 0)
    {
        printf("dma_alloc for  texture failed! \n");
        return -1;
    }

    return 0;
}


int CT507Graphics::load_texture(void *yuv_virAddr, int w, int h)
{

    pthread_mutex_lock(&dataMutex);

    if (h < texConfig.tex_height)
    {
        for (int i = 0; i < h; i++) // Y
            memcpy(TexMem.virt + i * texConfig.tex_width, yuv_virAddr + i * w, w);
        for (int i = 0; i < h / 2; i++) // UV
            memcpy(TexMem.virt + texConfig.tex_width * texConfig.tex_height + i * texConfig.tex_width, yuv_virAddr + h * w + i * w, w);
    }
    else
    {
        memcpy(TexMem.virt, yuv_virAddr, TexMem.size);
    }
    IonDmaSync(TexMem.dmafd);
    pic_width = w;
    pic_height = h;

    pthread_mutex_unlock(&dataMutex);

    pthread_mutex_lock(&RenderMutex);
    bRender = true;
    pthread_cond_signal(&RenderCond);
    pthread_mutex_unlock(&RenderMutex);
    
    return 0;
}

int CT507Graphics::setupTexture()
{
    int ret = 0;
    EGLImageKHR img_pre;
    glActiveTexture(GL_TEXTURE0);

    img_pre = creatEglImage(m_egl.egl_display, TexMem.dmafd, 1920, 1080, DRM_FORMAT_NV21);
    if (img_pre == EGL_NO_IMAGE_KHR)
    {
        ret = getEGL_GL_ErrInfo();
        if (ret)
            printf("create EGLImage failed. \n");

        return -1;
    }

    // create gl texture and refered to EGL Image
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, tex);

    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小时的采样方式
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大时的采样方式
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // 相当于一般情况下的glTexImage2D函数: 利用视频帧数据生成一张2D的纹理
    glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)img_pre);
    ret = getEGL_GL_ErrInfo();
    if (ret)
    {
        printf("MIPMAP failed. \n");
        return -1;
    }

    printf("secceed to setupTexture!\n");

    return 0;
}

int CT507Graphics::picDraw()
{
    pthread_mutex_lock(&RenderMutex);
    while (!bRender)
    {
        pthread_cond_wait(&RenderCond, &RenderMutex);
    }
    bRender = false;
    pthread_mutex_unlock(&RenderMutex);

    glUseProgram(shader_program[0]);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    fullScreenMode();
    eglSwapInterval(m_egl.egl_display, 1);

    eglSwapBuffers(m_egl.egl_display, m_egl.egl_surface);
    glFinish();

    return 0;
}

int CT507Graphics::dma_alloc(ion_mem *mem, int size)
{

    mem->virt = (unsigned char *)IonAlloc(size);
    mem->size = size;
    mem->phy = IonVir2phy(mem->virt);
    mem->dmafd = IonVir2fd(mem->virt);
    // mem->handle = sAllocInfo.handle;

    return mem->dmafd;
}

int CT507Graphics::fullScreenMode()
{
    pthread_mutex_lock(&dataMutex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertex_coord);
    glEnableVertexAttribArray(0);
    if (pic_height == 1088)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, tex_coord1088p);
    else if (pic_height == 1080)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, tex_coord1080p);
    else if (pic_height == 720)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, tex_coord720p);
    
    glEnableVertexAttribArray(1);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, tex);

    glViewport(0, 0, SCREEN_W, SCREEN_H);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    pthread_mutex_unlock(&dataMutex);
    

    return 0;
}

