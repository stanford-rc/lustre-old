/* -*- mode: c; c-basic-offset: 8; indent-tabs-mode: nil; -*-
 * vim:expandtab:shiftwidth=8:tabstop=8:
 *
 *  Copyright (C) 2001, 2002 Cluster File Systems, Inc.
 *
 *   This file is part of Lustre, http://www.lustre.org.
 *
 *   Lustre is free software; you can redistribute it and/or
 *   modify it under the terms of version 2 of the GNU General Public
 *   License as published by the Free Software Foundation.
 *
 *   Lustre is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Lustre; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _OBD_SUPPORT
#define _OBD_SUPPORT

#ifdef __KERNEL__
#include <linux/config.h>
#include <linux/autoconf.h>
#include <linux/slab.h>
#include <linux/highmem.h>
#else

#endif
#include <linux/kp30.h>

/* global variables */
extern atomic_t obd_memory;
extern int obd_memmax;
extern unsigned int obd_fail_loc;
extern unsigned int obd_timeout;
extern unsigned long obd_max_dirty_pages;
extern char obd_lustre_upcall[128];
extern unsigned int obd_sync_filter;

#define OBD_FAIL_MDS                     0x100
#define OBD_FAIL_MDS_HANDLE_UNPACK       0x101
#define OBD_FAIL_MDS_GETATTR_NET         0x102
#define OBD_FAIL_MDS_GETATTR_PACK        0x103
#define OBD_FAIL_MDS_READPAGE_NET        0x104
#define OBD_FAIL_MDS_READPAGE_PACK       0x105
#define OBD_FAIL_MDS_SENDPAGE            0x106
#define OBD_FAIL_MDS_REINT_NET           0x107
#define OBD_FAIL_MDS_REINT_UNPACK        0x108
#define OBD_FAIL_MDS_REINT_SETATTR       0x109
#define OBD_FAIL_MDS_REINT_SETATTR_WRITE 0x10a
#define OBD_FAIL_MDS_REINT_CREATE        0x10b
#define OBD_FAIL_MDS_REINT_CREATE_WRITE  0x10c
#define OBD_FAIL_MDS_REINT_UNLINK        0x10d
#define OBD_FAIL_MDS_REINT_UNLINK_WRITE  0x10e
#define OBD_FAIL_MDS_REINT_LINK          0x10f
#define OBD_FAIL_MDS_REINT_LINK_WRITE    0x110
#define OBD_FAIL_MDS_REINT_RENAME        0x111
#define OBD_FAIL_MDS_REINT_RENAME_WRITE  0x112
#define OBD_FAIL_MDS_OPEN_NET            0x113
#define OBD_FAIL_MDS_OPEN_PACK           0x114
#define OBD_FAIL_MDS_CLOSE_NET           0x115
#define OBD_FAIL_MDS_CLOSE_PACK          0x116
#define OBD_FAIL_MDS_CONNECT_NET         0x117
#define OBD_FAIL_MDS_CONNECT_PACK        0x118
#define OBD_FAIL_MDS_REINT_NET_REP       0x119
#define OBD_FAIL_MDS_DISCONNECT_NET      0x11a
#define OBD_FAIL_MDS_GETSTATUS_NET       0x11b
#define OBD_FAIL_MDS_GETSTATUS_PACK      0x11c
#define OBD_FAIL_MDS_STATFS_PACK         0x11d
#define OBD_FAIL_MDS_STATFS_NET          0x11e
#define OBD_FAIL_MDS_GETATTR_NAME_NET    0x11f
#define OBD_FAIL_MDS_PIN_NET             0x120
#define OBD_FAIL_MDS_UNPIN_NET           0x121
#define OBD_FAIL_MDS_ALL_REPLY_NET       0x122
#define OBD_FAIL_MDS_ALL_REQUEST_NET     0x123

