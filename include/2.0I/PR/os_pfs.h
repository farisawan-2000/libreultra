
/*====================================================================
 * os_pfs.h
 *
 * Copyright 1995, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: os_pfs.h,v $
        $Revision: 1.1 $
        $Date: 1998/10/09 08:01:16 $
 *---------------------------------------------------------------------*/

#ifndef _OS_PFS_H_
#define _OS_PFS_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/ultratypes.h>
#include "os_message.h"


#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/**************************************************************************
 *
 * Type definitions
 *
 */

/*
 * Structure for file system
 */
typedef struct {
        /* 0x00 */ int             status;
        /* 0x04 */ OSMesgQueue     *queue;
        /* 0x08 */ int             channel;
        /* 0x0C */ u8              id[0x20];
        /* 0x2C */ u8              label[0x20];
        /* 0x4C */ int             version;
        /* 0x50 */ int             dir_size;
        /* 0x54 */ int             inode_table;            /* block location */
        /* 0x58 */ int             minode_table;           /* mirrioring inode_table */
        /* 0x5C */ int             dir_table;              /* block location */
        /* 0x60 */ int             inode_start_page;       /* page # */
        /* 0x62 */ u8              banks;
        /* 0x64 */ u8              activebank;
} OSPfs; // size: 0x68?

typedef struct {
        u32     file_size;      /* bytes */
        u32     game_code;
        u16     company_code;
        char    ext_name[4];
        char    game_name[16];
} OSPfsState;


#endif /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */

/**************************************************************************
 *
 * Global definitions
 *
 */


/* File System size */
#define OS_PFS_VERSION          0x0200
#define OS_PFS_VERSION_HI       (OS_PFS_VERSION >> 8)
#define OS_PFS_VERSION_LO       (OS_PFS_VERSION & 255)

#define PFS_INODE_SIZE_PER_PAGE 128
#define PFS_FILE_NAME_LEN       16
#define PFS_FILE_EXT_LEN        4
#define BLOCKSIZE               32  /* bytes */
#define PFS_ONE_PAGE            8   /* blocks */
#define PFS_MAX_BANKS           62

/* File System flag */

#define PFS_READ                0
#define PFS_WRITE               1
#define PFS_CREATE              2

/* File System status */
#define PFS_INITIALIZED         0x1
#define PFS_CORRUPTED           0x2
#define PFS_ID_BROKEN           0x4
#define PFS_MOTOR_INITIALIZED   0x8
#define PFS_GBPAK_INITIALIZED   0x10

/* Definition for page usage */
#define PFS_EOF                 1
#define PFS_PAGE_NOT_EXIST      2
#define PFS_PAGE_NOT_USED       3

/* File System error number */

#define PFS_ERR_NOPACK          1   /* no memory card is plugged or */
#define PFS_ERR_NEW_PACK        2   /* ram pack has been changed to a different one */
#define PFS_ERR_INCONSISTENT    3   /* need to run Pfschecker*/
#define PFS_ERR_CONTRFAIL       CONT_OVERRUN_ERROR              
#define PFS_ERR_INVALID         5   /* invalid parameter or file not exist*/
#define PFS_ERR_BAD_DATA        6   /* the data read from pack are bad*/
#define PFS_DATA_FULL           7   /* no free pages on ram pack*/
#define PFS_DIR_FULL            8   /* no free directories on ram pack*/
#define PFS_ERR_EXIST           9   /* file exists*/
#define PFS_ERR_ID_FATAL        10  /* dead ram pack */
#define PFS_ERR_DEVICE          11  /* wrong device type*/
#define PFS_ERR_NO_GBCART       12  /* no gb cartridge (64GB-PAK) */
#define PFS_ERR_NEW_GBCART      13  /* gb cartridge may be changed */

/* Definition for bank */
#define PFS_ID_BANK_256K    0
#define PFS_ID_BANK_1M      4
#define PFS_BANKS_256K      1

#define PFS_WRITTEN             2
#define DEF_DIR_PAGES           2

#define PFS_ID_0AREA            1
#define PFS_ID_1AREA            3
#define PFS_ID_2AREA            4
#define PFS_ID_3AREA            6
#define PFS_LABEL_AREA          7
#define PFS_ID_PAGE             PFS_ONE_PAGE * 0

#define PFS_BANK_LAPPED_BY  8   /* => u8 */
#define PFS_SECTOR_PER_BANK 32
#define PFS_INODE_DIST_MAP  (PFS_BANK_LAPPED_BY * PFS_SECTOR_PER_BANK)
#define PFS_SECTOR_SIZE     (PFS_INODE_SIZE_PER_PAGE/PFS_SECTOR_PER_BANK)


#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/**************************************************************************
 *
 * Macro definitions
 *
 */


/**************************************************************************
 *
 * Extern variables
 *
 */


/**************************************************************************
 *
 * Function prototypes
 *
 */

/* file system interface */

extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsRepairId(OSPfs *);
extern s32 osPfsInit(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsReFormat(OSPfs *, OSMesgQueue *, int);
extern s32 osPfsChecker(OSPfs *);
extern s32 osPfsAllocateFile(OSPfs *, u16, u32, u8 *, u8 *, int, s32 *);
extern s32 osPfsFindFile(OSPfs *, u16, u32, u8 *, u8 *, s32 *);
extern s32 osPfsDeleteFile(OSPfs *, u16, u32, u8 *, u8 *);
extern s32 osPfsReadWriteFile(OSPfs *, s32, u8, int, int, u8 *);
extern s32 osPfsFileState(OSPfs *, s32, OSPfsState *);
extern s32 osPfsGetLabel(OSPfs *, u8 *, int *);
extern s32 osPfsSetLabel(OSPfs *, u8 *);
extern s32 osPfsIsPlug(OSMesgQueue *, u8 *);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern s32 osPfsNumFiles(OSPfs *, s32 *, s32 *);


#endif  /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !_OS_PFS_H_ */
