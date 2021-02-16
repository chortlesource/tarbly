////////////////////////////////////////////////////////////////////////////
//
// tarbly - tar.h
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

#ifndef _TARBLY_TAR_H
#define _TARBLY_TAR_H


/////////////////////////////////////////////////////////////
// TAR FUNCTION DECLARATION
//

// TAR struct manipulation
TAR*      make_tar(char *path);              // Create a new TAR struct
int       add_to_tar(TAR *tar, char *path);  // Add a file to a TAR struct
int       write_to_tar(TAR *tar);            // Write the TAR struct to disk
TAR*      read_from_tar(char *path);         // Read into a TAR struct
void      free_tar(TAR *tar);                // Free memory used by TAR

// HEADER_H manipulation
HEADER_H* make_header(char *path);           // Generate a ustar header
void      make_checksum(HEADER_H *header);   // Generate the checksum
int       verify_checksum(HEADER_H *header); // Verify the checksum

// Print functions
void      print_h(HEADER_H *header);
void      print_c(char *str, size_t num);


#endif // _TARBLY_TAR_H
