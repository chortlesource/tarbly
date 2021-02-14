////////////////////////////////////////////////////////////////////////////
//
// tarbly - header.c
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

#include "tarbly.h"


/////////////////////////////////////////////////////////////
// TARBLY FUNCTION IMPLEMENTATION
//

HEADER_H* make_header(char *path) {

  // First allocate memory to the pointer and 0-fill.
  HEADER_H *header = (HEADER_H*) malloc(sizeof(HEADER_H));
  memset(header->data_ptr, 0, BLOCK_SIZE);
  int errcount = 0;

  // Next stat the file to obtain relevant information
  struct stat st;
  if(stat(path, &st) != 0) {
    printf("[TARBLY: Unable to obtain file data: %s\n", path);
    ++errcount;
  }

  // Configure the 'name' field and ensure is between 0 - 100
  int rvalue = snprintf(header->data.name, 100, path);

  if(rvalue >= 100 || rvalue < 0) {
    printf("[TARBLY]: Invalid filename: %s\n", path);
    ++errcount;
  }

  // Configure the mode field
  octal_from_value(header->data.mode, st.st_mode, 8);

  // Configure the uid field
  octal_from_value(header->data.uid, st.st_uid, 8);

  // Configure the gid field
  octal_from_value(header->data.gid, st.st_gid, 8);

  // Configure the size field
  FILE *file;
  if((file = fopen(path, "rb")) != NULL) {
    fseek(file, 0L, SEEK_END);
    octal_from_value(header->data.size, ftell(file), 12);
    fclose(file);
  } else {
    printf("[TARBLY]: Unable to open file: %s\n", path);
    ++errcount;
  }

  // Configure the mtime field
  octal_from_value(header->data.mtime, st.st_mtime, 12);

  // Configure the checksum field
  strncpy(header->data.checksum, CHCK_BLANK, strnlen(CHCK_BLANK, 8));

  // Configure the typeflag
  switch(st.st_mode & S_IFMT) {
    case S_IFREG:
      header->data.typeflag = '0';
      break;
    case S_IFLNK:
      header->data.typeflag = '2';
      break;
    case S_IFCHR:
      header->data.typeflag = '3';
      break;
    case S_IFBLK:
      header->data.typeflag = '4';
      break;
    case S_IFDIR:
      header->data.typeflag = '5';
      break;
    case S_IFIFO:
      header->data.typeflag = '6';
      break;
    default:
      ++errcount;
      printf("[TARBLY]: Error unrecognised file type.\n");
      break;
  };

  // Configure the magic field
  strncpy(header->data.magic, MAGIC, strnlen(MAGIC, 6));

  // Configure the version field
  header->data.version[0] = '0';
  header->data.version[1] = '0';

  // Configure the uname field
  struct passwd *pass;
  pass = getpwuid(st.st_uid);
  strncpy(header->data.uname, pass->pw_name, strnlen(pass->pw_name, 32));

  // Configure the gname field
  struct group *group;
  if((group = getgrgid(st.st_gid)) != NULL )
    strncpy(header->data.gname, group->gr_name, strnlen(group->gr_name, 32));

  // Configure the devmajor field
  octal_from_value(header->data.devmajor, major(st.st_dev), 8);

    // Configure the devminor field
  octal_from_value(header->data.devminor, minor(st.st_dev), 8);

  // Fail by design unless all conditions are met
  if(errcount == 0)
    return header;

  printf("[TARBLY]: Unable to generate header file for: %s\n", path);
  return NULL;
}


void print_header(HEADER_H *header) {
  size_t mode  = 0;
  size_t uid   = 0;
  size_t gid   = 0;
  size_t size  = 0;
  size_t mtime = 0;
  size_t maj   = 0;
  size_t min   = 0;

  value_from_octal(header->data.mode, &mode);
  value_from_octal(header->data.uid, &uid);
  value_from_octal(header->data.gid, &gid);
  value_from_octal(header->data.size, &size);
  value_from_octal(header->data.mtime, &mtime);
  value_from_octal(header->data.devmajor, &maj);
  value_from_octal(header->data.devminor, &min);

  printf("|---------- HEADER ---------- |\n");
  printf("NAME: %s\n", header->data.name);
  printf("MODE: %zu\n", mode);
  printf("UID : %zu\n", uid);
  printf("GID : %zu\n", gid);
  printf("SIZE: %zu\n", size);
  printf("TIME: %zu\n", mtime);
  printf("SUM : ");
  print_chars(header->data.checksum, 8);
  printf("TYPE: %c\n", header->data.typeflag);
  printf("MAGI: ");
  print_chars(header->data.magic, 6);
  printf("VERS: %c", header->data.version[0]);
  printf("%c\n", header->data.version[1]);
  printf("UNAM: %s\n", header->data.uname);
  printf("GNAM: %s\n", header->data.gname);
  printf("|----------   END  ---------- |\n");
}


void print_chars(char *str, size_t num) {
  for(size_t i = 0; i < num; i++)
    printf("%c", str[i]);
  printf("\n");
}
