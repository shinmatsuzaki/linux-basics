/**********************************************************************************
 *
 * file             : me2fs_super.c
 * Descriptions     : super block operations for my ext2 file system
 *
 **********************************************************************************/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>

#include "me2fs.h"
#include "me2fs_util.h"
#include "me2fs_super.h"

/*
====================================================================================
  Prototype Statement
====================================================================================
*/
/*
---------------------------------------------------------------------------------
  For Super Block Operations
---------------------------------------------------------------------------------
*/
static int me2fsFillSuperBlock( struct super_block *sb,
                                void   *data,
				int    silent );
static void me2fsPutSuperBlock( struct super_block *sb );

/*
====================================================================================
  Helper Functions
====================================================================================
*/
static void dbgPrintExt2SB( struct ext2_super_block *esb ); 

/*
====================================================================================
  DEFINES
====================================================================================
*/

/*
====================================================================================
  Management
====================================================================================
*/
/*
---------------------------------------------------------------------------------
  Super Block Operations
---------------------------------------------------------------------------------
*/
static int me2fs_write_inode( struct inode* inode, struct writeback_control *wbc )
{ DBGPRINT( "<ME2FS>write_inode\n" ); return 0; }
static void me2fs_destroy_inode( struct inode *inode )
{ DBGPRINT( "<ME2FS>destroy_inode\n" ); }
//static int me2fs_drop_inode( struct inode *inode )
//{ DBGPRINT( "<ME2FS>drop_inode\n" ); return 0; }
static void me2fs_evict_inode( struct inode *inode )
{ DBGPRINT( "<ME2FS>evict_inode\n" ); }
static int me2fs_sync_fs( struct super_block *sb, int wait )
{ DBGPRINT( "<ME2FS>sync_fs\n" ); return 0; }
static int me2fs_freeze_fs( struct super_block *sb )
{ DBGPRINT( "<ME2FS>freeze_fs\n" ); return 0; }
static int me2fs_unfreeze_fs( struct super_block *sb )
{ DBGPRINT( "<ME2FS>unfreeze_fs\n" ); return 0; }
static int me2fs_statfs( struct dentry *dentry, struct kstatfs *buf )
{ DBGPRINT( "<ME2FS>unfreeze_fs\n" ); return 0; }
static int me2fs_remount_fs( struct super_block *sb, int *len, char *buf )
{ DBGPRINT( "<ME2FS>remount_fs\n" ); return 0; }
static int me2fs_show_options( struct seq_file *seq_file, struct dentry *dentry )
{ DBGPRINT( "<ME2FS>show_options\n" ); return 0; }


static struct super_operations me2fs_super_ops = {
	.destroy_inode = me2fs_destroy_inode,
//	.dirty_inode = me2fs_dirty_inode,
	.write_inode = me2fs_write_inode,
//	.drop_inode = me2fs_drop_inode,
//	.evict_inode = me2fs_evict_inode,
	.put_super = me2fsPutSuperBlock,
	.sync_fs = me2fs_sync_fs,
	.freeze_fs = me2fs_freeze_fs,
	.unfreeze_fs = me2fs_unfreeze_fs,
	.statfs = me2fs_statfs,
	.remount_fs = me2fs_remount_fs,
//	.umount_begin = me2fs_umount_begin,
	.show_options = me2fs_show_options,
};

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  < Open Functions >
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/*
 * _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_

   Function    :me2fsMountBlockDev
   Description :mount me2fs over block device
 * _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_
 */

