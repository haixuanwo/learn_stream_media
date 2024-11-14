<!--
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2024-11-13 16:08:12
 * @LastEditors: Clark
 * @LastEditTime: 2024-11-14 16:10:06
 * @Description: file content
-->

# FFmpeg 5.x
FFmpeg tag: n5.1.3

## Build FFmpeg lib
```
cd FFmpeg
mkdir install
./configure --enable-shared --enable-postproc --enable-gpl --prefix=install
make -j8
make install
```

# FFmpeg app
编译应用程序
./buid.sh


- 解码视频与音频
./build/04_demuxing_decoding nocturne.mp4 test.NV12 test.pcm

- 播放NV12视频
./bin/ffplay -f rawvideo -video_size 640x360 test.NV12

- 播放音频
./bin/ffplay -f f32le -ac 1 -ar 44100 test.pcm


