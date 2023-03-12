/*
Captive - A C++ port of the BASIC type-in game 'captive'
Copyright (C) 2022  Ashwin Menon

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <vector>

#include "captive.h"
#include "iotest.h"

int main(int argc, char **argv) {
  IoTest io({"kick door",
             "n",
             "w",
             "get torch",
             "n",
             "get grenade",
             "w",
             "read scratches",
             "e",
             "e",
             "get gloves",
             "n",
             "w",
             "get headphones",
             "e",
             "e",
             "get rough metal",
             "n",
             "light torch",
             "w",
             "w",
             "get magnifier",
             "e",
             "e",
             "e",
             "read inscription",
             "w",
             "n",
             "get sabre",
             "s",
             "s",
             "s",
             "kill mud man",
             "s",
             "cut rough metal",
             "get shiny key",
             "n",
             "e",
             "wear gloves",
             "get ice block",
             "w",
             "n",
             "w",
             "drop ice block",
             "e",
             "n",
             "w",
             "w",
             "w",
             "get aerial",
             "e",
             "e",
             "e",
             "s",
             "w",
             "drop aerial",
             "transmit",
             "e",
             "n",
             "w",
             "w",
             "n",
             "look window",
             "e",
             "open locked door",
             "wear headphones",
             "throw grenade",
             "n",
             "say swarck"},
            "You have materialised inside your ship which has immediately "
            "taken off.");
  Captive captive(&io);

  captive.Run();
  return io.passed() ? 0 : 1;
}