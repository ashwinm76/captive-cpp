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

#ifndef CAPTIVE_SRC_ITEM_H_
#define CAPTIVE_SRC_ITEM_H_

#include <algorithm>
#include <string>
#include <vector>

#include "action.h"

enum class ItemState { kWorn = 1, kLit = 2 };

class Item {
 public:
  Item(const std::string& name, std::vector<ActionHandler*> action_handlers)
      : name_(name), action_handlers_(action_handlers) {
    state_ = 0;
  }

  std::string name() const { return name_; }
  std::string msg() const { return msg_; }
  bool Is(const std::string& s) { return name_ == s; }

  void AddActionHandler(ActionHandler* handler) {
    action_handlers_.push_back(handler);
  }

  bool CanHandleAction(Action action) {
    auto a = std::find_if(action_handlers_.begin(), action_handlers_.end(),
                          [action](const ActionHandler* act) {
                            return (act->action() == action);
                          });
    return (a != action_handlers_.end());
  }

  bool HandleAction(Action action) {
    auto a = std::find_if(action_handlers_.begin(), action_handlers_.end(),
                          [action](const ActionHandler* act) {
                            return (act->action() == action);
                          });
    if (a != action_handlers_.end()) {
      bool ret = (*a)->operator()(this);
      if (!ret) msg_ = (*a)->msg();
      return ret;
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
  std::vector<ActionHandler*> action_handlers_;
  std::string msg_;
};

#endif