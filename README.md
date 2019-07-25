# UnitySimpleRenderer
实现一个Unity Scene效果的渲染器

1.只利用SDL访问线性帧缓冲的能力，不用任何图形库函数
2.obj文件，bmp文件读取
3.实现了Unity矩阵、向量、四元数的大部分算法，并提供了SIMD实现
4.基于AABB的物体视锥体剔除
5.视椎体裁剪，世界坐标系下和屏幕坐标系下的背面裁剪，视口裁剪
6.直线光栅化，三角形光栅化
7.平行光、点光源、聚光灯的颜色、强度、衰减算法
8.gouraud着色、phong着色
9.深度测试，透明度混合，mipmap, 双线性滤波，透视校正
10.立方体纹理，天空盒

![渲染器截图](https://github.com/JBLRADISH/SimpleRenderer/blob/master/SimpleRenderer/Renderer.jpg)
![Unity Game窗口截图](https://github.com/JBLRADISH/SimpleRenderer/blob/master/SimpleRenderer/Unity.jpg)
