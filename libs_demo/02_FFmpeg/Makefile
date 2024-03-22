
GXX=g++
GCC=gcc
STRIP=strip
RM=rm -rf

SOURCE = share/ffmpeg/examples/demux_decode.c
# share/ffmpeg/examples/demuxing_decoding.c  # 去混流器、解码音视频
#	share/ffmpeg/examples/decode_video.c
#   share/ffmpeg/examples/decode_audio.c
#	share/ffmpeg/examples/avio_reading.c
#   share/ffmpeg/examples/hw_decode.c

#   share/ffmpeg/examples/encode_audio.c
#   share/ffmpeg/examples/encode_video.c

#   share/ffmpeg/examples/muxing.c
#   share/ffmpeg/examples/remuxing.c

TARGET = bin/main
INCLUDE = include
LIB = lib

#CFLAGS = -lavdevice -lavformat -lavfilter -lavcodec -lswresample -lswscale -lavutil
CFLAGS = -lavformat -lavcodec -lswresample -lavutil
# CFLAGS +=  -lz -llzma -lbz2 -lX11 -lva-drm -lva -lm
CFLAGS += -lm -lX11 -lva-drm -lva-x11 -lva -lz -llzma -lbz2
CFLAGS += -Wall -g -O2

all:
	${GCC} ${SOURCE} -I ${INCLUDE} -L ${LIB} ${CFLAGS} -o ${TARGET}
	${STRIP} ${TARGET}

clean:
	${RM} -rf ${TARGET} *.o

# .PHONE伪目标，避免与文件同名
.PHONY:all clean install
