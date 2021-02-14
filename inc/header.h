////////////////////////////////////////////////////////////////////////////
//
// tarbly - header.h
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

#ifndef _TARBLY_HEADER_H
#define _TARBLY_HEADER_H


/////////////////////////////////////////////////////////////
// HEADER FUNCTION DECLARATION
//

HEADER_H* make_header(char *path);
void      print_header(HEADER_H *header);
void      print_chars(char *str, size_t num);


#endif // _TARBLY_HEADER_H
