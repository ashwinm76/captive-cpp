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

#ifndef CAPTIVE_SRC_IOSTD_H
#define CAPTIVE_SRC_IOSTD_H

#include <iostream>
#include <string>

#include "game.h"
#include "io.h"
#include "room.h"

class Iostd : public Io {
 public:
  void WriteInstructions(const std::string& s) { std::cout << s; }

  void WriteRoomInfo(Game& game) {
    Room* room = game.CurrentRoom();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << room->name() << std::endl << "Exits:- ";
    for (auto e : {Direction::kNorth, Direction::kSouth, Direction::kEast,
                   Direction::kWest}) {
      if (room->HasExit(e)) std::cout << ':' << e << ':';
    }

    std::cout << std::endl << std::endl << "Objects:- ";
    for (auto item : room->items()) std::cout << item->name() << ":";

    std::cout << std::endl << std::endl << "Inventory:- ";
    for (auto item : game.inventory().items()) std::cout << item->name() << ":";
    std::cout << std::endl;
  }

  void WriteResponse(const std::string& s) { std::cout << s << std::endl; }

  void WriteResponse(const std::list<const std::string>& m) {
    for (auto s : m) {
      std::cout << s;
    }
    std::cout << std::endl;
  }

  std::string ReadCommand(const std::string& prompt) {
    std::string input;

    std::cout << std::endl << prompt;
    std::getline(std::cin, input);
    return input;
  }
};

#endif
