#ifndef CAPTIVE_SRC_ROOM_H_
#define CAPTIVE_SRC_ROOM_H_

#include <algorithm>
#include <array>
#include <string>

#include "common.h"

class Item;

class Room {
 public:
  Room(const std::string& name, std::array<int, 4> connections = {0, 0, 0, 0})
      : name_(name), connections_(connections) {}

  std::string name() const { return name_; }

  int GetConnectedRoom(Direction dir) const {
    return connections_[static_cast<int>(dir)];
  }

  bool HasExit(Direction dir) const {
    return connections_[static_cast<int>(dir)] > 0;
  }

  void EnableExit(Direction dir) {
    if (connections_[static_cast<int>(dir)] < 0) {
      connections_[static_cast<int>(dir)] *= -1;
    }
  }

  void PutItem(Item* item) { items_.push_back(item); }

  void RemoveItem(const Item* item) {
    items_.erase(std::find(items_.begin(), items_.end(), item));
  }

  void RemoveItem(const std::string& name) {
    Item* item = GetItem(name);
    if (item != nullptr) {
      RemoveItem(item);
    }
  }

  bool HasItem(const Item* item) const {
    return std::any_of(items_.begin(), items_.end(),
                       [item](const Item* i) { return (i == item); });
  }

  bool HasItem(const std::string& name) const {
    return (GetItem(name) != nullptr);
  }

  Item* GetItem(const std::string& name) const {
    auto i = std::find_if(
        items_.begin(), items_.end(),
        [&name](const Item* item) { return item->name() == name; });
    return (i == items_.end()) ? nullptr : *i;
  }

  std::vector<Item*> items() const { return items_; }

 private:
  std::string name_;
  std::array<int, 4> connections_;
  std::vector<Item*> items_;
};

#endif