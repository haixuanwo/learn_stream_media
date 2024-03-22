/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2024-03-18 16:35:50
 * @LastEditors: Clark
 * @LastEditTime: 2024-03-19 11:32:02
 * @Description: file content
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <linux/videodev2.h>
#include <linux/uvcvideo.h>
#include <linux/usb/video.h>
#include <errno.h>

// #define UVC_RC_UNDEFINED				0x00
// #define UVC_SET_CUR					0x01
// #define UVC_GET_CUR					0x81
// #define UVC_GET_MIN					0x82
// #define UVC_GET_MAX					0x83
// #define UVC_GET_RES					0x84
// #define UVC_GET_LEN					0x85
// #define UVC_GET_INFO					0x86
// #define UVC_GET_DEF					0x87

#define UVC_EU1_ID 0x04
#define EU1_TEST_CMD 13
#define EU1_TEST_CMD_LEN 1

int main(int argc, char **argv)
{
    int fd = open("/dev/video0", O_RDWR);
    if(fd <= 0)
    {
        printf("open failed!\n");
        return -1;
    }

    unsigned char buf[EU1_TEST_CMD_LEN];// = {1, 2, 3, 4, 5};
    struct uvc_xu_control_query xu_ctrl_query =
    {
        .unit     = UVC_EU1_ID,
        .selector = EU1_TEST_CMD,
        // .query    = UVC_GET_CUR,
        .query    = UVC_GET_INFO,
        .size     = EU1_TEST_CMD_LEN,
        .data     = buf,
    };
    int ret = ioctl(fd, UVCIOC_CTRL_QUERY, &xu_ctrl_query);
    if(ret < 0)
    {
        printf("query fail!\n");
        printf("%s\n",strerror(errno));
    }else{
        printf("query success!\n");
        int i;
        printf("data:");
        for(i = 0;i < EU1_TEST_CMD_LEN;i++)
        {
            printf("%#x ",buf[i]);
        }
        printf("\n");
    }
    close(fd);
    return 0;
}