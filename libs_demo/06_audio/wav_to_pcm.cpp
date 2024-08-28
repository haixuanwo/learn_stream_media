
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// WAV文件头结构体定义
typedef struct {
    char chunkID[4];        // "RIFF"
    uint32_t chunkSize;     // 文件大小 - 8
    char format[4];         // "WAVE"
    char subchunk1ID[4];    // "fmt "
    uint32_t subchunk1Size; // 子块1大小
    uint16_t audioFormat;   // 音频格式（1表示PCM）
    uint16_t numChannels;   // 声道数
    uint32_t sampleRate;    // 采样率
    uint32_t byteRate;      // 比特率（每秒字节数）
    uint16_t blockAlign;    // 数据块对齐单位（每个样本的字节数）
    uint16_t bitsPerSample; // 采样位数
    char subchunk2ID[4];    // "data"
    uint32_t subchunk2Size; // 数据大小
} WavHeader;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s input.wav output.pcm\n", argv[0]);
        return -1;
    }

    WavHeader header;

    // 打开WAV文件
    FILE *wavFile = fopen(argv[1], "rb");
    if (!wavFile)
    {
        printf("无法打开文件: %s\n", argv[1]);
        return 1;
    }

    // 读取WAV文件头
    if (1 != fread(&header, sizeof(WavHeader), 1, wavFile))
    {
        printf("fread WavHeader failed\n");
        fclose(wavFile);
        return -1;
    }

    // 打印WavHeader结构体中所有元素
    printf("chunkID: %c%c%c%c\n", header.chunkID[0], header.chunkID[1], header.chunkID[2], header.chunkID[3]);
    printf("chunkSize: %u\n", header.chunkSize);
    printf("format: %c%c%c%c\n", header.format[0], header.format[1], header.format[2], header.format[3]);
    printf("subchunk1ID: %c%c%c%c\n", header.subchunk1ID[0], header.subchunk1ID[1], header.subchunk1ID[2], header.subchunk1ID[3]);
    printf("subchunk1Size: %u\n", header.subchunk1Size);
    printf("audioFormat: %u\n", header.audioFormat);
    printf("numChannels: %u\n", header.numChannels);
    printf("sampleRate: %u\n", header.sampleRate);
    printf("byteRate: %u\n", header.byteRate);
    printf("blockAlign: %u\n", header.blockAlign);
    printf("bitsPerSample: %u\n", header.bitsPerSample);
    printf("subchunk2ID: %c%c%c%c\n", header.subchunk2ID[0], header.subchunk2ID[1], header.subchunk2ID[2], header.subchunk2ID[3]);
    printf("subchunk2Size: %u\n", header.subchunk2Size);

    // 确保音频格式为PCM
    if (header.audioFormat != 1)
    {
        printf("不支持的音频格式\n");
        fclose(wavFile);
        return -1;
    }

    header.subchunk2Size = header.chunkSize + 8 - sizeof(WavHeader); // 计算pcm数据大小

    // 读取PCM数据
    int16_t *pcmData = (int16_t *)malloc(header.subchunk2Size);
    if (!pcmData)
    {
        printf("内存分配失败\n");
        fclose(wavFile);
        return -1;
    }

    if (1 != fread(pcmData, header.subchunk2Size, 1, wavFile))
    {
        printf("fread WavHeader failed\n");
        free(pcmData);
        fclose(wavFile);
        return -1;
    }

    // wav关闭文件
    fclose(wavFile);

    // 打开pcm文件
    FILE *pcmFile = fopen(argv[2], "wb");
    if (!pcmFile)
    {
        printf("无法打开文件:%s\n", argv[2]);
        free(pcmData);
        return -1;
    }

    if (1 != fwrite(pcmData, header.subchunk2Size, 1, pcmFile))
    {
        printf("fwrite pcmData failed\n");
        fclose(wavFile);
        free(pcmData);
        return -1;
    }

    fclose(pcmFile);
    free(pcmData);

    printf("congraduation success!\n");
    return 0;
}
