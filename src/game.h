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
  const Item* FindItem(const std::string item) const;
  Room& CurrentRoom() { return rooms_[room_ - 1]; }

  int room_;
  Command cmd_;
  std::vector<Room> rooms_;
  std::vector<Item> items_;
  Room inventory_;
  ActionHandler* get_handler;
  ActionHandler* drop_handler;
};

#endif