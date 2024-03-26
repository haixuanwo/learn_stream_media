
###
 # @Author: Clark
 # @Email: haixuanwoTxh@gmail.com
 # @Date: 2021-12-28 14:49:25
 # @LastEditors: Clark
 # @LastEditTime: 2024-03-22 00:04:51
 # @Description: file content
###

set -x
ROOT=`pwd`
NGINX_SOURCE=nginx-1.21.3.tar.gz
INSTALL_PATH=${ROOT}/learn_nginx_rtmp

# 1 下载nginx-rtmp-module
if [ ! -d "nginx-rtmp-module" ]
then
  git clone https://github.com/arut/nginx-rtmp-module.git
fi

# 2 下载Nginx，编译安装
if [ ! -e ${NGINX_SOURCE} ]
then
wget http://nginx.org/download/${NGINX_SOURCE}
tar -zxvf ${NGINX_SOURCE}
fi

mkdir ${INSTALL_PATH}
cd nginx-1.21.3
./configure --add-module=../nginx-rtmp-module --prefix=${INSTALL_PATH}

make
make install

cd ${ROOT}
cp nginx.conf ${INSTALL_PATH}/conf/
mkdir ${INSTALL_PATH}/video

echo "run success"

