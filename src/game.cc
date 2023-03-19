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
  // Create rooms
  CreateRooms();

  // Create action handlers
  CreateActions();

  // Create items
  CreateItems();

  current_room_ = starting_room_;
  current_room_->Enter(this);
  running_ = true;

  while (running_) {
    std::string input;
    io_->WriteRoomInfo(this);
    input = io_->ReadCommand("Command? ");
    cmd_.Parse(input);

    Direction dir = MakeDirection(cmd_);
    if (dir != Direction::kNone) {
      if (!current_room_->HasExit(dir)) {
        io_->WriteResponse("No Exit!");
      } else {
        current_room_->Turn(this);
        current_room_->Exit(this);
        current_room_ = current_room_->GetConnectedRoom(dir);
        current_room_->Enter(this);
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
    current_room_->Turn(this);
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

Item* Game::GetItem(const std::string& name) {
  Item* i = CurrentRoom()->GetItem(name);
  if (!i) {
    i = inventory_.GetItem(name);
  }
  if (!i) {
    i = nowhere_.GetItem(name);
  }
  return i;
}

Room* Game::GetRoom(const std::string& name) {
  if (nowhere().Is(name)) return &nowhere();
  auto r =
      std::find_if(rooms().begin(), rooms().end(),
                   [&name](const Room* room) { return room->name() == name; });
  return (r == rooms().end()) ? nullptr : *r;
}

Room* Game::MakeRoom(const std::string& name) {
  Room* r = new Room(name);
  rooms().push_back(r);
  return r;
}

Item* Game::MakeItem(const std::string& name, const std::string& room,
                     std::vector<Action> actions) {
  std::vector<const ActionHandler*> h;
  for (auto a : actions) {
    for (auto hnd : handlers()) {
      if (hnd.second->action() == a) {
        h.push_back(hnd.second);
        break;
      }
    }
  }
  Item* item = new Item(name, h);
  if (room != "") {
    GetRoom(room)->PutItem(item);
  }
  return item;
}

bool Game::DontHaveItem(Item* item) {
  if (inventory().HasItem(item)) {
    io()->WriteResponse({"You already have ", item->name(), "."});
    return false;
  }
  return true;
}

bool Game::HaveItem(Item* item) {
  if (!inventory().HasItem(item)) {
    io()->WriteResponse({"You don't have ", item->name(), "."});
    return false;
  }
  return true;
}

bool Game::ItemInCurrentRoom(Item* item) {
  if (!CurrentRoom()->HasItem(item)) {
    io()->WriteResponse({"I don't see ", item->name(), " here."});
    return false;
  }
  return true;
}