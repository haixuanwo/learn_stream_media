/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2024-03-18 16:35:50
 * @LastEditors: Clark
 * @LastEditTime: 2024-03-28 09:50:45
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
#include "md5sum.h"

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

static int videoDevice = -1;

bool init_uvc_xu_control_query()
{
    videoDevice = open("/dev/video0", O_RDWR);
    if(videoDevice <= 0)
    {
        printf("open failed!\n");
        return false;
    }

    printf("open success!\n");
    return true;
}

bool send_cmd_to_uvc_xu_control(unsigned char cmd, char *data, int len)
{
    struct uvc_xu_control_query xu_ctrl_query =
    {
        .unit     = UVC_EU1_ID,
        .selector = EU1_TEST_CMD,
        .query    = cmd,
        .data     = data,
        .size     = len,
    };

    int ret = ioctl(videoDevice, UVCIOC_CTRL_QUERY, &xu_ctrl_query);
    if(ret < 0)
    {
        printf("send cmd fail!\n");
        printf("%s\n",strerror(errno));
        return false;
    }

    return true;
}

/*
开始升级    0xf1固件size
升级数据包  0xf2固件数据包
升级结束    0xf3固件md5sum
*/
typedef enum {
    OTA_START = 0xf1,
    OTA_DATA = 0xf2,
    OTA_END = 0xf3
} OTACmd;


int send_ota_file(int argc, char **argv)
{
    if (2 != argc)
    {
        printf("Usage: %s <firmware_file>\n", argv[0]);
        return -1;
    }

    uint32_t len = 0;
    #define BUF_SIZE 60
    uint8_t buf[BUF_SIZE] = {0};

    FILE *fp = fopen(argv[1], "r+");
    if(fp < 0)
    {
        printf("open file fail!\n");
        return -1;
    }
    printf("open file[%s] success!\n", argv[1]);

    if(init_uvc_xu_control_query() == false)
    {
        printf("init uvc xu control query fail!\n");
        return -1;
    }

    if (send_cmd_to_uvc_xu_control(UVC_GET_LEN, buf, 2) == false)
    {
        printf("send cmd fail!\n");
        return -1;
    }

    MD5_CONTEXT ctx;
	md5_init (&ctx);

    // 1、开始升级
    // 固件大小
    uint32_t file_size = 0;
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    printf("file size is %d\n", file_size);

    buf[0] = OTA_START;
    buf[1] = (file_size >> 24) & 0xff;
    buf[2] = (file_size >> 16) & 0xff;
    buf[3] = (file_size >> 8) & 0xff;
    buf[4] = file_size & 0xff;

    if (send_cmd_to_uvc_xu_control(UVC_SET_CUR, buf, sizeof(buf)) == false)
    {
        printf("send cmd fail!\n");
        return -1;
    }
    printf("send start cmd success!\n");

    // 2、升级数据包
    buf[0] = OTA_DATA;
    uint8_t *file_buf = malloc(file_size);
    fread(file_buf, 1, file_size, fp);
    uint32_t send_size = 0;
    uint32_t need_send_size = file_size;
    uint32_t index = 0;
    while (1)
    {
        send_size = (need_send_size > (BUF_SIZE - 2) ? (BUF_SIZE - 2) : need_send_size);
        buf[1] = send_size;
        memcpy(buf + 2, file_buf + index, send_size);

        if (send_cmd_to_uvc_xu_control(UVC_SET_CUR, buf, sizeof(buf)) == false)
        {
            printf("send cmd fail!\n");
            return -1;
        }

        index += send_size;
        need_send_size -= send_size;
        if (need_send_size <= 0)
        {
            break;
        }
    }

    printf("send file success!\n");
    fclose(fp);

    // 3、升级结束
    // 发送固件md5sum
    memset(buf, 0, sizeof(buf));
    buf[0] = OTA_END;
    get_md5sum_from_data(file_buf, file_size, buf + 1, sizeof(buf) - 1);
    if (send_cmd_to_uvc_xu_control(UVC_SET_CUR, buf, sizeof(buf)) == false)
    {
        printf("send cmd fail!\n");
        return -1;
    }

    printf("send end md5sum[%s] success!\n", buf + 1);
    return 0;
}

int receive_file(int argc, char **argv)
{
    if (2 != argc)
    {
        printf("Usage: %s <firmware_file>\n", argv[0]);
        return -1;
    }

    uint32_t len = 0;
    #define BUF_SIZE 60
    uint8_t buf[BUF_SIZE] = {0};

    FILE *fp = fopen(argv[1], "w+");
    if(fp < 0)
    {
        printf("open file fail!\n");
        return -1;
    }
    printf("open file[%s] success!\n", argv[1]);

    if(init_uvc_xu_control_query() == false)
    {
        printf("init uvc xu control query fail!\n");
        return -1;
    }

    // 1、接收固件大小
    if (send_cmd_to_uvc_xu_control(UVC_GET_LEN, buf, 2) == false)
    {
        printf("send cmd fail!\n");
        return -1;
    }

    while (1)
    {
        if (send_cmd_to_uvc_xu_control(UVC_GET_CUR, buf, sizeof(buf)) == false)
        {
            printf("send cmd fail!\n");
            return -1;
        }

        if (buf[0] == OTA_DATA)
        {
             fwrite(buf+2, 1, buf[1], fp);
             printf("receive len[%u] \n", buf[1]);
        }
        else if (buf[0] == OTA_END)
        {
            break;
        }
    }

    printf("receive file success!\n");
    fclose(fp);
    return 0;
}

int main(int argc, char **argv)
{
    send_ota_file(argc, argv);
    //receive_file(argc, argv);

    return 0;
}