#define OBD_FAIL_OST                     0x200
#define OBD_FAIL_OST_CONNECT_NET         0x201
#define OBD_FAIL_OST_DISCONNECT_NET      0x202
#define OBD_FAIL_OST_GET_INFO_NET        0x203
#define OBD_FAIL_OST_CREATE_NET          0x204
#define OBD_FAIL_OST_DESTROY_NET         0x205
#define OBD_FAIL_OST_GETATTR_NET         0x206
#define OBD_FAIL_OST_SETATTR_NET         0x207
#define OBD_FAIL_OST_OPEN_NET            0x208
#define OBD_FAIL_OST_CLOSE_NET           0x209
#define OBD_FAIL_OST_BRW_NET             0x20a
#define OBD_FAIL_OST_PUNCH_NET           0x20b
#define OBD_FAIL_OST_STATFS_NET          0x20c
#define OBD_FAIL_OST_HANDLE_UNPACK       0x20d
#define OBD_FAIL_OST_BRW_WRITE_BULK      0x20e
#define OBD_FAIL_OST_BRW_READ_BULK       0x20f
#define OBD_FAIL_OST_SYNCFS_NET          0x210
#define OBD_FAIL_OST_ALL_REPLY_NET       0x211
#define OBD_FAIL_OST_ALL_REQUESTS_NET    0x212
#define OBD_FAIL_OST_LDLM_REPLY_NET      0x213
#define OBD_FAIL_OST_BRW_PAUSE_BULK      0x214

#define OBD_FAIL_LDLM                    0x300
#define OBD_FAIL_LDLM_NAMESPACE_NEW      0x301
#define OBD_FAIL_LDLM_ENQUEUE            0x302
#define OBD_FAIL_LDLM_CONVERT            0x303
#define OBD_FAIL_LDLM_CANCEL             0x304
#define OBD_FAIL_LDLM_BL_CALLBACK        0x305
#define OBD_FAIL_LDLM_CP_CALLBACK        0x306

#define OBD_FAIL_OSC                     0x400
#define OBD_FAIL_OSC_BRW_READ_BULK       0x401
#define OBD_FAIL_OSC_BRW_WRITE_BULK      0x402
#define OBD_FAIL_OSC_LOCK_BL_AST         0x403
#define OBD_FAIL_OSC_LOCK_CP_AST         0x404

#define OBD_FAIL_PTLRPC                  0x500
#define OBD_FAIL_PTLRPC_ACK              0x501

#define OBD_FAIL_OBD_PING_NET            0x600
#define OBD_FAIL_OBD_LOG_CANCEL_NET      0x601

/* preparation for a more advanced failure testbed (not functional yet) */
#define OBD_FAIL_MASK_SYS    0x0000FF00
#define OBD_FAIL_MASK_LOC    (0x000000FF | OBD_FAIL_MASK_SYS)
#define OBD_FAIL_ONCE        0x80000000
#define OBD_FAILED           0x40000000
#define OBD_FAIL_MDS_ALL_NET 0x01000000
#define OBD_FAIL_OST_ALL_NET 0x02000000

#define OBD_FAIL_CHECK(id)   (((obd_fail_loc & OBD_FAIL_MASK_LOC) ==           \
                              ((id) & OBD_FAIL_MASK_LOC)) &&                   \
                              ((obd_fail_loc & (OBD_FAILED | OBD_FAIL_ONCE))!= \
                                (OBD_FAILED | OBD_FAIL_ONCE)))

#define OBD_FAIL_CHECK_ONCE(id)                                              \
({      int _ret_ = 0;                                                       \
        if (OBD_FAIL_CHECK(id)) {                                            \
                CERROR("obd_fail_loc=%x\n", id);                             \
                obd_fail_loc |= OBD_FAILED;                                  \
                if ((id) & OBD_FAIL_ONCE)                                    \
                        obd_fail_loc |= OBD_FAIL_ONCE;                       \
                _ret_ = 1;                                                   \
        }                                                                    \
        _ret_;                                                               \
})

