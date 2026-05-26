/* SPDX-License-Identifier: LGPL-2.1+
 * Android no-op stub implementation of POSIX ACL functions.
 *
 * Android's tmpfs does not support CONFIG_TMPFS_XATTR so POSIX ACL
 * operations cannot be applied to /dev/shm.  All functions return
 * success (or a harmless null/zero) so that the SCORE communication
 * library compiles and links without modification.
 *
 * acl_set_fd() silently succeeds — running as root (UID 0) provides
 * sufficient shared-memory access without ACL enforcement.
 */

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/acl.h>
#include <acl/libacl.h>

/* -----------------------------------------------------------------------
 * Allocation / deallocation
 * ----------------------------------------------------------------------- */
acl_t acl_init(int count)       { (void)count; return NULL; }
acl_t acl_dup(acl_t acl)        { (void)acl;   return NULL; }
int   acl_free(void *obj_p)     { (void)obj_p; return 0; }
acl_t acl_from_mode(mode_t mode){ (void)mode;  return NULL; }

/* -----------------------------------------------------------------------
 * Get / set ACL from file descriptor or path
 * ----------------------------------------------------------------------- */
acl_t acl_get_fd(int fd)
{
    (void)fd;
    return NULL; /* empty ACL — constructor treats NULL as "no ACL" */
}

acl_t acl_get_file(const char *path_p, acl_type_t type)
{
    (void)path_p; (void)type;
    return NULL;
}

int acl_set_fd(int fd, acl_t acl)
{
    (void)fd; (void)acl;
    return 0; /* silently succeed */
}

int acl_set_file(const char *path_p, acl_type_t type, acl_t acl)
{
    (void)path_p; (void)type; (void)acl;
    return 0;
}

/* -----------------------------------------------------------------------
 * Entry manipulation
 * ----------------------------------------------------------------------- */
int acl_create_entry(acl_t *acl_p, acl_entry_t *entry_p)
{
    (void)acl_p;
    if (entry_p) *entry_p = NULL;
    return 0;
}

int acl_delete_entry(acl_t acl, acl_entry_t entry)
{
    (void)acl; (void)entry;
    return 0;
}

int acl_get_entry(acl_t acl, int entry_id, acl_entry_t *entry_p)
{
    (void)acl; (void)entry_id;
    if (entry_p) *entry_p = NULL;
    return 0; /* 0 = no more entries */
}

int acl_copy_entry(acl_entry_t dest, acl_entry_t src)
{
    (void)dest; (void)src;
    return 0;
}

/* -----------------------------------------------------------------------
 * Tag type
 * ----------------------------------------------------------------------- */
int acl_get_tag_type(acl_entry_t entry_d, acl_tag_t *tag_type_p)
{
    (void)entry_d;
    if (tag_type_p) *tag_type_p = ACL_UNDEFINED_TAG;
    errno = EINVAL;
    return -1;
}

int acl_set_tag_type(acl_entry_t entry_d, acl_tag_t tag_type)
{
    (void)entry_d; (void)tag_type;
    return 0;
}

/* -----------------------------------------------------------------------
 * Qualifier (uid/gid)
 * ----------------------------------------------------------------------- */
void *acl_get_qualifier(acl_entry_t entry_d)
{
    (void)entry_d;
    errno = EINVAL;
    return NULL;
}

int acl_set_qualifier(acl_entry_t entry_d, const void *tag_qualifier_p)
{
    (void)entry_d; (void)tag_qualifier_p;
    return 0;
}

/* -----------------------------------------------------------------------
 * Permission set
 * ----------------------------------------------------------------------- */
int acl_get_permset(acl_entry_t entry_d, acl_permset_t *permset_p)
{
    (void)entry_d;
    if (permset_p) *permset_p = NULL;
    return 0;
}

int acl_set_permset(acl_entry_t entry_d, acl_permset_t permset)
{
    (void)entry_d; (void)permset;
    return 0;
}

int acl_clear_perms(acl_permset_t permset)
{
    (void)permset;
    return 0;
}

int acl_add_perm(acl_permset_t permset, acl_perm_t perm)
{
    (void)permset; (void)perm;
    return 0;
}

int acl_delete_perm(acl_permset_t permset, acl_perm_t perm)
{
    (void)permset; (void)perm;
    return 0;
}

/* -----------------------------------------------------------------------
 * Validation and mask
 * ----------------------------------------------------------------------- */
int acl_calc_mask(acl_t *acl_p)
{
    (void)acl_p;
    return 0;
}

int acl_valid(acl_t acl)
{
    (void)acl;
    return 0;
}

/* -----------------------------------------------------------------------
 * Text conversion
 * ----------------------------------------------------------------------- */
acl_t acl_from_text(const char *buf_p)
{
    (void)buf_p;
    return NULL;
}

char *acl_to_text(acl_t acl, ssize_t *len_p)
{
    (void)acl;
    if (len_p) *len_p = 0;
    errno = ENOTSUP;
    return NULL;
}

/* -----------------------------------------------------------------------
 * Size / copy
 * ----------------------------------------------------------------------- */
ssize_t acl_size(acl_t acl)
{
    (void)acl;
    return 0;
}

acl_t acl_copy_int(const void *buf_p)
{
    (void)buf_p;
    return NULL;
}

ssize_t acl_copy_ext(void *buf_p, acl_t acl, ssize_t size)
{
    (void)buf_p; (void)acl; (void)size;
    return 0;
}

/* -----------------------------------------------------------------------
 * libacl extensions (acl/libacl.h)
 * ----------------------------------------------------------------------- */
int acl_get_perm(acl_permset_t permset, acl_perm_t perm)
{
    (void)permset; (void)perm;
    return 0;
}

int acl_extended_fd(int fd)
{
    (void)fd;
    return 0;
}

int acl_extended_file(const char *path_p)
{
    (void)path_p;
    return 0;
}

int acl_extended_file_nofollow(const char *path_p)
{
    (void)path_p;
    return 0;
}

int acl_equiv_mode(acl_t acl, mode_t *mode_p)
{
    (void)acl;
    if (mode_p) *mode_p = 0;
    return 0;
}

/* non-portable extension used by acl_impl.cpp */
int acl_get_perm_np(acl_permset_t permset, acl_perm_t perm)
{
    (void)permset; (void)perm;
    return 0;
}
