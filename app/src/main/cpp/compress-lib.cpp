

#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

extern "C" {
#include "inculde/jpeglib.h"
#include "inculde/android/config.h"
}

#include <stdio.h>
#include <setjmp.h>
#include <malloc.h>

#define TAG "JNI_LOG"
#define Loge(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LogI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

typedef u_int8_t BYTE;
struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

extern "C"
boolean generateCompress(BYTE *data, int width, int height, const char *fileout, int qulit) {
    int mComponent = 3;
    struct jpeg_compress_struct jcs;
//自定义的error
    struct my_error_mgr jem;
    jcs.err = jpeg_std_error(&jem.pub);

    if (setjmp(jem.setjmp_buffer)) {
        return false;
    }

    //为ｊｐｅｇ分配空间并初始化
    LogI("分配空间并初始化");
    jpeg_create_compress(&jcs);
    //获取文件信息
    LogI("获取文件信息");
    FILE *f = fopen(fileout, "wb");

    if (f == nullptr) {
        Loge("打开文件失败");
        return false;
    }

    LogI("指定压缩数据源");
    jpeg_stdio_dest(&jcs, f);
    jcs.image_width = width;
    jcs.image_height = height;

    jcs.arith_code = false;
    jcs.input_components = mComponent;
    jcs.in_color_space = JCS_RGB;

    jpeg_set_defaults(&jcs);
    jcs.optimize_coding = true;

    LogI("设置压缩参数：%d", qulit);
    jpeg_set_quality(&jcs, qulit, true);
    LogI("开始压缩");
    jpeg_start_compress(&jcs, true);

    JSAMPROW row_point[1];
    int row_stride;
    row_stride = jcs.image_width * mComponent;
    while (jcs.next_scanline < jcs.image_height) {
        row_point[0] = &data[jcs.next_scanline * row_stride];
        jpeg_write_scanlines(&jcs, row_point, 1);
    }

    if (jcs.optimize_coding) {
        LogI("使用了哈夫曼算法完成压缩");
    } else {
        LogI("未使用哈夫曼算法");
    }

    //压缩完毕
    jpeg_finish_compress(&jcs);
    //释放资源
    jpeg_destroy_compress(&jcs);
    fclose(f);
    return true;

}


extern "C"
boolean
compress(JNIEnv *env, jobject instance, jobject bitmap, jstring outputfile,
         jint qulit) {
    LogI("begin compress------------");
    const char *fileName = env->GetStringUTFChars(outputfile, 0);
    AndroidBitmapInfo androidBitmapInfo;
    BYTE *pixelsColor;
    int ret;
    BYTE *data;
    BYTE *tempData;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &androidBitmapInfo)) < 0) {
        Loge("获取bitmap信息失败");
        return false;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, (void **) &pixelsColor)) < 0) {
        Loge("获取像素颜色失败");
        return false;
    }

    LogI("bitmap: width = %d height = %d format = %d", androidBitmapInfo.width,
         androidBitmapInfo.height, androidBitmapInfo.format);

    BYTE r, g, b;
    int color;

    int w, h, format;
    w = androidBitmapInfo.width;
    h = androidBitmapInfo.height;
    format = androidBitmapInfo.format;

    data = (BYTE *) malloc(w * h * 3);
    tempData = data;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
                color = (*(int *) pixelsColor);
                //这里面对应的颜色RGBA各占８位
                b = (color >> 16) & 0xFF;
                g = (color >> 8) & 0xFF;
                r = (color >> 0) & 0xFF;

                *data = r;
                *(data + 1) = g;
                *(data + 2) = b;
                data += 3;
                pixelsColor += 4;
            } else {
                return false;
            }
        }
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    //开始压缩
    if (!generateCompress(tempData, w, h, fileName, qulit)) {
        return false;
    }
    free(data);
    free(tempData);
    return true;

}


static const JNINativeMethod mMethod[] = {
        {"compress", "(Landroid/graphics/Bitmap;Ljava/lang/String;I)Z", (void *) compress}
};

static const char *className = "com/cylan/compress/MainActivity";

jint JNI_OnLoad(JavaVM *vm, void *v) {
    JNIEnv *env = 0;
    int r = vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (r != JNI_OK) {
        Loge("获取JNIEnv失败");
        return -1;
    }
    jclass jcla = env->FindClass(className);
    r = env->RegisterNatives(jcla, mMethod, sizeof(mMethod) / sizeof(JNINativeMethod));
    if (r != JNI_OK) {
        Loge("注册方法失败");
        return -1;
    }

    return JNI_VERSION_1_6;
}