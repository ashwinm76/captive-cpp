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

#include "game.h"

#include <algorithm>
#include <string>

#include "action.h"
#include "command.h"
#include "io.h"

Game::Game(Io* io)
    : inventory_("Inventory"),
      nowhere_("Nowhere"),
      nothing_("nothing", {}),
      io_(io) {}

void Game::Run() {
  room_ = 1;
  running_ = true;

  while (running_) {
    std::string input;
    Room* current_room = CurrentRoom();
    io_->WriteRoomInfo(*this);
    input = io_->ReadCommand("Command? ");
    cmd_.Parse(input);

    Direction dir = MakeDirection(cmd_);
    if (dir != Direction::kNone) {
      if (!current_room->HasExit(dir)) {
        io_->WriteResponse("No Exit!");
      } else {
        room_ = current_room->GetConnectedRoom(dir);
      }
      AfterTurn();
      continue;
    }

    switch (cmd_.action()) {
      case Action::kQuit:
        running_ = false;
        break;
      default: {
        std::string obj = cmd_.object();
        Item* item = GetItem(obj);
        if (!item) {
          item = &nothing_;
        }
        if (!item->HandleAction(cmd_.action())) {
          io_->WriteResponse("I can't do that.");
        }
        break;
      }
    }
    AfterTurn();
  }
}

Direction Game::MakeDirection(const Command cmd) const {
  switch (cmd.action()) {
    case Action::kMoveNorth:
      return Direction::kNorth;
    case Action::kMoveSouth:
      return Direction::kSouth;
    case Action::kMoveEast:
      return Direction::kEast;
    case Action::kMoveWest:
      return Direction::kWest;
    default:
      return Direction::kNone;
  }
}

Item* Game::GetItem(const std::string& name) const {
  Item* i = rooms_[room_ - 1]->GetItem(name);
  if (!i) {
    i = inventory_.GetItem(name);
  }
  if (!i) {
    i = nowhere_.GetItem(name);
  }
  return i;
}

Room* Game::GetRoom(const std::string& name) {
  auto r =
      std::find_if(rooms_.begin(), rooms_.end(),
                   [&name](const Room* room) { return room->name() == name; });
  return (r == rooms_.end()) ? nullptr : *r;
}