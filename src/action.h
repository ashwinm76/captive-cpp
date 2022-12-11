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
  kTransmit
};

/* The action handler acts on an Item (without modifying it) and can change
   the Game state. */
class ActionHandler {
 public:
  ActionHandler(Action action, Game* game,
                std::function<void(Game*, const Item*)> handler)
      : action_(action), game_(game), handler_(handler) {}

  void operator()(const Item* item) const { handler_(game_, item); }

  Action action() const { return action_; }

 private:
  Action action_;
  Game* game_;
  std::function<void(Game*, const Item*)> handler_;
};

#endif