/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2024-03-18 16:07:11
 * @LastEditors: Clark
 * @LastEditTime: 2024-03-18 16:10:12
 * @Description: file content
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>

int main() {
    int fd;
    struct v4l2_ext_controls controls;
    struct v4l2_ext_control control;

    // 打开视频设备文件
    fd = open("/dev/video0", O_RDWR);
    if (fd == -1) {
        perror("Failed to open video device");
        return -1;
    }

    // 设置要发送的自定义指令
    // control.id = V4L2_CID_PRIVATE_BASE + 1;  // 自定义指令的ID，可以根据需要进行调整
    control.id = 0x0d;  // 自定义指令的ID，可以根据需要进行调整
    control.value = 123;  // 自定义指令的值

    // controls.ctrl_class = V4L2_CTRL_CLASS_USER;
    controls.ctrl_class = 0x13;
    controls.count = 1;
    controls.controls = &control;

    // 发送自定义指令
    if (ioctl(fd, VIDIOC_S_EXT_CTRLS, &controls) == -1) {
        perror("Failed to send custom command");
        close(fd);
        return -1;
    }

    // 关闭视频设备文件
    close(fd);

    return 0;
}
