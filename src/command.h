#ifndef CAPTIVE_SRC_COMMAND_H_
#define CAPTIVE_SRC_COMMAND_H_

#include <string>

#include "action.h"

class Command {
 public:
  Command() : action_(Action::kUnknown), object_("") {}

  void Parse(const std::string& str) {
    int sep = str.find_first_of(' ');
    std::string s = str.substr(0, sep);
    if (s[0] == 'n') {
      action_ = Action::kMoveNorth;
    } else if (s[0] == 's') {
      action_ = Action::kMoveSouth;
    } else if (s[0] == 'e') {
      action_ = Action::kMoveEast;
    } else if (s[0] == 'w') {
      action_ = Action::kMoveWest;
    } else if (s == "quit") {
      action_ = Action::kQuit;
    } else if (s == "get") {
      action_ = Action::kGet;
    } else if (s == "drop") {
      action_ = Action::kDrop;
    } else if (s == "throw") {
      action_ = Action::kThrow;
    } else if (s == "cut") {
      action_ = Action::kCut;
    } else if (s == "wear") {
      action_ = Action::kWear;
    } else if (s == "light") {
      action_ = Action::kLight;
    } else if (s == "kick") {
      action_ = Action::kKick;
    } else if (s == "ring") {
      action_ = Action::kRing;
    } else if (s == "read") {
      action_ = Action::kRead;
    } else if (s == "look") {
      action_ = Action::kLook;
    } else if (s == "kill") {
      action_ = Action::kKill;
    } else if (s == "say") {
      action_ = Action::kSay;
    } else if (s == "transmit") {
      action_ = Action::kTransmit;
    } else {
      action_ = Action::kUnknown;
    }
    object_ = (sep == std::string::npos) ? "" : str.substr(sep + 1);
  }

  Action action() const { return action_; }
  const std::string& object() const { return object_; }

 private:
  Action action_;
  std::string object_;
};

#endif