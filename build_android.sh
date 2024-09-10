#!/bin/sh

$HOME/android-ndk-r27b/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android34-clang \
    -Ofast test.c -o test
