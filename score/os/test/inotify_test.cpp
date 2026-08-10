/********************************************************************************
 * Copyright (c) 2025 Contributors to the Eclipse Foundation
 *
 * See the NOTICE file(s) distributed with this work for additional
 * information regarding copyright ownership.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 ********************************************************************************/
#include "score/os/inotify.h"
#include "score/os/errno.h"

#include <sys/inotify.h>

#include <gtest/gtest.h>

namespace score
{
namespace os
{
namespace test
{

struct InotifyTest : ::testing::Test
{
    void SetUp() override
    {
        char tmp_dir_template[] = "dirent_test.XXXXXX";
        char* result = ::mkdtemp(tmp_dir_template);
        ASSERT_NE(result, nullptr);
        temp_dir_ = result;

        fd = score::os::Inotify::instance().inotify_init();
        EXPECT_TRUE(fd.has_value());
    };

    void TearDown() override
    {
        ASSERT_EQ(::rmdir(temp_dir_.c_str()), 0);
    }

    std::string temp_dir_;
    score::cpp::expected<int, score::os::Error> fd;
};

TEST_F(InotifyTest, AddWatchSuccessfull)
{
    RecordProperty("Verifies", "SCR-46010294");
    RecordProperty("ASIL", "B");
    RecordProperty("Description", "InotifyTest Add Watch Successfull");
    RecordProperty("TestType", "Interface test");
    RecordProperty("DerivationTechnique", "Generation and analysis of equivalence classes");

    const auto wd = score::os::Inotify::instance().inotify_add_watch(
        fd.value(), temp_dir_.c_str(), score::os::Inotify::EventMask::kAccess);
#if defined(__QNX__)
    // QNX inotify requires a non-tmpfs filesystem (e.g. qnx6). The temp directory
    // created by mkdtemp resides on a filesystem that does not support inotify on
    // this target. Skip rather than fail so the error-path tests remain unaffected.
    if (!wd.has_value())
    {
        GTEST_SKIP() << "inotify_add_watch failed; filesystem of " << temp_dir_
                     << " does not support inotify on this QNX target";
    }
#endif
    ASSERT_TRUE(wd.has_value());
}

TEST_F(InotifyTest, AddWatchFailsIfDirectoryDoesNotExist)
{
    RecordProperty("Verifies", "SCR-46010294");
    RecordProperty("ASIL", "B");
    RecordProperty("Description", "InotifyTest Add Watch Fails If Directory Does Not Exist");
    RecordProperty("TestType", "Interface test");
    RecordProperty("DerivationTechnique", "Generation and analysis of equivalence classes");

    const auto wd =
        score::os::Inotify::instance().inotify_add_watch(fd.value(), "/blah", score::os::Inotify::EventMask::kInCreate);
    ASSERT_FALSE(wd.has_value());
    EXPECT_EQ(wd.error(), score::os::Error::Code::kNoSuchFileOrDirectory);
}

TEST_F(InotifyTest, AddWatchFailsWithoutInit)
{
    RecordProperty("Verifies", "SCR-46010294");
    RecordProperty("ASIL", "B");
    RecordProperty("Description", "InotifyTest Add Watch Fails Without Init");
    RecordProperty("TestType", "Interface test");
    RecordProperty("DerivationTechnique", "Generation and analysis of equivalence classes");

    const auto wd =
        score::os::Inotify::instance().inotify_add_watch(0, temp_dir_.c_str(), score::os::Inotify::EventMask::kInCreate);
    ASSERT_FALSE(wd.has_value());
    // Error is not specified and thus OS specific
}

TEST_F(InotifyTest, AddWatchFailsWithEBADF)
{
    RecordProperty("Verifies", "SCR-46010294");
    RecordProperty("ASIL", "B");
    RecordProperty("Description", "InotifyTest Add Watch Fails With EBADF");
    RecordProperty("TestType", "Interface test");
    RecordProperty("DerivationTechnique", "Generation and analysis of equivalence classes");

    const auto wd =
        score::os::Inotify::instance().inotify_add_watch(-1, temp_dir_.c_str(), score::os::Inotify::EventMask::kInCreate);
    ASSERT_FALSE(wd.has_value());
    // Error is not specified and thus OS specific
}

TEST_F(InotifyTest, RemoveWatchSuccessfull)
{
    RecordProperty("Verifies", "SCR-46010294");
    RecordProperty("ASIL", "B");
    RecordProperty("Description", "InotifyTest Remove Watch Successfull");
    RecordProperty("TestType", "Interface test");
    RecordProperty("DerivationTechnique", "Generation and analysis of equivalence classes");

    const auto wd = score::os::Inotify::instance().inotify_add_watch(
        fd.value(), temp_dir_.c_str(), score::os::Inotify::EventMask::kInMovedTo);
#if defined(__QNX__)
    // QNX inotify requires a non-tmpfs filesystem (e.g. qnx6). Skip if the
    // filesystem of the temp directory does not support inotify on this target.
    if (!wd.has_value())
    {
        GTEST_SKIP() << "inotify_add_watch failed; filesystem of " << temp_dir_
                     << " does not support inotify on this QNX target";
    }
#endif
    ASSERT_TRUE(wd.has_value());
    const auto ret = score::os::Inotify::instance().inotify_rm_watch(fd.value(), wd.value());
    ASSERT_TRUE(ret.has_value());
}

TEST_F(InotifyTest, RemoveWatchFailsWithoutInit)
{
    RecordProperty("Verifies", "SCR-46010294");
    RecordProperty("ASIL", "B");
    RecordProperty("Description", "InotifyTest Remove Watch Fails Without Init");
    RecordProperty("TestType", "Interface test");
    RecordProperty("DerivationTechnique", "Generation and analysis of equivalence classes");

    const auto ret = score::os::Inotify::instance().inotify_rm_watch(1, 1);
    ASSERT_FALSE(ret.has_value());
    // Error is not specified and thus OS specific
}

TEST_F(InotifyTest, RemoveWatchFailsWithEBADF)
{
    RecordProperty("Verifies", "SCR-46010294");
    RecordProperty("ASIL", "B");
    RecordProperty("Description", "InotifyTest Remove Watch Fails With EBADF");
    RecordProperty("TestType", "Interface test");
    RecordProperty("DerivationTechnique", "Generation and analysis of equivalence classes");

    const auto ret = score::os::Inotify::instance().inotify_rm_watch(-1, 0);
    ASSERT_FALSE(ret.has_value());
    EXPECT_EQ(ret.error(), score::os::Error::Code::kBadFileDescriptor);
}

TEST_F(InotifyTest, RemoveWatchFailsWithInvalidArguments)
{
    RecordProperty("Verifies", "SCR-46010294");
    RecordProperty("ASIL", "B");
    RecordProperty("Description", "InotifyTest Remove Watch Fails With Invalid Arguments");
    RecordProperty("TestType", "Interface test");
    RecordProperty("DerivationTechnique", "Generation and analysis of equivalence classes");

    const auto ret = score::os::Inotify::instance().inotify_rm_watch(0, 0);
    ASSERT_FALSE(ret.has_value());
    // Error is not specified and thus OS specific
}

}  // namespace test
}  // namespace os
}  // namespace score
