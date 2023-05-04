## 全志T507平台利用openGL和v4l2显示摄像头程序

### 环境

> 编译环境:Ubuntu 18.04 LTS<br>平台:Allwinner T507<br>交叉编译工具链: aarch64-linux-gnu(详细请看CmakeLists)

### 简介

这是一个在全志T507平台的简单抓取摄像头图像v4l2视频流到屏幕的demo程序，其中利用opengl来处理nv21到argb8888的格式变化。

### 模型

```text
         IMAGEWIDTH                                                  SCREEN_W                     
+--------------------------+                                +--------------------------+         
|                          |                                |                          |         
|                          |                                |                          | 
|                          | --v4l2--> opengl ---> fb0 -->  |                          |
|          camera          | IMAGEHEIGHT                    |         lcd(fb0)         | SCREEN_H
|                          |                                |                          |
|                          |                                |                          |
+--------------------------+                                +--------------------------+       
```

### 移植

1. 修改CmakeLists.txt中的`SDK_DIR`为T5 SDK路径
2. 修改`common.h`中的摄像头分辨率，fb大小等参数
3. `cd build`
4. `cmake ..` 然后 `make`,便能看到`v4l2-to-screen`的执行文件。
5. 将生成的`v4l2_to_screen`放入开发板中，并将shader_source中的文件放入开发板的root目录下
6. 执行`v4l2_to_screen`即可