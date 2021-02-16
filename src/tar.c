////////////////////////////////////////////////////////////////////////////
//
// tarbly - tar.c
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
// TAR FUNCTION IMPLEMENTATION
//


TAR*      make_tar(char *path) {
  // Allocate memory for the struct and initialize
  TAR  *tar = malloc(sizeof(TAR));
  tar->capacity = 1;
  tar->size     = 0;

  // Allocate memory for the headers and filenames
  size_t psize       = strnlen(path, NAME_SIZE);

  char     *fpath    = malloc(psize * sizeof(char));
  char     **fnames  = malloc(sizeof(char*) * tar->capacity);
  HEADER_H **headers = malloc(sizeof(HEADER_H*) * tar->capacity);

  // Assign the allocated vars to our TAR struct
  tar->path      = fpath;
  tar->filenames = fnames;
  tar->headers   = headers;

  // Copy path to our
  for(size_t i = 0; i < psize; i++)
    tar->path[i] = path[i];

  return tar;
}


int       add_to_tar(TAR *tar, char *path) {
  // Allocate memory for the filename
  size_t psize = strnlen(path, NAME_SIZE);
  char *fpath  = malloc(psize * sizeof(char));

  // Copy the path to our string
  for(size_t i = 0; i < psize; i++)
    fpath[i] = path[i];

  // Create the header for our file
  HEADER_H *header = make_header(fpath);
  if(header == NULL) return 0; // Return false on fail

  // Resize the arrays
  tar->capacity += 1;
  char     **ftemp = realloc(tar->filenames, tar->capacity * sizeof(char*));
  HEADER_H **htemp = realloc(tar->headers, tar->capacity * sizeof(HEADER_H*));

  // Assign the allocated vars to our TAR struct
  tar->filenames            = ftemp;
  tar->headers              = htemp;
  tar->filenames[tar->size] = fpath;
  tar->headers[tar->size]   = header;
  tar->size                += 1;

  return 1; // Return true on success
}


void      free_tar(TAR *tar) {
  // Prevent accidental calling
  if(tar == NULL) return;

  // First free the filenames and headers
  if(tar->size > 0) {
    for(size_t i = 0; i < tar->size; i++) {
      free(tar->filenames[i]);
      free(tar->headers[i]);
    }
  }

  tar->capacity = 0;
  tar->size     = 0;
  free(tar->filenames);
  free(tar->headers);
  free(tar->path);
  free(tar);
}


HEADER_H* make_header(char *path) {

  // First allocate memory to the pointer and 0-fill.
  HEADER_H *header = malloc(sizeof(HEADER_H));
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

  // Generate the checksum
  make_checksum(header);

  // Fail by design unless all conditions are met
  if(errcount == 0)
    return header;

  printf("[TARBLY]: Unable to generate header file for: %s\n", path);
  return NULL;
}


void      make_checksum(HEADER_H *header) {
  // Configure some helper vars
  size_t checksum = 0;
  char   *data    = header->data_ptr;

  // Calculate the sum of all bytes in the header file
  for(int i = 0; i < BLOCK_SIZE; i++)
    checksum += *data++;

  // Represent in Octal ASCII
  octal_from_value(header->data.checksum, checksum, 8);
}


int       verify_checksum(HEADER_H *header) {
  // First retain a copy of the checksum
  size_t oldsum = 0;
  size_t newsum = 0;
  char   *data  = header->data_ptr;

  // Copy the value of the checksum and replace with ' '
  value_from_octal(header->data.checksum, &oldsum);
  strncpy(header->data.checksum, CHCK_BLANK, strnlen(CHCK_BLANK, 8));

  // Calculate the sum of all bytes in the header file
  for(int i = 0; i < BLOCK_SIZE; i++)
    newsum += *data++;

  // Return the checksum
  octal_from_value(header->data.checksum, oldsum, 8);

  printf("OLD: %zu\n", oldsum);
  printf("NEW: %zu\n", newsum);

  return (newsum == oldsum);
}


void print_h(HEADER_H *header) {
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
  print_c(header->data.checksum, 8);
  printf("TYPE: %c\n", header->data.typeflag);
  printf("MAGI: ");
  print_c(header->data.magic, 6);
  printf("VERS: %c", header->data.version[0]);
  printf("%c\n", header->data.version[1]);
  printf("UNAM: %s\n", header->data.uname);
  printf("GNAM: %s\n", header->data.gname);
  printf("|----------   END  ---------- |\n");
}


void print_c(char *str, size_t num) {
  for(size_t i = 0; i < num; i++) {
    if(str[i] == '\0') break; // Will break at num or '\0'

    printf("%c", str[i]);
  }

  printf("\n");
}
