#ifndef CAPTIVE_SRC_ROOM_H_
#define CAPTIVE_SRC_ROOM_H_

#include <string>
#include <array>
#include <algorithm>
#include "common.h"

class Item;

class Room {
 public:
  Room(const std::string& name, std::array<int, 4> connections = {0, 0, 0, 0}) 
      : name_(name), connections_(connections) { 
  }
  
  std::string name() const { return name_; }
  
  int GetConnectedRoom(Direction dir) const {
    return connections_[static_cast<int>(dir)];
  }

  bool HasExit(Direction dir) const {
    return connections_[static_cast<int>(dir)] != 0;
  }

  void PutItem(const Item* item) {
    items_.push_back(item); 
  }
  
  void RemoveItem(const Item* item) {
    items_.erase(std::find(items_.begin(), items_.end(), item));
   }

   bool HasItem(const Item* item) const {
    return std::any_of(items_.begin(), items_.end(), 
                       [item](const Item *i) { return (i == item); });
   }

   std::vector<const Item*> items() const { return items_; }
   
  private:
   std::string name_;
   std::array<int, 4> connections_;
   std::vector<const Item*> items_;
};

#endif