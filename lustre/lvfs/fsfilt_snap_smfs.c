/* -*- mode: c; c-basic-offset: 8; indent-tabs-mode: nil; -*-
 * vim:expandtab:shiftwidth=8:tabstop=8:
 *
 *  Lustre filesystem abstraction routines
 *
 *  Copyright (C) 2004 Cluster File Systems, Inc.
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
 */

#define DEBUG_SUBSYSTEM S_FILTER

#include <linux/fs.h>
#include <linux/jbd.h>
#include <linux/slab.h>
#include <linux/pagemap.h>
#include <linux/quotaops.h>
#include <linux/version.h>
#include <linux/kp30.h>
#include <linux/lustre_fsfilt.h>
#include <linux/lustre_smfs.h>
#include <linux/obd.h>
#include <linux/obd_class.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/lustre_snap.h>
#include <linux/lustre_smfs.h>

static struct inode* fsfilt_smfs_create_indirect(struct inode *inode,
                                                 int index,
                                                 unsigned int gen,
                                                 struct inode *parent,
                                                 int del)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        struct inode *cache_parent = NULL;
        struct inode *cache_ind_inode = NULL;
        struct inode *ind_inode = NULL;
        ENTRY;

        if (!snap_fsfilt)
                RETURN(NULL);

        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(NULL);

        cache_parent = I2CI(parent);
        if (!cache_parent)
                RETURN(NULL);

        pre_smfs_inode(inode, cache_inode);
        pre_smfs_inode(inode, cache_parent);

        if (snap_fsfilt->fs_create_indirect)
                cache_ind_inode = snap_fsfilt->fs_create_indirect(cache_inode, 
                                               index, gen, cache_parent, del);
        post_smfs_inode(inode, cache_inode);
        post_smfs_inode(inode, cache_parent);
       #if 0 
        if (cache_ind_inode && !IS_ERR(cache_ind_inode)){ 
                /*FIXME: get indirect inode set_cow flags*/ 
                ind_inode = smfs_get_inode(inode->i_sb, cache_ind_inode->i_ino, 
                                           inode, 0);
        }   
        #endif 
        RETURN(ind_inode);
}

static struct inode*  fsfilt_smfs_get_indirect(struct inode *inode, 
                                               int *table, int slot)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        struct inode *cache_ind_inode = NULL;
        struct inode *ind_inode = NULL;
        ENTRY;

        if (!snap_fsfilt)
                RETURN(NULL);

        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(NULL);

        pre_smfs_inode(inode, cache_inode);

        if (snap_fsfilt->fs_get_indirect)
                cache_ind_inode = snap_fsfilt->fs_get_indirect(cache_inode, 
                                                               table, slot);
        post_smfs_inode(inode, cache_inode);
#if 0      
        if (cache_ind_inode && !IS_ERR(cache_ind_inode)){ 
                /*FIXME: get indirect inode set_cow flags*/ 
                ind_inode = smfs_get_inode(inode->i_sb, cache_ind_inode->i_ino,
                                           inode, slot);
        }    
#endif
        RETURN(ind_inode);
}

static int fsfilt_smfs_set_indirect(struct inode *inode, int index,
                                    ino_t ind_ino, ino_t parent_ino)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        int    rc = -EIO;
        ENTRY;

        if (snap_fsfilt == NULL)
                RETURN(rc);

        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(rc);

        pre_smfs_inode(inode, cache_inode);
        if (snap_fsfilt->fs_set_indirect)
                rc = snap_fsfilt->fs_set_indirect(cache_inode, index,
                                                  ind_ino, parent_ino);
        post_smfs_inode(inode, cache_inode);
        
        RETURN(rc);
}

static int fsfilt_smfs_snap_feature(struct super_block *sb, int feature, 
                                    int op)
{
        struct fsfilt_operations *snap_fsfilt = S2SNAPI(sb)->snap_cache_fsfilt;
        struct super_block       *csb = S2CSB(sb);
        int                      rc = -EIO;
        
        if (snap_fsfilt == NULL)
                RETURN(rc);
        if (!csb)
                RETURN(rc);
        
        if (snap_fsfilt->fs_snap_feature)
                rc = snap_fsfilt->fs_snap_feature(csb, feature, op);  
        
        RETURN(rc); 
}