#define OBD_FAIL_RETURN(id, ret)                                             \
do {                                                                         \
        if (OBD_FAIL_CHECK_ONCE(id)) {                                       \
                RETURN(ret);                                                 \
        }                                                                    \
} while(0)

#define OBD_FAIL_TIMEOUT(id, secs)                                           \
do {                                                                         \
        if  (OBD_FAIL_CHECK_ONCE(id)) {                                      \
                CERROR("obd_fail_timeout id %x sleeping for %d secs\n",     \
                       (id), (secs));                                        \
                set_current_state(TASK_UNINTERRUPTIBLE);                     \
                schedule_timeout((secs) * HZ);                               \
                set_current_state(TASK_RUNNING);                             \
                CERROR("obd_fail_timeout id %x awake\n", (id));              \
       }                                                                     \
} while(0)

#define fixme() CDEBUG(D_OTHER, "FIXME\n");

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/blkdev.h>


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0))
#define BDEVNAME_DECLARE_STORAGE(foo) char foo[BDEVNAME_SIZE]
#define ll_bdevname(DEV, STORAGE) __bdevname(DEV, STORAGE)
#define ll_lock_kernel lock_kernel()
#else
#define BDEVNAME_DECLARE_STORAGE(foo) char __unused_##foo
#define ll_bdevname(DEV, STORAGE) ((void)__unused_##STORAGE, bdevname((DEV)))
#define ll_lock_kernel
#endif

void dev_set_rdonly(kdev_t dev, int no_write);
void dev_clear_rdonly(int);

static inline void OBD_FAIL_WRITE(int id, kdev_t dev)
{
        if (OBD_FAIL_CHECK(id)) {
                BDEVNAME_DECLARE_STORAGE(tmp);
#ifdef CONFIG_DEV_RDONLY
                CERROR("obd_fail_loc=%x, fail write operation on %s\n",
                       id, ll_bdevname(kdev_t_to_nr(dev), tmp));
                dev_set_rdonly(dev, 2);
#else
                CERROR("obd_fail_loc=%x, can't fail write operation on %s\n",
                       id, ll_bdevname(kdev_t_to_nr(dev), tmp));
#endif
                /* We set FAIL_ONCE because we never "un-fail" a device */
                obd_fail_loc |= OBD_FAILED | OBD_FAIL_ONCE;
        }
}
#else /* !__KERNEL__ */
#define LTIME_S(time) (time)
#endif  /* __KERNEL__ */

#define OBD_ALLOC_GFP(ptr, size, gfp_mask)                                    \
do {                                                                          \
        (ptr) = kmalloc(size, gfp_mask);                                      \
        if ((ptr) == NULL) {                                                  \
                CERROR("kmalloc of '" #ptr "' (%d bytes) failed at %s:%d\n",  \
                       (int)(size), __FILE__, __LINE__);                      \
        } else {                                                              \
                memset(ptr, 0, size);                                         \
                atomic_add(size, &obd_memory);                                \
                if (atomic_read(&obd_memory) > obd_memmax)                    \
                        obd_memmax = atomic_read(&obd_memory);                \
                CDEBUG(D_MALLOC, "kmalloced '" #ptr "': %d at %p (tot %d)\n", \
                       (int)(size), ptr, atomic_read(&obd_memory));           \
        }                                                                     \
} while (0)

#ifndef OBD_GFP_MASK
# define OBD_GFP_MASK GFP_KERNEL
#endif

#define OBD_ALLOC(ptr, size) OBD_ALLOC_GFP(ptr, size, OBD_GFP_MASK)

