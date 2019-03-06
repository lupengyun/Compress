# Compress
哈夫曼算法压缩ｊｐｅｇ

#### 编写步骤
* 1、git clone git://git.linaro.org/people/tomgall/libjpeg-turbo/libjpeg-turbo.git -b linaro-android
* ２、将clone下来的libjpeg-turbo文件夹名字改为jni
* ３、使用ndk编译 ndk-build APP_ABI=armeabi-v7a,armeabi 得到两个架构的so文件
* ４、创建ａｎｄｒｏｉｄ－ｎｄｋ工程，编写CMakeLists.txt文件
* ５、剩下的参考此工程
  > 压缩图片流程
  > * bitmap 转为　rgb
  > * 为ｊｐｅｇ分配空间
  > * 获取输出文件信息
  > * 设置压缩参数
  > * 开始压缩
  
 #### 参考
 > * https://blog.csdn.net/a992036795/article/details/53814178
 > * https://www.jianshu.com/p/5022c0457c61
