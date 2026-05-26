# Android implementation of POSIX shared memory (shm_open / shm_unlink)
# Android bionic does not provide these functions. This library implements
# them using files in /dev/shm (tmpfs) on the Android GVM vendor partition.

cc_library(
    name = "shm_android",
    srcs = ["shm_android.c"],
    copts = ["-std=c11"],
    target_compatible_with = [
        "@platforms//os:android",
        "@platforms//cpu:aarch64",
    ],
    visibility = ["//visibility:public"],
)
