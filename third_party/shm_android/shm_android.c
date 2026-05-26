/* SPDX-License-Identifier: LGPL-2.1+
 * Android implementation of POSIX shared memory (shm_open / shm_unlink)
 *
 * Android bionic explicitly does NOT implement shm_open/shm_unlink
 * (see bionic/libc/include/bits/posix_limits.h).
 *
 * This implementation stores shared memory objects as regular files under
 * /dev/shm, which must be mounted as tmpfs on the target before use:
 *
 *   mkdir -p /dev/shm
 *   mount -t tmpfs tmpfs /dev/shm
 *
 * The Android GVM kernel (Linux 6.x) supports the underlying mmap/ftruncate
 * syscalls on tmpfs files, so this works correctly for shared memory.
 */

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * Disable Bionic/Scudo heap pointer tagging at process startup.
 *
 * Issue: "Pointer tag was truncated" abort at free()
 * Bionic's Scudo allocator tags every heap allocation on aarch64 Android 11+
 * (API 30+). Any code path that strips the tag via reinterpret_cast<uintptr_t>
 * arithmetic and then passes the truncated pointer back to free()/realloc()
 * triggers a Scudo tag-mismatch abort. The SCORE shared-memory offset_ptr
 * arithmetic and several libc++ template instantiations do exactly that.
 *
 * Fix: disable Scudo's pointer tagging for the whole process before any
 * application-level allocation happens. Constructor priority 101 runs after
 * libc init but before user-level C++ static initializers (priority 65535).
 *
 * Tradeoff: disabling tagging weakens Bionic's heap-corruption diagnostics.
 * Acceptable for testing. For production, a per-binary opt-out via the
 * .note.android.memtag ELF note is preferable.
 * ============================================================ */
#if defined(__ANDROID__) || defined(ANDROID)
#include <malloc.h>

#ifndef M_BIONIC_SET_HEAP_TAGGING_LEVEL
#define M_BIONIC_SET_HEAP_TAGGING_LEVEL (-204)
#endif
#ifndef M_HEAP_TAGGING_LEVEL_NONE
#define M_HEAP_TAGGING_LEVEL_NONE 0
#endif

__attribute__((constructor(101)))
static void disable_heap_pointer_tagging(void)
{
    (void)mallopt(M_BIONIC_SET_HEAP_TAGGING_LEVEL, M_HEAP_TAGGING_LEVEL_NONE);
}
#endif  /* __ANDROID__ */

/* Directory used to back shared memory objects */
static const char kShmDir[] = "/dev/shm";

/**
 * Build the filesystem path for a shared memory object.
 * POSIX names start with '/', e.g. "/my_shm_object".
 * We strip the leading '/' and map to kShmDir/name.
 */
static int build_path(const char* name, char* out, size_t outsz)
{
    /* Strip leading '/' characters (POSIX requirement) */
    while (name && *name == '/') ++name;

    if (!name || !*name) {
        errno = EINVAL;
        return -1;
    }

    /* Reject names with embedded '/' (POSIX requirement) */
    if (strchr(name, '/') != NULL) {
        errno = EINVAL;
        return -1;
    }

    int n = snprintf(out, outsz, "%s/%s", kShmDir, name);
    if (n < 0 || (size_t)n >= outsz) {
        errno = ENAMETOOLONG;
        return -1;
    }
    return 0;
}

/**
 * shm_open — open or create a POSIX shared memory object
 *
 * Creates the backing directory if needed, then opens/creates the file.
 */
int shm_open(const char* name, int oflag, mode_t mode)
{
    /* Idempotent mkdir — EEXIST is fine, other errors are fatal */
    if (mkdir(kShmDir, 0777) != 0 && errno != EEXIST) {
        return -1;
    }

    char path[256];
    if (build_path(name, path, sizeof(path)) != 0) {
        return -1;
    }

    return open(path, oflag | O_CLOEXEC, mode);
}

/**
 * shm_unlink — remove a POSIX shared memory object
 */
int shm_unlink(const char* name)
{
    char path[256];
    if (build_path(name, path, sizeof(path)) != 0) {
        return -1;
    }
    return unlink(path);
}
