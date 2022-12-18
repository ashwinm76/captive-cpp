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

#ifndef CAPTIVE_SRC_ACTION_H_
#define CAPTIVE_SRC_ACTION_H_

#include <functional>

class Item;
class Game;

enum class Action {
  kUnknown,
  kMoveNorth,
  kMoveSouth,
  kMoveEast,
  kMoveWest,
  kQuit,
  kGet,
  kDrop,
  kThrow,
  kCut,
  kWear,
  kLight,
  kKick,
  kRing,
  kRead,
  kLook,
  kKill,
  kSay,
  kTransmit,
  kOpen
};

/* The action handler acts on an Item (without modifying it) and can change
   the Game state. */
class ActionHandler {
 public:
  ActionHandler(Action action, Game* game,
                std::function<void(Game*, Item*)> handler)
      : action_(action), game_(game), handler_(handler) {}

  void operator()(Item* item) const { handler_(game_, item); }

  Action action() const { return action_; }

 private:
  Action action_;
  Game* game_;
  std::function<void(Game*, Item*)> handler_;
};

#endif