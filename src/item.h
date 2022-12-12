#ifndef CAPTIVE_SRC_ITEM_H_
#define CAPTIVE_SRC_ITEM_H_

#include <algorithm>
#include <string>
#include <vector>

#include "action.h"

class Item {
 public:
  Item(const std::string& name, 
       std::vector<const ActionHandler*> action_handlers)
      : name_(name), action_handlers_(action_handlers) {}

  std::string name() const { return name_; }

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
  std::vector<const ActionHandler*> action_handlers_;
};

#endif