/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2024-11-13 15:00:16
 * @LastEditors: Clark
 * @LastEditTime: 2024-11-13 15:45:23
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
    if (argc < 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        return -1;
    }

    const char *fileName = argv[1];

    //创建AVFormatContext对象
    AVFormatContext *fmt_ctx = avformat_alloc_context();//创建对象并初始化

    //打开文件
    if (avformat_open_input(&fmt_ctx, fileName, NULL, NULL) < 0)
    {
        printf("Cannot open file\n");
        return -1;
    }

    //查找流信息（音频流和视频流）
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
    {
        printf("Cannot find stream information\n");
        return -1;
    }

    //输出视频信息
    av_dump_format(fmt_ctx, 0, fileName, 0);

    avformat_close_input(&fmt_ctx);
    return 0;
}