static int fsfilt_smfs_is_redirector(struct inode *inode)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        int    rc = -EIO;
        ENTRY;

        if (snap_fsfilt == NULL)
                RETURN(rc);

        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(rc);

        pre_smfs_inode(inode, cache_inode);
        if (snap_fsfilt->fs_is_redirector)
                rc = snap_fsfilt->fs_is_redirector(cache_inode);
        post_smfs_inode(inode, cache_inode);
        
        RETURN(rc);
}
static int fsfilt_smfs_is_indirect(struct inode *inode)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        int    rc = -EIO;
        ENTRY;

        if (snap_fsfilt == NULL)
                RETURN(rc);

        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(rc);

        pre_smfs_inode(inode, cache_inode);
        if (snap_fsfilt->fs_is_indirect)
                rc = snap_fsfilt->fs_is_indirect(cache_inode);
        post_smfs_inode(inode, cache_inode);
        
        RETURN(rc);
}
static ino_t fsfilt_smfs_get_indirect_ino(struct super_block *sb, ino_t ino, 
                                          int index)
{
        struct fsfilt_operations *snap_fsfilt = S2SNAPI(sb)->snap_cache_fsfilt;
        struct super_block       *csb = S2CSB(sb);
        int    rc = -EIO;
        ENTRY;

        if (snap_fsfilt == NULL)
                RETURN(rc);

        if (snap_fsfilt->fs_get_indirect_ino)
                rc = snap_fsfilt->fs_get_indirect_ino(csb, ino, index);
         
        RETURN(rc);
}

static int fsfilt_smfs_destroy_indirect(struct inode *inode, int index,
                                        struct inode *next_ind)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        struct inode *cache_next = NULL;
        int    rc = -EIO;
        ENTRY;

        if (snap_fsfilt == NULL)
                RETURN(rc);

        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(rc);

        cache_next = I2CI(next_ind);
        if (!cache_next)
                RETURN(rc);

        pre_smfs_inode(inode, cache_inode);
        pre_smfs_inode(next_ind, cache_next);

        if (snap_fsfilt->fs_destroy_indirect)
                rc = snap_fsfilt->fs_destroy_indirect(cache_inode, index, 
                                                      cache_next);
        post_smfs_inode(inode, cache_inode);
        post_smfs_inode(next_ind, cache_next);
        
        RETURN(rc);
}
static int fsfilt_smfs_restore_indirect(struct inode *inode, int index)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        int    rc = -EIO;
        ENTRY;

        if (snap_fsfilt == NULL)
                RETURN(rc);

        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(rc);

        pre_smfs_inode(inode, cache_inode);
        
        if (snap_fsfilt->fs_restore_indirect)
                rc = snap_fsfilt->fs_restore_indirect(cache_inode, index);
        
        post_smfs_inode(inode, cache_inode);
        
        RETURN(rc);
}
static int fsfilt_smfs_iterate(struct super_block *sb,
                               int (*repeat)(struct inode *inode, void *priv),
                               struct inode **start, void *priv, int flag)
{
        struct fsfilt_operations *snap_fsfilt = S2SNAPI(sb)->snap_cache_fsfilt;
        struct super_block       *csb = S2CSB(sb);
        int                      rc = -EIO;
        ENTRY;

        /*FIXME start == NULL, later*/
        LASSERT(start == NULL);
 
        if (snap_fsfilt == NULL)
                RETURN(rc);
         
        if (snap_fsfilt->fs_iterate)
                rc = snap_fsfilt->fs_iterate(csb, repeat, start, priv, flag);

        RETURN(rc);
}

static int fsfilt_smfs_copy_block(struct inode *dst, struct inode *src, int blk)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(dst);
        struct inode *cache_dst = NULL;
        struct inode *cache_src = NULL;
        int    rc = -EIO;
        ENTRY;

        if (snap_fsfilt == NULL)
                RETURN(rc);
         
        cache_dst = I2CI(dst);
        if (!cache_dst)
                RETURN(rc);
       
        cache_src = I2CI(src);
        if (!cache_src)
                RETURN(rc);

        pre_smfs_inode(dst, cache_dst);
        pre_smfs_inode(src, cache_src);
        
        if (snap_fsfilt->fs_copy_block)
                rc = snap_fsfilt->fs_copy_block(cache_dst, cache_src, blk);

        post_smfs_inode(dst, cache_dst);
        post_smfs_inode(src, cache_src);
         
        RETURN(rc); 
}

