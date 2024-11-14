/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2024-11-13 15:00:16
 * @LastEditors: Clark
 * @LastEditTime: 2024-11-13 15:41:45
 * @Description: file content
 */

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/ffversion.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
// #include "libpostproc/postprocess.h"
}

#include <cstdio>

int main(int argc, char **argv)
{
    unsigned codecVersion = avcodec_version();

    int ver_major = (codecVersion >> 16) & 0xFF;
    int ver_minor = (codecVersion >> 8) & 0xFF;
    int ver_micro = codecVersion & 0xFF;

    printf("FFmpeg version[%s]\n %d=%d.%d.%d\n",
        FFMPEG_VERSION, codecVersion, ver_major, ver_minor, ver_micro);

    return 0;
}
