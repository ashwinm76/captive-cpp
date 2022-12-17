#include "game.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "action.h"
#include "command.h"

Game::Game()
    : inventory_("Inventory"), nowhere_("Nowhere"), nothing_("nothing", {}) {}

void Game::Run() {
  room_ = 1;
  running_ = true;

  // Print instructions
  std::cout
      << "You have been captured by creatures on an uncharted planet."
      << std::endl
      << "Unfortunately the planet happens to be unstable,and has been "
      << "evacuated." << std::endl
      << "You therefore have to escape before the planet blows up with you on "
      << "it." << std::endl
      << "The computer has a fairly large number of commands,so if one command "
      << "does not work then try another." << std::endl
      << "The first three letters of each command and object need be typed "
      << "in, although, if desired, the full word may be entered." << std::endl;

  while (running_) {
    std::string input;
    Room* current_room = CurrentRoom();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << current_room->name() << std::endl << "Exits:- ";
    for (auto e : {Direction::kNorth, Direction::kSouth, Direction::kEast,
                   Direction::kWest}) {
      if (current_room->HasExit(e)) std::cout << ':' << e << ':';
    }

    std::cout << std::endl << std::endl << "Objects:- ";
    for (auto item : current_room->items()) std::cout << item->name() << ":";

    std::cout << std::endl << std::endl << "Inventory:- ";
    for (auto item : inventory_.items()) std::cout << item->name() << ":";

    std::cout << std::endl << std::endl << "Command? ";
    std::getline(std::cin, input);
    cmd_.Parse(input);

    Direction dir = MakeDirection(cmd_);
    if (dir != Direction::kNone) {
      if (!current_room->HasExit(dir)) {
        std::cout << "No Exit!" << std::endl;
      } else {
        room_ = current_room->GetConnectedRoom(dir);
      }
      AfterTurn();
      continue;
    }

    switch (cmd_.action()) {
      case Action::kQuit:
        running_ = false;
        break;
      default: {
        std::string obj = cmd_.object();
        Item* item = GetItem(obj);
        if (!item) {
          item = &nothing_;
        }
        if (!item->HandleAction(cmd_.action())) {
          std::cout << "I can't do that." << std::endl;
        }
        break;
      }
    }
    AfterTurn();
  }
}

Direction Game::MakeDirection(const Command cmd) const {
  switch (cmd.action()) {
    case Action::kMoveNorth:
      return Direction::kNorth;
    case Action::kMoveSouth:
      return Direction::kSouth;
    case Action::kMoveEast:
      return Direction::kEast;
    case Action::kMoveWest:
      return Direction::kWest;
    default:
      return Direction::kNone;
  }
}

Item* Game::GetItem(const std::string& name) const {
  Item* i = rooms_[room_ - 1]->GetItem(name);
  if (!i) {
    i = inventory_.GetItem(name);
  }
  if (!i) {
    i = nowhere_.GetItem(name);
  }
  return i;
}

Room* Game::GetRoom(const std::string& name) {
  auto r =
      std::find_if(rooms_.begin(), rooms_.end(),
                   [&name](const Room* room) { return room->name() == name; });
  return (r == rooms_.end()) ? nullptr : *r;
}