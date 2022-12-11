#ifndef CAPTIVE_SRC_ITEM_H_
#define CAPTIVE_SRC_ITEM_H_

#include <algorithm>
#include <string>
#include <vector>

#include "action.h"

class Item {
 public:
  Item(const std::string& name, int location,
       std::vector<const ActionHandler*> action_handlers)
      : name_(name), location_(location), action_handlers_(action_handlers) {}

  std::string name() const { return name_; }

  int location() const { return location_; }

  bool HandleAction(Action action) const {
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

 private:
  std::string name_;
  int location_;
  std::vector<const ActionHandler*> action_handlers_;
};

#endif