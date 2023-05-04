#include "my_v4l2.h"

float test[IMAGEHEIGHT][IMAGEWIDTH][3];

my_v4l2::my_v4l2(char *filename)
{
    isInit = 0;
    for (int i = 0; i < ION_BUF_NUM; i++)
    {
        m_buffer[i].buffer = NULL;
        m_buffer[i].size = 0;
    }

    struct v4l2_input inp;
    struct v4l2_streamparm parms;
    struct v4l2_format format;

    v4l2_fd = open(filename, O_RDWR | O_NONBLOCK, 0);
    if (v4l2_fd < 0)
    {
        printf("open %s error\r\n", filename);
        return;
    }
    else
        printf("open %s in fd %d \r\n", filename, v4l2_fd);

    memset(&inp, 0, sizeof(inp));
    inp.index = 0;
    if (-1 == ioctl(v4l2_fd, VIDIOC_S_INPUT, &inp))
    {
        printf("VIDIOC_S_INPUT %d error!\n", inp.index);
        return;
    }

    // set parm
    memset(&parms, 0, sizeof(parms));
    parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    parms.parm.capture.timeperframe.numerator = 1;
    parms.parm.capture.timeperframe.denominator = 30; // FPS
    parms.parm.capture.capturemode = V4L2_MODE_VIDEO;
    parms.parm.capture.reserved[0] = 0;
    parms.parm.capture.reserved[1] = 0; /*2:command, 1: wdr, 0: normal*/
    if (-1 == ioctl(v4l2_fd, VIDIOC_S_PARM, &parms))
    {
        printf("VIDIOC_S_PARM error\n");
    }

    // get foramt

    CLEAR(format);
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    if (ioctl(v4l2_fd, VIDIOC_G_FMT, &format) == -1)
    {
        printf("VIDIOC_S_FMT error !\n");
    }
    printf("V4L2: getting format: %c%c%c%c %ux%u\n", pixfmtstr(format.fmt.pix_mp.pixelformat), format.fmt.pix_mp.width, format.fmt.pix_mp.height);

    // set format
    CLEAR(format);
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    format.fmt.pix_mp.width = IMAGEWIDTH;
    format.fmt.pix_mp.height = IMAGEHEIGHT;
    format.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV21;
    format.fmt.pix_mp.field = V4L2_FIELD_NONE;
    format.fmt.pix_mp.colorspace = V4L2_COLORSPACE_REC709;
    if (ioctl(v4l2_fd, VIDIOC_S_FMT, &format) == -1)
    {
        printf("VIDIOC_S_FMT error !\n");
    }
    else
    {
        printf("VIDIOC_S_FMT success \n");
        return;
    }

    isInit = 1;
}

my_v4l2::~my_v4l2()
{
    unsigned int i;

    StreamOFF();

    for (i = 0; i < ION_BUF_NUM; i++)
    {
        if (m_buffer[i].buffer != NULL)
        {
            if (munmap(m_buffer[i].buffer, m_buffer[i].size) < 0)
            {
                printf("v4l2 destory error:%s\r\n", strerror(errno));
            }
            m_buffer[i].buffer = NULL;
        }
    }

    // 关闭设备
    close(v4l2_fd);
}

int my_v4l2::v4l2_mmap()
{
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers req;

    CLEAR(req);
    req.count = FRAME_NUM;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(v4l2_fd, VIDIOC_REQBUFS, &req) < 0)
    {
        printf("request for buffers error\n");
        return FAILURE;
    }

    for (unsigned int i = 0; i < req.count; i++)
    {
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.m.planes = (struct v4l2_plane *)malloc(sizeof(struct v4l2_plane));
        memset(buf.m.planes, 0, sizeof(struct v4l2_plane));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.length = 1;
        buf.index = i;
        if (buf.m.planes == NULL)
        {
            printf("buf.m.planes calloc failed!\n");
            return FAILURE;
        }
        // 查询
        if (ioctl(v4l2_fd, VIDIOC_QUERYBUF, &buf) == -1)
        {
            printf("query buffer error\n");
            return FAILURE;
        }

        m_buffer[i].buffer = (unsigned int *)mmap(NULL, buf.m.planes->length, PROT_READ | PROT_WRITE, MAP_SHARED, v4l2_fd, buf.m.planes->m.mem_offset);

        m_buffer[i].size = buf.m.planes->length;

        // if (ioctl(v4l2_fd, VIDIOC_QBUF, &buf) < 0)
        // {
        //     printf("VIDIOC_QBUF error \r\n");
        //     return FAILURE;
        // }
    }

    free(buf.m.planes);

    return SUCCESS;
}

int my_v4l2::StreamON()
{
    enum v4l2_buf_type type;
    struct v4l2_buffer buf;

    int ret = 0;
    int idx = 0;

    for (int i = 0; i < FRAME_NUM; i++)
    {
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.length = 1;
        buf.index = i;
        ioctl(v4l2_fd, VIDIOC_QBUF, &buf);
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

    if (ioctl(v4l2_fd, VIDIOC_STREAMON, &type) == -1)
    {
        printf("VIDIOC_STREAMON error\r\n");
        return FAILURE;
    }
    return SUCCESS;
}

int my_v4l2::StreamOFF()
{
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    if (ioctl(v4l2_fd, VIDIOC_STREAMOFF, &type) == -1)
    {
        printf("VIDIOC_STREAMOFF error\r\n");
        return FAILURE;
    }
    return SUCCESS;
}

int my_v4l2::getFrame(int index)
{
    int ret;
    fd_set fds;
    struct timeval tv;

    FD_ZERO(&fds);
    FD_SET(v4l2_fd, &fds);

    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    ret = select(v4l2_fd + 1, &fds, NULL, NULL, &tv);

    if (0 == ret)
    {
        fprintf(stderr, "select timeout\n");
        exit(EXIT_FAILURE);
    }
    // 出队
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.length = 1;
    buf.m.planes = (struct v4l2_plane *)malloc(sizeof(struct v4l2_plane));
    memset(buf.m.planes, 0, sizeof(struct v4l2_plane));
    buf.index = index;
    if (ioctl(v4l2_fd, VIDIOC_DQBUF, &buf) < 0)
    {
        printf("VIDIOC_DQBUF error!\r\n");
        return FAILURE;
    }

    return SUCCESS;
}

int my_v4l2::quitBuffer(int index)
{
    CLEAR(buf);
    buf.m.planes = (struct v4l2_plane *)malloc(sizeof(struct v4l2_plane));
    memset(buf.m.planes, 0, sizeof(struct v4l2_plane));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.length = 1;
    buf.index = index;
    if (ioctl(v4l2_fd, VIDIOC_QBUF, &buf) < 0)
    {
        printf("VIDIOC_QBUF error \r\n");
        return FAILURE;
    }
    return SUCCESS;
}

color_t *my_v4l2::getBuffer(int index)
{
    return m_buffer[index].buffer;
}

unsigned int my_v4l2::getSize(int index)
{
    return m_buffer[index].size;
}

int my_v4l2::getInitStatus()
{
    return isInit;
}