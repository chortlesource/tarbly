////////////////////////////////////////////////////////////////////////////
//
// tarbly - tarbly.c
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
// MAIN FUNCTION
//


int main(const int argc, const char *argv[]) {
  // Generate a header
  TAR *tar = make_tar("CMakeCache.txt\n");

  if(add_to_tar(tar, "CMakeCache.txt")) {
    printf("FILE: %s\n", tar->filenames[tar->size - 1]);
    print_h(tar->headers[tar->size -1]);
  }

  // Verify the checksum
  if(verify_checksum(tar->headers[tar->size - 1]))
    printf("Checksum checks out!\n");


  // Print the header again
  print_h(tar->headers[tar->size - 1]);

  // Free memory
  free_tar(tar);

  return 0;
}