struct dentry*
me2fsMountBlockDev( struct     file_system_type *fs_type, // file system type
                    int        flags,                     // mount flags
		    const char *dev_name,                 // device name
		    void       *data )                    // user data
{
   //debug
   DBGPRINT("<DEBUG>me2fsMountBlockDev on me2fs_super.c called");

   return( mount_bdev( fs_type, flags, dev_name, data, me2fsFillSuperBlock ) );
}

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  < Local functions >
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/* 
====================================================================================
  Function     :me2fsFillSuperBlock
  Description  :fill my ext2 super block object 
====================================================================================
*/
static int me2fsFillSuperBlock( struct super_block *sb,  // vfs super block *sb
                                void   *data,            // user data
				int    silent )          // silent flag
{
  struct buffer_head             *bh; 
  struct ext2_super_block        *esb;  //implementation of struct super block, defiend in me2fs.h
  struct me2fs_sb_info           *msi;  //super block admin info, defined in me2fs.h
  struct inode                   *root;
  int                            block_size;
  int                            ret = -EINVAL;
  //unsigned long                  sb_block = 1;


  //debug
  DBGPRINT( "me2fsFillSuperBlock called from mount_bdev");
  DBGPRINT( "<DEBUG>sb->smagic: %lu", sb->s_magic);


  /* -------------------------------------------------------------------------------*/
  /* allocate memory to me2fw_sb_info                                               */ 
  /* -------------------------------------------------------------------------------*/
  msi = kzalloc( sizeof( struct me2fs_sb_info), GFP_KERNEL); //動的メモリ割り当て

  if( !msi ) //error handler for kzalloc
  {
    DBGPRINT( "<ME2FS>error: unable to alloc me2fs_sb_info\n" );
    ret = -ENOMEM;
    return( ret );
  }

  /* -------------------------------------------------------------------------------*/
  /* set device's block size and size bits to super block                           */ 
  /* -------------------------------------------------------------------------------*/
  block_size = sb_min_blocksize( sb, BLOCK_SIZE );

  DBGPRINT( "<ME2FS>Fill Super! block_size = %d\n", block_size );
  DBGPRINT( "<ME2FS>s_blocksize_bits = %d\n", sb->s_blocksize_bits );
  DBGPRINT( "<ME2FS>default block size is = %d\n", BLOCK_SIZE );
  DBGPRINT( "<ME2FS>device is : %s\n", sb->s_id );

  if( !block_size )
  {
    DBGPRINT( "<ME2FS>error: unable to set blocksize\n" );
    goto error_read_sb;
  }

  if( !( bh = sb_bread( sb, 1 ) ) ) //read super block info from block device to buffer cache and return buffer head
  {
    DBGPRINT( "<ME2FS>failed to bread super block\n" );
    goto error_read_sb;
  }

  //debug
  DBGPRINT( "<DEBUG> struct buffer_head ->b_state: %lu\n", bh->b_state);
  DBGPRINT( "<DEBUG>esb: %p", esb);

  esb = ( struct ext2_super_block* )( bh->b_data); // b_data is pointer to data addr within the page

  //debug
  DBGPRINT( "<DEBUG>esb: %p", esb);
  DBGPRINT( "<DEBUG>esb->s_magic: %d\n", esb->s_magic);
  dbgPrintExt2SB( esb );

  sb->s_magic = le16_to_cpu( esb->s_magic );

  if( sb->s_magic != ME2FS_SUPER_MAGIC )
  {
    DBGPRINT( "<ME2FS>error : magic of super block is %lu\n", sb->s_magic );
    goto error_mount;
  }

  msi->s_esb = esb;  //msi = me2fs_sb_info , esb = ext2_super_block
  msi->s_sbh = bh;   //bh  = buffer_head

  //dbgPrintExt2SB( esb );
  /* -------------------------------------------------------------------------------*/
  /* set up vfs super block                                                         */
  /* -------------------------------------------------------------------------------*/
  sb->s_op		= &me2fs_super_ops; // register super block operation to struct super block on vfs

  DBGPRINT( "<ME2FS>max file size = %lu\n", ( unsigned long )sb->s_maxbytes );

  sb->s_fs_info = ( void* )msi; // set pointer from super block object(vfs side) to super block info(filesystem side)

  root = iget_locked( sb, ME2FS_EXT2_ROOT_INO );

  if( IS_ERR( root ) )
  {
    DBGPRINT( "<ME2FS>error : failed to get root inode\n" );
    ret = PTR_ERR( root );
    goto error_mount;
  }

  unlock_new_inode( root );
  inc_nlink( root );
  
  root->i_mode = S_IFDIR;

  if( !S_ISDIR( root->i_mode ) )
  {
      DBGPRINT( "<ME2FS>root is not directory!!!!\n" );
  }

  sb->s_root = d_make_root( root );

  if( !sb->s_root )
  {
    DBGPRINT( "<ME2FS>error : failed to make root\n" );
    ret = -ENOMEM;
    goto error_mount;
  }

  le16_add_cpu( &esb->s_mnt_count, 1 );

  DBGPRINT( "<ME2FS> me2fs is mounted !\n" );

  return( 0 );

  /* ------------------------------------------------------------------------ */
  /* release buffer cache for read super block                                */
  /* ------------------------------------------------------------------------ */
error_mount:
  brelse( bh );


  /* ------------------------------------------------------------------------ */
  /* release me2fs super block information                                    */
  /* ------------------------------------------------------------------------ */
error_read_sb:
  sb->s_fs_info = NULL;
  kfree( msi );

  return( ret );

}

/*
==================================================================================
	Function	:me2fsPutSuperBlock
	Description	:put super block
==================================================================================
*/

static void me2fsPutSuperBlock( struct super_block *sb)
{
  struct me2fs_sb_info *msi;

  msi = ME2FS_SB( sb );
  
  brelse( msi->s_sbh );
  sb->s_fs_info = NULL;
  kfree( msi );
}

