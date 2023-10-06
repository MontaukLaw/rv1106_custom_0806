#!/bin/sh

# You should modify the below line according your env's directory in your compile PC.
ENV_PATH=/home/thomas/rv1106/env

PATH=${ENV_PATH}/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH
export CROSS_COMPILE=arm-rockchip830-linux-uclibcgnueabihf
export CC=arm-rockchip830-linux-uclibcgnueabihf-gcc
export CXX=arm-rockchip830-linux-uclibcgnueabihf-gcc
export CPP=arm-rockchip830-linux-uclibcgnueabihf-g++
export GDB=arm-rockchip830-linux-uclibcgnueabihf-gdb
export AR=arm-rockchip830-linux-uclibcgnueabihf-ar
export STRIP=arm-rockchip830-linux-uclibcgnueabihf-strip
export SYSROOT=${ENV_PATH}/arm-rockchip830-linux-uclibcgnueabihf/arm-rockchip830-linux-uclibcgnueabihf/sysroot

