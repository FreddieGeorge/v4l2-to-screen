#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <string.h>

// #include "my_fb.h"
#include "my_v4l2.h"
#include "my_opengl.h"
#include "common.h"

// usage : v4l2_to_screen ([0-CAM_MAX])
int main(int argc, char *argv[])
{
    // 默认video0
    char *buf = "/dev/video";
    int cam_num = 0;
    char *video_filename = new char[strlen(buf) + sizeof(cam_num) + 1];

    if (argc == 2)
    {
        if (atoi(argv[1]) >= MAX_CAM_NUM || atoi(argv[1]) < 0)
        {
            printf("input param error\n");
            return -1;
        }
        else
        {
            cam_num = atoi(argv[1]);
        }
    }
    sprintf(video_filename, "%s%d", buf, cam_num);

    // open vidoe and init v4l2
    my_v4l2 m_v4l2(video_filename);

    CT507Graphics::getInstance();
    CT507Graphics::getInstance()->initOpenGL();

    m_v4l2.v4l2_mmap();

    for (int i = 0; i < 4; i++)
    {
        m_v4l2.quitBuffer(i);
    }
    m_v4l2.StreamON();

    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            m_v4l2.getFrame(i);
            CT507Graphics::getInstance()->load_texture(m_v4l2.getBuffer(i),1920,1080);
            CT507Graphics::getInstance()->picDraw();
            m_v4l2.quitBuffer(i);
        }
    }

    m_v4l2.StreamOFF();
    // }
    

    
    return 1;
}