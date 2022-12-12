#ifndef CAPTIVE_SRC_GAME_H
#define CAPTIVE_SRC_GAME_H

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

 private:
  /* Locations > 0 are Room numbers. */
  enum class Location { kNowhere = -1, kInventory = 0 };

  Direction MakeDirection(const Command cmd) const;
  const Item* GetItem(const std::string& name) const;
  Room* GetRoom(const std::string& name);
  Room* CurrentRoom() { return rooms_[room_ - 1]; }

  int room_;
  Command cmd_;
  std::vector<Room*> rooms_;
  Room inventory_;
  Room nowhere_;
  ActionHandler* get_handler;
  ActionHandler* drop_handler;
};

#endif