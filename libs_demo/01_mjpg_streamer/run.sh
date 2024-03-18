
###
 # @Author: Clark
 # @Email: haixuanwoTxh@gmail.com
 # @Date: 2024-03-16 15:00:36
 # @LastEditors: Clark
 # @LastEditTime: 2024-03-16 15:12:18
 # @Description: file content
###

export LD_LIBRARY_PATH=`pwd`/lib/mjpg-streamer
#./bin/mjpg_streamer -i "input_uvc.so -d /dev/video0 -r 1920x1080 -f 30" -o "output_http.so -w share/mjpg-streamer/www"
./bin/mjpg_streamer -i "input_uvc.so -d /dev/video0 -r 640x480 -f 30" -o "output_http.so -w share/mjpg-streamer/www"
