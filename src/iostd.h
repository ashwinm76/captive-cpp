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
#include <sstream>
#include <string>

#include "game.h"
#include "io.h"
#include "room.h"

class IoStd : public Io {
 public:
  IoStd(int width): width_(width), pos_(0) {}

  void WriteInstructions(const std::string& s) { Write(s); }

  void WriteRoomInfo(Game& game) {
    Room* room = game.CurrentRoom();

    Write(std::string(width_, '-'));
    WriteLn(room->name());
    Write("Exits:- ");
    for (auto e : {Direction::kNorth, Direction::kSouth, Direction::kEast,
                   Direction::kWest}) {
      std::ostringstream os;
      os << e;
      if (room->HasExit(e)) Write(':' + os.str() + ':');
    }

    WriteLn("");
    Write("Objects:- ");
    for (auto item : room->items()) Write(item->name() + ":");

    WriteLn("");
    Write("Inventory:- ");
    for (auto item : game.inventory().items()) Write(item->name() + ":");
    WriteLn("");
  }

  void WriteResponse(const std::string& s) { WriteLn(s); }

  void WriteResponse(const std::list<const std::string>& m) {
    for (auto s : m) {
      Write(s);
    }
    WriteLn("");
  }

  std::string ReadCommand(const std::string& prompt) {
    std::string input;

    Write(prompt);
    std::getline(std::cin, input);
    return input;
  }

 private:
  int width_;
  int pos_;

  void Write(std::string s) {
    std::istringstream ss(s);
    std::string word;

    while(ss >> word) {
      if (pos_ + 1 + word.length() >= width_) {
        pos_ = 0;
        std::cout << std::endl;
      }
      std::cout << word << ' ';
      pos_ += word.length() + 1;
    }
  }

  void WriteLn(std::string s) {
    Write(s);
    std::cout << std::endl;
  }
};

#endif
