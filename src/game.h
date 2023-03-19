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

#ifndef CAPTIVE_SRC_GAME_H
#define CAPTIVE_SRC_GAME_H

#include <map>
#include <string>
#include <vector>

#include "action.h"
#include "command.h"
#include "io.h"
#include "item.h"
#include "room.h"

class Game {
 public:
  Game(Io* io);

  void Run();

  Direction MakeDirection(const Command cmd) const;

  Item* GetItem(const std::string& name);

  Room* GetRoom(const std::string& name);

  Room* CurrentRoom() { return current_room_; }

  void StartingRoom(Room* room) { starting_room_ = room; }

  void Over() { running_ = false; }

  bool& running() { return running_; }

  Command& cmd() { return cmd_; }

  std::vector<Room*>& rooms() { return rooms_; }

  Room& inventory() { return inventory_; }

  virtual bool DontHaveItem(Item* item);

  virtual bool HaveItem(Item* item);

  virtual bool ItemInCurrentRoom(Item* item);

  Io* io() { return io_; }

  Room& nowhere() { return nowhere_; }

  Item& nothing() { return nothing_; }

  std::map<std::string, ActionHandler*>& handlers() { return handlers_; }

 protected:
  Room* MakeRoom(const std::string& name);
  Item* MakeItem(const std::string& name, const std::string& room = "",
                 std::vector<Action> actions = {});

  virtual void CreateRooms() = 0;

  virtual void CreateActions() = 0;

  virtual void CreateItems() = 0;

  virtual void AfterTurn() = 0;

 private:
  bool running_;
  Room* current_room_;
  Room* starting_room_;
  Command cmd_;
  std::vector<Room*> rooms_;
  std::vector<Item*> items_;
  Room inventory_;
  Room nowhere_;
  Item nothing_;
  std::map<std::string, ActionHandler*> handlers_;
  Io* io_;
};

#endif