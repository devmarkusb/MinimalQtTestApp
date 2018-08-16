#!/usr/bin/env bash

set -x

if [ -z "$1" ]
then
    echo "You need to pass Debug or Release as parameter."
    exit
fi

echo "Hint: you might want to delete cmake-build-debug/release dir(s) for clean cmake run first"

build_config=$1
subdir_lowercase=${1,,}


### arm ###

build_dir=cmake-build-$subdir_lowercase-android-arm

mkdir -p $build_dir

cd $build_dir

cmake \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=$build_config \
-DTOO_ANDROID_TOOLCHAIN=qt \
-DCMAKE_TOOLCHAIN_FILE=../sdks/tfl/buildenv/cmake_util/toolchain/android.cmake \
-DANDROID_STL="gnustl_shared" \
-DANDROID_ABI="armeabi-v7a" \
-DTOO_QT5_VERSION=5.9.1 -DTOO_QT_COMPILER_SUBDIR=android_armv7 \
..

cd ..
cmake --build $build_dir


### x86 ###
# Note, yout might need to rename (copy) some subdirs in your NDK.

build_dir=cmake-build-$subdir_lowercase-android-x86

mkdir -p $build_dir

cd $build_dir

cmake \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=$build_config \
-DTOO_ANDROID_TOOLCHAIN=qt \
-DCMAKE_TOOLCHAIN_FILE=../sdks/tfl/buildenv/cmake_util/toolchain/android.cmake \
-DANDROID_STL="gnustl_shared" \
-DANDROID_ABI="x86" \
-DTOO_QT5_VERSION=5.9.1 -DTOO_QT_COMPILER_SUBDIR=android_x86 \
..

cd ..
cmake --build $build_dir



#-DCMAKE_SYSTEM_NAME=Android \
#-DCMAKE_C_COMPILER=$dev_sdk_path/AndroidNDK/android-toolchain/bin/arm-linux-androideabi-gcc \
#-DCMAKE_CXX_COMPILER=$dev_sdk_path/AndroidNDK/android-toolchain/bin/arm-linux-androideabi-g++ \
#-DANDROID_NDK=$dev_sdk_path/AndroidNDK/android-ndk-r15c \
#-DCMAKE_MAKE_PROGRAM=$dev_sdk_path/AndroidNDK/android-toolchain/bin/make \
#-DANDROID_TOOLCHAIN_NAME="x86-4.9" \
#-DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-4.9" \
#-DANDROID_NATIVE_API_LEVEL=android-16 \
#-DANDROID_PLATFORM=android-16
#-DANDROID_ABI="armeabi-v7a" \


