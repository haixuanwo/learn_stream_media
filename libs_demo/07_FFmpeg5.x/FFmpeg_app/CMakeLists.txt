
cmake_minimum_required(VERSION 3.22)

project(FFmpeg CXX)

set(CXX_FLAGS
 -g
 -O2
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 -Wall
 -Wextra
 # -m32
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++17
 -rdynamic
 )

include_directories(include)
link_directories(lib)

set(LIBS
    m
    X11
    va-drm
    va-x11
    va
    z
    lzma
    bz2
    avcodec
    avdevice
    avfilter
    avformat
    avutil
    postproc
    swresample
    swscale
)

add_executable(01_avcodec_version src/01_avcodec_version.cpp)
target_link_libraries(01_avcodec_version ${LIBS})

add_executable(02_stream_info src/02_stream_info.cpp)
target_link_libraries(02_stream_info ${LIBS})

add_executable(03_decode src/03_decode.cpp)
target_link_libraries(03_decode ${LIBS})

add_executable(04_demuxing_decoding src/04_demuxing_decoding.cpp)
target_link_libraries(04_demuxing_decoding ${LIBS})
