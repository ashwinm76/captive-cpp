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

#ifndef CAPTIVE_SRC_CAPTIVE_H
#define CAPTIVE_SRC_CAPTIVE_H

#include "game.h"
#include "io.h"

class Captive : public Game {
 public:
  Captive(Io* io);

 protected:
  void CreateRooms();

  void CreateActions();

  void CreateItems();

  void AfterTurn();

 private:
  bool transmitter_overheating_;
  bool air_lock_open_;
  bool air_lock_door_open_;
  int turns_;
};

#endif