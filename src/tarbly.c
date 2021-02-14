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
  HEADER_H *header = make_header("CMakeCache.txt");

  // Print the details
  print_header(header);

  // Free memory
  free(header);

  return 0;
}
