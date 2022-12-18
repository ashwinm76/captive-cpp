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

#ifndef CAPTIVE_SRC_COMMAND_H_
#define CAPTIVE_SRC_COMMAND_H_

#include <string>

#include "action.h"

class Command {
 public:
  Command() : action_(Action::kUnknown), object_("") {}

  void Parse(const std::string& str) {
    int sep = str.find_first_of(' ');
    std::string s = str.substr(0, sep);
    if (s == "quit") {
      action_ = Action::kQuit;
    } else if (s == "get") {
      action_ = Action::kGet;
    } else if (s == "drop") {
      action_ = Action::kDrop;
    } else if (s == "throw") {
      action_ = Action::kThrow;
    } else if (s == "cut") {
      action_ = Action::kCut;
    } else if (s == "wear") {
      action_ = Action::kWear;
    } else if (s == "light") {
      action_ = Action::kLight;
    } else if (s == "kick") {
      action_ = Action::kKick;
    } else if (s == "ring") {
      action_ = Action::kRing;
    } else if (s == "read") {
      action_ = Action::kRead;
    } else if (s == "look") {
      action_ = Action::kLook;
    } else if (s == "kill") {
      action_ = Action::kKill;
    } else if (s == "say") {
      action_ = Action::kSay;
    } else if (s == "transmit") {
      action_ = Action::kTransmit;
    } else if (s == "open") {
      action_ = Action::kOpen;
    } else if (s[0] == 'n') {
      action_ = Action::kMoveNorth;
    } else if (s[0] == 's') {
      action_ = Action::kMoveSouth;
    } else if (s[0] == 'e') {
      action_ = Action::kMoveEast;
    } else if (s[0] == 'w') {
      action_ = Action::kMoveWest;
    } else {
      action_ = Action::kUnknown;
    }
    object_ = (sep == std::string::npos) ? "" : str.substr(sep + 1);
  }

  Action action() const { return action_; }
  const std::string& object() const { return object_; }

 private:
  Action action_;
  std::string object_;
};

#endif