#ifdef __arch_um__
# define OBD_VMALLOC(ptr, size) OBD_ALLOC(ptr, size)
#else
# define OBD_VMALLOC(ptr, size)                                               \
do {                                                                          \
        (ptr) = vmalloc(size);                                                \
        if ((ptr) == NULL) {                                                  \
                CERROR("vmalloc of '" #ptr "' (%d bytes) failed at %s:%d\n",  \
                       (int)(size), __FILE__, __LINE__);                      \
        } else {                                                              \
                memset(ptr, 0, size);                                         \
                atomic_add(size, &obd_memory);                                \
                if (atomic_read(&obd_memory) > obd_memmax)                    \
                        obd_memmax = atomic_read(&obd_memory);                \
                CDEBUG(D_MALLOC, "vmalloced '" #ptr "': %d at %p (tot %d)\n", \
                       (int)(size), ptr, atomic_read(&obd_memory));           \
        }                                                                     \
} while (0)
#endif

#ifdef CONFIG_DEBUG_SLAB
#define POISON(ptr, c, s) do {} while (0)
#else
#define POISON(ptr, c, s) memset(ptr, c, s)
#endif

#define OBD_FREE(ptr, size)                                                   \
do {                                                                          \
        LASSERT(ptr);                                                         \
        atomic_sub(size, &obd_memory);                                        \
        CDEBUG(D_MALLOC, "kfreed '" #ptr "': %d at %p (tot %d).\n",           \
               (int)(size), ptr, atomic_read(&obd_memory));                   \
        POISON(ptr, 0x5a, size);                                              \
        kfree(ptr);                                                           \
        (ptr) = (void *)0xdeadbeef;                                           \
} while (0)

#ifdef __arch_um__
# define OBD_VFREE(ptr, size) OBD_FREE(ptr, size)
#else
# define OBD_VFREE(ptr, size)                                                 \
do {                                                                          \
        LASSERT(ptr);                                                         \
        atomic_sub(size, &obd_memory);                                        \
        CDEBUG(D_MALLOC, "vfreed '" #ptr "': %d at %p (tot %d).\n",           \
               (int)(size), ptr, atomic_read(&obd_memory));                   \
        POISON(ptr, 0x5a, size);                                              \
        vfree(ptr);                                                           \
        (ptr) = (void *)0xdeadbeef;                                           \
} while (0)
#endif

/* we memset() the slab object to 0 when allocation succeeds, so DO NOT
 * HAVE A CTOR THAT DOES ANYTHING.  its work will be cleared here.  we'd
 * love to assert on that, but slab.c keeps kmem_cache_s all to itself. */
#define OBD_SLAB_ALLOC(ptr, slab, type, size)                                 \
do {                                                                          \
        LASSERT(!in_interrupt());                                             \
        (ptr) = kmem_cache_alloc(slab, type);                                 \
        if ((ptr) == NULL) {                                                  \
                CERROR("slab-alloc of '"#ptr"' (%d bytes) failed at %s:%d\n", \
                       (int)(size), __FILE__, __LINE__);                      \
        } else {                                                              \
                memset(ptr, 0, size);                                         \
                atomic_add(size, &obd_memory);                                \
                if (atomic_read(&obd_memory) > obd_memmax)                    \
                        obd_memmax = atomic_read(&obd_memory);                \
                CDEBUG(D_MALLOC, "slab-alloced '"#ptr"': %d at %p (tot %d)\n",\
                       (int)(size), ptr, atomic_read(&obd_memory));           \
        }                                                                     \
} while (0)

#define OBD_SLAB_FREE(ptr, slab, size)                                        \
do {                                                                          \
        LASSERT(ptr);                                                         \
        CDEBUG(D_MALLOC, "slab-freed '" #ptr "': %d at %p (tot %d).\n",       \
               (int)(size), ptr, atomic_read(&obd_memory));                   \
        atomic_sub(size, &obd_memory);                                        \
        POISON(ptr, 0x5a, size);                                              \
        kmem_cache_free(slab, ptr);                                           \
        (ptr) = (void *)0xdeadbeef;                                           \
} while (0)

#endif
