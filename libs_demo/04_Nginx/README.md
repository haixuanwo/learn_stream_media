<!--
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2021-12-28 16:01:51
 * @LastEditors: Clark
 * @LastEditTime: 2024-03-26 22:46:13
 * @Description: file content
-->
# learn_nginx_rtmp
学习Nginx+rtmp模块

运行脚本，自动编译安装应用
run.sh


yum -y install gcc-c++ //c语言环境
yum install -y pcre pcre-devel //正则
yum install -y zlib zlib-devel //lib包
yum install -y openssl openssl-devel //插件


https://github.com/winshining/nginx-http-flv-module/tree/master
https://github.com/arut/nginx-rtmp-module/tree/master

nginx-http-flv-module v1.2.6
nginx-1.18.0

# 编译安装
tar -zxvf pcre-8.34.tar.gz
cd pcre-8.34
./configure
make && make install

tar -zxvf zlib-1.2.11.tar.gz
cd zlib-1.2.11
./configure
make && make install

tar -zxvf nginx-rtmp-module-1.2.1.tar.gz
tar -zxvf nginx-1.17.6.tar.gz
cd nginx-1.17.6

./configure --add-module=../nginx-rtmp-module-1.2.2 --prefix=../learn_nginx_rtmp/
make && make install

# 修改配置文件
cd ../learn_nginx_rtmp/
vim conf/nginx.conf

增加配置
--------------------------------------------------------------------------------------
rtmp {                #RTMP service
    server {
        listen 1935;  #port
        chunk_size 4096;   #data size

        application vod {
          play video;
          #play /home/txh/openSource/nginx-rtmp/video; #video path
        }

	application live{ #first live field
            live on;
	    hls on;    #这个参数把直播服务器改造成实时回放服务器。
            wait_key on;  #对视频切片进行保护，这样就不会产生马赛克了。
            #hls_path /home/txh/technical/nginx-rtmp/video/hls;  #切片视频文件存放位置。
            hls_path video/hls;
            hls_fragment 300s;     #每个视频切片的时长。
            hls_playlist_length 1800s;  #总共可以回看的事件，这里设置的是1分钟。
            hls_continuous on; #连续模式。
            hls_cleanup on;    #对多余的切片进行删除。
            hls_nested on;     #嵌套模式。

	    record all;    #record flv
            #record_path /home/txh/technical/nginx-rtmp/video/hls;
            record_path video/hls;
            record_suffix -%d-%b-%y-%T.flv;
	    record_unique on;
            record_interval 300s;
        }
    }
}
--------------------------------------------------------------------------------------
# 启动运行
./learn_nginx_rtmp/sbin/nginx -s reload

# 测试推流
ffmpeg -f v4l2 -framerate 10 -i /dev/video1 -q 10  -f flv rtmp://127.0.0.1/live/wei
ffmpeg -re -i test.mp4 -vcodec copy -acodec copy -f flv rtmp://127.0.0.1/live/wei
ffmpeg -re -i test.mp4 -vcodec copy -acodec copy -f flv rtmp://127.0.0.1/live/txh


# VLC播放器拉流
rtmp://192.168.2.59/live/wei
rtmp://192.168.2.59/live/txh
http://192.1682.59/test?app=live&stream=wei
