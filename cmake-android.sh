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

# Uncomment arm/x86 blocks according to your needs.
# Typically arm is more popular for deployments. And x86 is the only convenient (performance-wise) build
# to be tested within an emulator on a desktop computer.
# Look for the call to add_qt_android_apk in CMakeLists.txt for further important instructions concerning
# deployment builds.

# set
#-DUL_DEPLOYMENT_BUILD=ON \
# below for deployment build

### x86 ###
# Note, you might need to rename (copy) some subdirs in your NDK.

build_dir=_build-$subdir_lowercase-android

mkdir -p $build_dir

cd $build_dir || exit


#--log-level=DEBUG --debug-find \
#$ANDROID_NDK/build/cmake/android.toolchain.cmake \
#../sdks/util/buildenv/cmake_util/toolchain/android.cmake \

# ANDROID_TOOLCHAIN
# ANDROID_ABI
# ANDROID_PLATFORM
# ANDROID_STL
# ANDROID_PIE
# ANDROID_CPP_FEATURES
# ANDROID_ALLOW_UNDEFINED_SYMBOLS
# ANDROID_ARM_MODE
# ANDROID_ARM_NEON
# ANDROID_DISABLE_FORMAT_STRING_CHECKS
# ANDROID_CCACHE

ndk_path=$dev_sdk_path/Android/Sdk/ndk/21.3.6528147

cmake \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=$build_config \
-DUL_DEPLOYMENT_BUILD=OFF \
-DCMAKE_TOOLCHAIN_FILE=$ndk_path/build/cmake/android.toolchain.cmake \
-DUL_JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64 \
-DUL_ANDROID_NDK=$ndk_path \
-DANDROID_STL="c++_shared" \
-DANDROID_ABI="arm64-v8a" \
-DUL_QT5_VERSION=5.15.2 \
-DUL_QT_COMPILER_SUBDIR=android \
.. -Wno-deprecated

cd ..
cmake --build $build_dir
