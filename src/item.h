#ifndef CAPTIVE_SRC_ITEM_H_
#define CAPTIVE_SRC_ITEM_H_

#include <algorithm>
#include <string>
#include <vector>

#include "action.h"

enum class ItemState { kWorn = 1, kLit = 2 };

class Item {
 public:
  Item(const std::string& name,
       std::vector<const ActionHandler*> action_handlers)
      : name_(name), action_handlers_(action_handlers) {
    state_ = 0;
  }

  std::string name() const { return name_; }

  void AddActionHandler(const ActionHandler* handler) {
    action_handlers_.push_back(handler);
  }

  bool HandleAction(Action action) {
    auto a = std::find_if(action_handlers_.begin(), action_handlers_.end(),
                          [action](const ActionHandler* act) {
                            return (act->action() == action);
                          });
    if (a != action_handlers_.end()) {
      (*a)->operator()(this);
      return true;
    } else {
      return false;
    }
  }

  void AddState(ItemState state) { state_ |= static_cast<int>(state); }

  bool HasState(ItemState state) const {
    return state_ & static_cast<int>(state);
  }

 private:
  int state_;
  std::string name_;
  std::vector<const ActionHandler*> action_handlers_;
};

#endif