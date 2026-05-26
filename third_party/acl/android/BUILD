# Android no-op ACL stub library.
# Provides sys/acl.h + acl/libacl.h headers and a C stub implementation
# so that score/os/acl_impl.cpp compiles and links on Android without
# modification. All functions are no-ops — Android tmpfs has no xattr support.

cc_library(
    name = "acl_android",
    srcs = ["src/acl_stub.c"],
    hdrs = [
        "include/sys/acl.h",
        "include/acl/libacl.h",
    ],
    includes = ["include"],
    target_compatible_with = [
        "@platforms//os:android",
        "@platforms//cpu:aarch64",
    ],
    visibility = ["//visibility:public"],
)

# Keep acl_headers as an alias for backwards compatibility
alias(
    name = "acl_headers",
    actual = ":acl_android",
    visibility = ["//visibility:public"],
)
