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

#ifndef CAPTIVE_SRC_IO_H
#define CAPTIVE_SRC_IO_H

#include <list>
#include <string>

#include "game.h"

class Io {
 public:
  virtual void WriteInstructions(const std::string& s) = 0;
  virtual void WriteRoomInfo(Game& game) = 0;
  virtual void WriteResponse(const std::string& s) = 0;
  virtual void WriteResponse(const std::list<const std::string>& m) = 0;
  virtual std::string ReadCommand(const std::string& prompt) = 0;
};

#endif