/*
====================================================================================
  Function    :dbgPrintExt2SB
  Description :print ext2 super block
====================================================================================
*/
static void dbgPrintExt2SB( struct ext2_super_block *esb)
{
  unsigned int value;
  int i;

  value = ( unsigned int )( le32_to_cpu( esb->s_inodes_count ) );
  DBGPRINT( "<ME2FS>s_inodes_count = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_blocks_count ) );
  DBGPRINT( "<ME2FS>s_blocks_count = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_r_blocks_count ) );
  DBGPRINT( "<ME2FS>s_r_blockss_count = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_free_blocks_count ) );
  DBGPRINT( "<ME2FS>s_free_blocks_count = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_free_inodes_count ) );
  DBGPRINT( "<ME2FS>s_free_inodes_count = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_first_data_block ) );
  DBGPRINT( "<ME2FS>s_first_data_block = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_log_block_size ) );
  DBGPRINT( "<ME2FS>s_log_block_size = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_log_frag_size ) );
  DBGPRINT( "<ME2FS>s_log_frag_size = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_blocks_per_group ) );
  DBGPRINT( "<ME2FS>s_blocks_per_group = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_frags_per_group ) );
  DBGPRINT( "<ME2FS>s_frags_per_group = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_inodes_per_group ) );
  DBGPRINT( "<ME2FS>s_inodes_per_group = %u\n", value);

  value = ( unsigned int )( le32_to_cpu( esb->s_mtime ) );
  DBGPRINT( "<ME2FS>s_mtime = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_wtime ) );
  DBGPRINT( "<ME2FS>s_wtime = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_mnt_count ) );
  DBGPRINT( "<ME2FS>s_mnt_count = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_max_mnt_count ) );
  DBGPRINT( "<ME2FS>s_max_mnt_count = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_magic ) );
  DBGPRINT( "<ME2FS>s_magic = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_state ) );
  DBGPRINT( "<ME2FS>s_state = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_errors ) );
  DBGPRINT( "<ME2FS>s_errors = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_minor_rev_level ) );
  DBGPRINT( "<ME2FS>s_minor_rev_level = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_lastcheck ) );
  DBGPRINT( "<ME2FS>s_lastcheck = %u\n", value);

  value = ( unsigned int )( le32_to_cpu( esb->s_creator_os ) );
  DBGPRINT( "<ME2FS>s_creator_os = %u\n", value);
  value = ( unsigned int )( le16_to_cpu( esb->s_rev_level ) );
  DBGPRINT( "<ME2FS>s_rev_level = %u\n", value);
  value = ( unsigned int )( le16_to_cpu( esb->s_def_resuid ) );
  DBGPRINT( "<ME2FS>s_def_resuid = %u\n", value);
  value = ( unsigned int )( le16_to_cpu( esb->s_def_resgid ) );
  DBGPRINT( "<ME2FS>s_def_resgid = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_first_ino ) );
  DBGPRINT( "<ME2FS>s_first_ino = %u\n", value);
  value = ( unsigned int )( le16_to_cpu( esb->s_inode_size ) );
  DBGPRINT( "<ME2FS>s_inode_size = %u\n", value);
  value = ( unsigned int )( le16_to_cpu( esb->s_block_group_nr ) );
  DBGPRINT( "<ME2FS>s_block_group_nr = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_feature_compat ) );
  DBGPRINT( "<ME2FS>s_feature_compat = %u\n", value);
  value = ( unsigned int )( le32_to_cpu( esb->s_feature_incompat ) );
  DBGPRINT( "<ME2FS>s_feature_incompat = %u\n", value);
  DBGPRINT( "<ME2FS>s_uuid[ 16 ] = " );
  for ( i = 0 ; i < 16 ; i++ )
  {
    DBGPRINT( "%02X", esb->s_uuid[ i ] );
  }
  DBGPRINT( "\n" );
  value = ( unsigned int )( le32_to_cpu( esb->s_journal_inum ) );
  DBGPRINT( "<ME2FS>s_journal_inum = %u\n", value );
  value = ( unsigned int )( le32_to_cpu( esb->s_journal_dev ) );
  DBGPRINT( "<ME2FS>s_journal_dev = %u\n", value );
  value = ( unsigned int )( le32_to_cpu( esb->s_last_orphan ) );
  DBGPRINT( "<ME2FS>s_last_orphan = %u\n", value );
  DBGPRINT( "<ME2FS>s_hash_seed[ 4 ] = " );
  for( i = 0 ; i < 4 ; i++ )
  {
    DBGPRINT( "%16X", esb->s_hash_seed[ i ]);
  }
  DBGPRINT( "\n" );
  value = ( unsigned int )( esb->s_def_hash_version );
  DBGPRINT( "<ME2FS>s_def_hash_version = %u\n", value );
  value = ( unsigned int )( le32_to_cpu( esb->s_default_mount_opts ) );
  DBGPRINT( "<ME2FS>s_default_mount_opts = %u\n", value );
  value = ( unsigned int )( le32_to_cpu( esb->s_first_meta_bg ) );
  DBGPRINT( "<ME2FS>s_first_meta_bg = %u\n", value );
}
