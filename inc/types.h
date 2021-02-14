////////////////////////////////////////////////////////////////////////////
//
// tarbly - types.h
//
// Copyright (c) 2021 Christopher M. Short
//
// This file is part of ink.
//
// ink is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ink is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ink. If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////

#ifndef _TARBLY_TYPES_H
#define _TARBLY_TYPES_H


/////////////////////////////////////////////////////////////
// MACRO'S
//

#define  BLOCK_SIZE  512
#define  MAGIC       "ustar  "
#define  CHCK_BLANK  "        "

#define  F_NORMAL    '0'        // Normal disk file
#define  F_LINK      '1'        // Link to previously dumped file
#define  F_SYMLINK   '2'        // Symbolic link
#define  F_CHR       '3'        // Character special file
#define  F_BLK       '4'        // Block special file
#define  F_DIR       '5'        // Directory
#define  F_FIFO      '6'        // FIFO special file
#define  F_CONTIG    '7'        // Contiguous file

#define  TSUID       04000      // Set UID on execution
#define  TSGID       02000      // Set GID on execution
#define  TSVTX       01000      // Save text (sticky bit)

#define  TUREAD      00400      // read by owner */
#define  TUWRITE     00200      // write by owner */
#define  TUEXEC      00100      // execute/search by owner */
#define  TGREAD      00040      // read by group */
#define  TGWRITE     00020      // write by group */
#define  TGEXEC      00010      // execute/search by group */
#define  TOREAD      00004      // read by other */
#define  TOWRITE     00002      // write by other */
#define  TOEXEC      00001      // execute/search by other */


/////////////////////////////////////////////////////////////
// TYPES
//

// USTAR Header Struct
typedef struct POSIX_USTAR_H {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char checksum[8];
  char typeflag;
  char linkname[100];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[155];
  char pad[12];
} USTAR_H;


typedef union FILE_HEADER {
  USTAR_H data;
  unsigned char data_ptr [BLOCK_SIZE];
} HEADER_H;


#endif // _TARBLY_TYPES_H