static int fsfilt_smfs_set_snap_info(struct inode *inode, 
                                     void* key, __u32 keylen, void *val, 
                                     __u32 *vallen)
{

        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        int                      rc = -EIO;
                
        if (snap_fsfilt == NULL)
                RETURN(rc);
        
        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(rc);

        pre_smfs_inode(inode, cache_inode);
        
        if (snap_fsfilt->fs_set_snap_info)
                rc = snap_fsfilt->fs_set_snap_info(cache_inode, key, 
                                                   keylen, val, vallen);
        post_smfs_inode(inode, cache_inode);
        
        RETURN(rc);
}

static int fsfilt_smfs_get_snap_info(struct inode *inode, void *key, 
                                     __u32 keylen, void *val, __u32 *vallen)
{
        struct fsfilt_operations *snap_fsfilt = I2SNAPCOPS(inode);
        struct inode *cache_inode = NULL;
        int                      rc = -EIO;
        
        if (snap_fsfilt == NULL)
                RETURN(rc);
        
        cache_inode = I2CI(inode);
        if (!cache_inode)
                RETURN(rc);
        
        if (snap_fsfilt->fs_get_snap_info)
                rc = snap_fsfilt->fs_get_snap_info(cache_inode, key, 
                                                   keylen, val, vallen);
        
        RETURN(rc);
}

static int fsfilt_smfs_read_dotsnap_dir_page(struct file *file, char *buf,
                                             size_t count, loff_t *off)
{
#if 0
        struct inode *inode = file->f_dentry->d_inode;
        struct fsfilt_operations *snap_cops = I2SNAPCOPS(inode);
        int    i = 0, size = 0, off_count = 0, buf_off = 0, rc = 0;
        ENTRY;
        /*Get the offset of dir ent*/
        //struct snap_table *stbl = S2SNAPI(inode->i_sb)->sni_table;
        while (size < *off && off_count < stbl->sntbl_count) {
                char *name = stbl->sntbl_items[i].sn_name;
                size +=snap_cops->fs_dir_ent_size(name);
                off_count ++;
        }
        for (i = off_count; i < stbl->sntbl_count; i++) {
                char *name = stbl->sntbl_items[i].sn_name;
                rc = snap_cops->fs_set_dir_ent(inode->i_sb, name, buf, buf_off, 
                                               rc, count);
                if (rc < 0)
                        break;

                buf_off += rc;

                if (buf_off >= count) 
                        break;
        }
        if (rc > 0) 
                rc = 0; 
#else
#warning "still not implement read .snap dir page for fsfilt Wangdi"
#endif
        RETURN(0); 
}

struct fsfilt_operations fsfilt_smfs_snap_ops = {
        .fs_type                = "smfs_snap",
        .fs_owner               = THIS_MODULE,
        .fs_create_indirect     = fsfilt_smfs_create_indirect,
        .fs_get_indirect        = fsfilt_smfs_get_indirect,
        .fs_set_indirect        = fsfilt_smfs_set_indirect,
	.fs_snap_feature	= fsfilt_smfs_snap_feature,
	.fs_is_redirector	= fsfilt_smfs_is_redirector,
	.fs_is_indirect		= fsfilt_smfs_is_indirect,
        .fs_get_indirect_ino    = fsfilt_smfs_get_indirect_ino,
        .fs_destroy_indirect    = fsfilt_smfs_destroy_indirect,
        .fs_restore_indirect    = fsfilt_smfs_restore_indirect,
        .fs_iterate             = fsfilt_smfs_iterate,
        .fs_copy_block          = fsfilt_smfs_copy_block,
        .fs_set_snap_info       = fsfilt_smfs_set_snap_info,
        .fs_get_snap_info       = fsfilt_smfs_get_snap_info,
        .fs_read_dotsnap_dir_page = fsfilt_smfs_read_dotsnap_dir_page,
};


static int __init fsfilt_smfs_snap_init(void)
{
        int rc;

        rc = fsfilt_register_ops(&fsfilt_smfs_snap_ops);
        return rc;
}

static void __exit fsfilt_smfs_snap_exit(void)
{
        fsfilt_unregister_ops(&fsfilt_smfs_snap_ops);
}

module_init(fsfilt_smfs_snap_init);
module_exit(fsfilt_smfs_snap_exit);

MODULE_AUTHOR("Cluster File Systems, Inc. <info@clusterfs.com>");
MODULE_DESCRIPTION("Lustre SMFS SNAP Filesystem Helper v0.1");
MODULE_LICENSE("GPL");
