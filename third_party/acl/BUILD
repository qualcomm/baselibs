# *******************************************************************************
# Copyright (c) 2025 Contributors to the Eclipse Foundation
#
# See the NOTICE file(s) distributed with this work for additional
# information regarding copyright ownership.
#
# This program and the accompanying materials are made available under the
# terms of the Apache License Version 2.0 which is available at
# https://www.apache.org/licenses/LICENSE-2.0
#
# SPDX-License-Identifier: Apache-2.0
# *******************************************************************************
load("@bazel_skylib//lib:selects.bzl", "selects")

cc_library(
    name = "libacl",
    linkopts = [
        "-lacl",
    ],
    target_compatible_with = ["@platforms//os:linux", "@platforms//cpu:aarch64"],
    visibility = ["//visibility:public"],
)

selects.config_setting_group(
    name = "linux_aarch64",
    match_all = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

alias(
    name = "acl",
    actual = select({
        "@platforms//os:android": "//third_party/acl/android:acl_android",
        ":linux_aarch64": ":libacl",
        "//conditions:default": "@acl-deb//:acl",
    }),
    visibility = ["//visibility:public"],
)
