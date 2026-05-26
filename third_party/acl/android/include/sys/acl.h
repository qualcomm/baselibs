/* SPDX-License-Identifier: LGPL-2.1+
 * Android implementation of POSIX ACL (sys/acl.h)
 * Uses kernel xattr syscalls (getxattr/setxattr) which are available
 * on Android bionic. The kernel supports POSIX ACL via the
 * "system.posix_acl_access" and "system.posix_acl_default" xattrs.
 */
#ifndef _SYS_ACL_H
#define _SYS_ACL_H

#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------
 * Opaque ACL types (POSIX.1e draft 17)
 * ----------------------------------------------------------------------- */
typedef struct __acl_t_struct    *acl_t;
typedef struct __acl_entry_t_struct  *acl_entry_t;
typedef struct __acl_permset_t_struct *acl_permset_t;
typedef uint32_t acl_perm_t;
typedef int      acl_type_t;
typedef int      acl_tag_t;

/* -----------------------------------------------------------------------
 * ACL entry iteration constants
 * ----------------------------------------------------------------------- */
#define ACL_FIRST_ENTRY  0
#define ACL_NEXT_ENTRY   1

/* -----------------------------------------------------------------------
 * ACL tag types
 * ----------------------------------------------------------------------- */
#define ACL_UNDEFINED_TAG  (0x00)
#define ACL_USER_OBJ       (0x01)
#define ACL_USER           (0x02)
#define ACL_GROUP_OBJ      (0x04)
#define ACL_GROUP          (0x08)
#define ACL_MASK           (0x10)
#define ACL_OTHER          (0x20)

/* -----------------------------------------------------------------------
 * ACL permission bits
 * ----------------------------------------------------------------------- */
#define ACL_READ    (0x04)
#define ACL_WRITE   (0x02)
#define ACL_EXECUTE (0x01)

/* -----------------------------------------------------------------------
 * ACL types (access vs default)
 * ----------------------------------------------------------------------- */
#define ACL_TYPE_ACCESS  (0x8000)
#define ACL_TYPE_DEFAULT (0x4000)

/* -----------------------------------------------------------------------
 * POSIX ACL API
 * ----------------------------------------------------------------------- */

/* Allocation / deallocation */
acl_t   acl_init(int count);
acl_t   acl_dup(acl_t acl);
int     acl_free(void *obj_p);
acl_t   acl_from_mode(mode_t mode);

/* Get / set ACL from file descriptor or path */
acl_t   acl_get_fd(int fd);
acl_t   acl_get_file(const char *path_p, acl_type_t type);
int     acl_set_fd(int fd, acl_t acl);
int     acl_set_file(const char *path_p, acl_type_t type, acl_t acl);

/* Entry manipulation */
int     acl_create_entry(acl_t *acl_p, acl_entry_t *entry_p);
int     acl_delete_entry(acl_t acl, acl_entry_t entry);
int     acl_get_entry(acl_t acl, int entry_id, acl_entry_t *entry_p);
int     acl_copy_entry(acl_entry_t dest, acl_entry_t src);

/* Tag type */
int     acl_get_tag_type(acl_entry_t entry_d, acl_tag_t *tag_type_p);
int     acl_set_tag_type(acl_entry_t entry_d, acl_tag_t tag_type);

/* Qualifier (uid/gid) */
void   *acl_get_qualifier(acl_entry_t entry_d);
int     acl_set_qualifier(acl_entry_t entry_d, const void *tag_qualifier_p);

/* Permission set */
int     acl_get_permset(acl_entry_t entry_d, acl_permset_t *permset_p);
int     acl_set_permset(acl_entry_t entry_d, acl_permset_t permset);
int     acl_clear_perms(acl_permset_t permset);
int     acl_add_perm(acl_permset_t permset, acl_perm_t perm);
int     acl_delete_perm(acl_permset_t permset, acl_perm_t perm);

/* Validation and mask */
int     acl_calc_mask(acl_t *acl_p);
int     acl_valid(acl_t acl);

/* Text conversion */
acl_t   acl_from_text(const char *buf_p);
char   *acl_to_text(acl_t acl, ssize_t *len_p);

/* Size */
ssize_t acl_size(acl_t acl);

/* Copy to/from external representation */
acl_t   acl_copy_int(const void *buf_p);
ssize_t acl_copy_ext(void *buf_p, acl_t acl, ssize_t size);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_ACL_H */
