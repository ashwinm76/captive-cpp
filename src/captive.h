#ifndef CAPTIVE_SRC_CAPTIVE_H
#define CAPTIVE_SRC_CAPTIVE_H

#include "game.h"

class Captive : public Game {
 public:
  Captive();

 protected:
  void CreateRooms();

  void CreateActions();

  void CreateItems();

  void AfterTurn();

 private:
  bool transmitter_overheating_;
  bool air_lock_open_;
  bool air_lock_door_open_;
  int turns_;
};

#endif