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

build_dir=_build-$subdir_lowercase-android-x86

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

cmake \
-G "Ninja" \
-DCMAKE_BUILD_TYPE=$build_config \
-DUL_DEPLOYMENT_BUILD=OFF \
-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
-DANDROID_STL="c++_shared" \
-DANDROID_ABI="x86" \
-DANDROID_PLATFORM=19 \
-DUL_QT5_VERSION=5.9.9 \
-DUL_QT_COMPILER_SUBDIR=android_x86 \
.. -Wno-deprecated

cd ..
cmake --build $build_dir
