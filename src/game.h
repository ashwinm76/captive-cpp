#ifndef CAPTIVE_SRC_GAME_H
#define CAPTIVE_SRC_GAME_H

#include <map>
#include <string>
#include <vector>

#include "action.h"
#include "command.h"
#include "item.h"
#include "room.h"

class Game {
 public:
  Game();

  void Run();

  Direction MakeDirection(const Command cmd) const;

  Item* GetItem(const std::string& name) const;

  Room* GetRoom(const std::string& name);

  Room* CurrentRoom() { return rooms_[room_ - 1]; }

  void Over() { running_ = false; }

  bool& running() { return running_; }

  int& room() { return room_; }

  Command& cmd() { return cmd_; }

  std::vector<Room*>& rooms() { return rooms_; }

  Room& inventory() { return inventory_; }

  Room& nowhere() { return nowhere_; }

  Item& nothing() { return nothing_; }

  std::map<std::string, ActionHandler*>& handlers() { return handlers_; }

 protected:
  virtual void CreateRooms() = 0;

  virtual void CreateActions() = 0;

  virtual void CreateItems() = 0;

  virtual void AfterTurn() = 0;

 private:
  bool running_;
  int room_;
  Command cmd_;
  std::vector<Room*> rooms_;
  Room inventory_;
  Room nowhere_;
  Item nothing_;
  std::map<std::string, ActionHandler*> handlers_;
};

#endif