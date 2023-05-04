#ifndef COMMON_H
#define COMMON_H

// 32bits 分布
// 32  24  16   8   0
//  -----------------
//  | T | R | G | B |
//  -----------------
typedef unsigned int    color_t;
#define __u32           __uint32_t

// 摄像头数量，用来限制输入
#define MAX_CAM_NUM     2
// 摄像头分辨率
#define IMAGEWIDTH      1920
#define IMAGEHEIGHT     1080
// 屏幕对应的fb大小
#define SCREEN_W        1920
#define SCREEN_H        1080

#define FRAME_NUM       3
#define ION_BUF_NUM     3

#define SUCCESS         1
#define FAILURE         0


#define fourcc_code(a, b, c, d) ((__u32)(a) | ((__u32)(b) << 8) | \
                                 ((__u32)(c) << 16) | ((__u32)(d) << 24))

#define pixfmtstr(x) (x) & 0xff, ((x) >> 8) & 0xff, ((x) >> 16) & 0xff, ((x) >> 24) & 0xff

#define CLEAR(x)        memset(&(x), 0, sizeof(x))

#define DRM_FORMAT_ARGB8888     fourcc_code('A', 'R', '2', '4') /* [31:0] A:R:G:B 8:8:8:8 little endian */
#define DRM_FORMAT_ABGR8888     fourcc_code('A', 'B', '2', '4') /* [31:0] A:B:G:R 8:8:8:8 little endian */
#define DRM_FORMAT_RGBA8888     fourcc_code('R', 'A', '2', '4') /* [31:0] R:G:B:A 8:8:8:8 little endian */
#define DRM_FORMAT_BGRA8888     fourcc_code('B', 'A', '2', '4') /* [31:0] B:G:R:A 8:8:8:8 little endian */

#define DRM_FORMAT_NV12         fourcc_code('N', 'V', '1', '2') /* 2x2 subsampled Cr:Cb plane */
#define DRM_FORMAT_NV21         fourcc_code('N', 'V', '2', '1') /* 2x2 subsampled Cb:Cr plane */
#define DRM_FORMAT_NV16         fourcc_code('N', 'V', '1', '6') /* 2x1 subsampled Cr:Cb plane */
#define DRM_FORMAT_NV61         fourcc_code('N', 'V', '6', '1') /* 2x1 subsampled Cb:Cr plane */
#define DRM_FORMAT_NV24         fourcc_code('N', 'V', '2', '4') /* non-subsampled Cr:Cb plane */
#define DRM_FORMAT_NV42         fourcc_code('N', 'V', '4', '2') /* non-subsampled Cb:Cr plane */

#endif