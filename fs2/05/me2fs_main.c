/**********************************************************************************
 *
 * file             : me2fs_main.c
 * Descriptions     : my ext2 file system main routine
 *
 **********************************************************************************/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/blkdev.h>

#include "me2fs.h"
#include "me2fs_super.h"
#include "me2fs_util.h"

/*
====================================================================================
  Prototype Statement
====================================================================================
*/

static struct dentry *me2fs_mount( struct      file_system_type *fs_type,
                                   int         flags,
				   const char  *dev_name,
				   void        *data );

/*
====================================================================================
  Defines
====================================================================================
*/

#define ME2FS_VERSION_STRING    "me2fs_0.01"
#define ME2FS_PROC_MODULE_NAME  "me2fs"

#define ME2FS_MODULE_DESC       "My ext2 file system (me2fs)"
#define ME2FS_MODULE_AUTHOR     "Shin Matsuzaki <shin.matsuzaki.777@gmail.com>"


/*
====================================================================================
  Management
====================================================================================
*/

/*
------------------------------------------------------------------------------------
  File system type
------------------------------------------------------------------------------------
*/
static struct file_system_type me2fs_fstype =
{
    .name       = "me2fs",
    .mount      = me2fs_mount,
    .kill_sb    = kill_block_super,
    .fs_flags   = FS_REQUIRES_DEV,
};

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  < Open Functions >
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/*
 * _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_

   Function    :void
   Input       :void
   Output      :void
   Return      :void

   Description :void
 * _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_
 */

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  < Local functions >
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/* 
====================================================================================
  Function     :me2fs_mount
  Description  :mount a ext2 fs
====================================================================================
*/

static struct dentry *me2fs_mount( struct     file_system_type *fs_type,     // file system type
                                   int        flags,                         // flags for mounting
				   const char *dev_name,                     // device file name
				   void       *data )                        // options
{
  // DEBUG
  DBGPRINT("<DEBUG>me2fs_mount() on main called");

  return( me2fsMountBlockDev( fs_type, flags, dev_name, data ) );
}

/*
====================================================================================
  Function     :initMe2fs
  Description  :initialize Me2fs module
====================================================================================
*/

static int __init initMe2fs( void )
{
  DBGPRINT( "(Me2FS)Hello, world\n" );

  return( register_filesystem( &me2fs_fstype ) );
}

/*
====================================================================================
  Function     :exitMe2fs
  Description :clean up me2fs module
====================================================================================
*/

static void __exit exitMe2fs( void )
{
  DBGPRINT( "(Me2FS)Good-bye!\n" );

  unregister_filesystem( &me2fs_fstype );
}

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  < Kernel Registrations >

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/*
------------------------------------------------------------------------------------
  General Module Information
------------------------------------------------------------------------------------
*/

module_init( initMe2fs );
module_exit( exitMe2fs );
MODULE_AUTHOR( ME2FS_MODULE_AUTHOR);
MODULE_DESCRIPTION( ME2FS_MODULE_DESC );
MODULE_LICENSE( "GPL" );
