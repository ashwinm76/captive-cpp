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

#ifndef CAPTIVE_SRC_ROOM_H_
#define CAPTIVE_SRC_ROOM_H_

#include <algorithm>
#include <array>
#include <string>

#include "common.h"

class Item;

class Room {
 public:
  struct Door {
    bool enabled;
    Room* room;
  };

  Room(const std::string& name) : name_(name) {
    for (auto d : connections_) d.enabled = false;
  }

  std::string name() const { return name_; }

  void ConnectRoom(Room* room, Direction dir, bool enabled = true,
                   bool oneway = false) {
    connections_[static_cast<int>(dir)].enabled = enabled;
    connections_[static_cast<int>(dir)].room = room;
    if (!oneway) {
      switch (dir) {
        case Direction::kEast:
          room->ConnectRoom(this, Direction::kWest, enabled, true);
          break;
        case Direction::kWest:
          room->ConnectRoom(this, Direction::kEast, enabled, true);
          break;
        case Direction::kNorth:
          room->ConnectRoom(this, Direction::kSouth, enabled, true);
          break;
        case Direction::kSouth:
          room->ConnectRoom(this, Direction::kNorth, enabled, true);
          break;
        case Direction::kNone:
          break;
      }
    }
  }

  Room* GetConnectedRoom(Direction dir) const {
    return connections_[static_cast<int>(dir)].room;
  }

  bool HasExit(Direction dir) const {
    return connections_[static_cast<int>(dir)].enabled;
  }

  void EnableExit(Direction dir) {
    connections_[static_cast<int>(dir)].enabled = true;
  }

  void PutItem(Item* item) { items_.push_back(item); }

  void RemoveItem(const Item* item) {
    items_.erase(std::find(items_.begin(), items_.end(), item));
  }

  void RemoveItem(const std::string& name) {
    Item* item = GetItem(name);
    if (item != nullptr) {
      RemoveItem(item);
    }
  }

  bool HasItem(const Item* item) const {
    return std::any_of(items_.begin(), items_.end(),
                       [item](const Item* i) { return (i == item); });
  }

  bool HasItem(const std::string& name) const {
    return (GetItem(name) != nullptr);
  }

  Item* GetItem(const std::string& name) const {
    auto i = std::find_if(
        items_.begin(), items_.end(),
        [&name](const Item* item) { return item->name() == name; });
    return (i == items_.end()) ? nullptr : *i;
  }

  std::vector<Item*> items() const { return items_; }

 private:
  std::string name_;
  std::array<Door, 4> connections_;
  std::vector<Item*> items_;
};

#endif