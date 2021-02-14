////////////////////////////////////////////////////////////////////////////
//
// tarbly - util.h
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
// UTILITY FUNCTION IMPLEMENTATION
//

void octal_from_value(char *field, size_t value, int len) {
  snprintf(field, len, "%*lo\n", len -1, value);
}


void value_from_octal(char *octal, size_t *value) {
  sscanf(octal, "%zo", value);
}
