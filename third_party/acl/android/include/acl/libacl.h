/* SPDX-License-Identifier: LGPL-2.1+
 * Android implementation of libacl extensions (acl/libacl.h)
 */
#ifndef _ACL_LIBACL_H
#define _ACL_LIBACL_H

#include <sys/acl.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Check if a specific permission is set in a permission set.
 * Returns 1 if the permission is set, 0 if not, -1 on error. */
int acl_get_perm(acl_permset_t permset, acl_perm_t perm);

/* Check if a file has an extended ACL (beyond the minimal mode bits).
 * Returns 1 if extended ACL exists, 0 if not, -1 on error. */
int acl_extended_fd(int fd);
int acl_extended_file(const char *path_p);
int acl_extended_file_nofollow(const char *path_p);

/* Convert mode bits to a minimal ACL */
acl_t acl_from_mode(mode_t mode);

/* Convert ACL to equivalent mode bits */
int acl_equiv_mode(acl_t acl, mode_t *mode_p);

#ifdef __cplusplus
}
#endif

#endif /* _ACL_LIBACL_H */
