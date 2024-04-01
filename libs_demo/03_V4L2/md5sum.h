/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2022-04-26 11:59:32
 * @LastEditors: Clark
 * @LastEditTime: 2022-07-22 11:05:05
 * @Description: file content
 */

#ifndef MD5SUM_H
#define MD5SUM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MD5SUM_CHAR_NUM 32

typedef unsigned int u32;

typedef struct {
    u32 A,B,C,D;	  /* chaining variables */
    u32  nblocks;
    unsigned char buf[64];
    int  count;
}MD5_CONTEXT;

void md5_init(MD5_CONTEXT *ctx);

void md5_write(MD5_CONTEXT *hd, unsigned char *inbuf, size_t inlen);

void md5_final(MD5_CONTEXT *hd);

/**
 * @brief 获取数据的md5sum
 * @param data          数据
 * @param dataLen       数据长度
 * @param md5sum        获取的md5sum
 * @param md5sumSize    存md5sum空间大小
 * @return true
 * @return false
 */
bool get_md5sum_from_data(uint8_t *data, int dataLen, char *md5sumBuf, int md5sumBufSize);

#endif
