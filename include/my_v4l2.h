#ifndef V4L2_H
#define V4L2_H

#include <sys/mman.h>

#include <linux/fb.h>
#include <linux/videodev2.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <asm/types.h>
#include <string.h>
#include <errno.h>

#include "common.h"

#define V4L2_MODE_VIDEO 0x0002

struct v4l2_mem
{
    color_t *buffer;
    unsigned int size;
};

// 循环
// m_v4l2.quitBuffer(0-3);
// m_v4l2.StreamON();
// loop start
// m_v4l2.getFrame(i);
// memcpy(some_place, m_v4l2.getBuffer(i), m_v4l2.getSize(i));
// m_v4l2.quitBuffer(i);
// loop end
class my_v4l2
{
private:
    /* data */
    int v4l2_fd;
    struct v4l2_mem m_buffer[ION_BUF_NUM];
    struct v4l2_buffer buf;

    int isInit;
public:
    my_v4l2(char *filename);
    ~my_v4l2();

    int getInitStatus();

    int v4l2_mmap();           // 分配buffer
    int StreamON();            // 开始stream
    int StreamOFF();           // 结束stream
    int getFrame(int);         // 获取一帧
    int quitBuffer(int);       // 出队
    color_t *getBuffer(int);   // 获取视频缓存地址
    unsigned int getSize(int); // 获取buffer长度

    // ctrl
    int setMirror(bool bMirror);
    int setFlip(bool bFlip);
    int setMirrorAndFlip(bool bMirror, bool bFlip);
    // class UltraFcw m_fcw;
};

#endif