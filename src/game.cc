#include "game.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "action.h"
#include "command.h"

Game::Game() : inventory_("Inventory") {
  // Create rooms
  /* Map:
        21
        |
      18-20 12
      |     |
  19-17-14-10-13
            |
      9--7--8
        |  |
  6--4--5  11-16
      |     |
      3--2  15
        |
        1
  */
  rooms_ = {
      Room("Prison cell", {2, 0, 0, 0}),
      Room("Bell tower", {0, 1, 0, 3}),
      Room("Winding staircase", {4, 0, 2, 0}),
      Room("Gunpowder chamber", {0, 3, 5, 6}),
      Room("Place with a rocky floor", {7, 0, 0, 4}),
      Room("Wall with scratches on it", {0, 0, 4, 0}),
      Room("Signal transmitter room", {0, 5, 8, 9}),
      Room("Room of chains", {10, 11, 0, 7}),
      Room("Padded cell", {0, 0, 7, 0}),
      Room("Area with a hole in the ceiling", {12, 8, 13, 14}),
      Room("Muddy area", {8, 15, 16, 0}),
      Room("Altar", {0, 10, 0, 0}),
      Room("Place beside a monolith", {0, 0, 0, 10}),
      Room("Dimly lit passage", {0, 0, 10, 17}),
      Room("Locksmiths", {11, 0, 0, 0}),
      Room("Frozen room", {0, 0, 0, 11}),
      Room("Brightly coloured room", {18, 0, 14, 19}),
      Room("Observation point", {0, 17, 20, 0}),
      Room("Repairs room", {0, 0, 17, 0}),
      Room("Air lock", {21, 0, 0, 18}),
      Room("Outside of ship", {0, 20, 0, 0}),
  };

  // Create action handlers
  get_handler =
      new ActionHandler(Action::kGet, this, [](Game* game, const Item* item) {
        if (game->inventory_.HasItem(item)) {
          std::cout << "You already have " << item->name() << "." << std::endl;
        } else if (!game->CurrentRoom().HasItem(item)) {
          std::cout << "I don't see " << item->name() << " here." << std::endl;
        } else {
          game->CurrentRoom().RemoveItem(item);
          game->inventory_.PutItem(item);
          std::cout << "OK" << std::endl;
        }
      });

  drop_handler =
      new ActionHandler(Action::kDrop, this, [](Game* game, const Item* item) {
        if (!game->inventory_.HasItem(item)) {
          std::cout << "You don't have " << item->name() << "." << std::endl;
        } else {
          game->CurrentRoom().PutItem(item);
          game->inventory_.RemoveItem(item);
          std::cout << "OK" << std::endl;
        }
      });

  // Create items
  items_ = {
      Item("grenade", 4, {get_handler, drop_handler}),
      Item("rough metal", 8, {get_handler, drop_handler}),
      Item("shiny key", static_cast<int>(Location::kNowhere),
           {get_handler, drop_handler}),
      Item("ice block", 16, {get_handler, drop_handler}),
      Item("gloves", 5, {get_handler, drop_handler}),
      Item("sabre", 12, {get_handler, drop_handler}),
      Item("aerial", static_cast<int>(Location::kNowhere),
           {get_handler, drop_handler}),
      Item("torch", 3, {get_handler, drop_handler}),
      Item("headphones", 9, {get_handler, drop_handler}),
      Item("magnifier", 17, {get_handler, drop_handler}),
      Item("locked door", 20, {}),
      Item("door", 1, {}),
      Item("bell", 2, {}),
      Item("scratches", 6, {}),
      Item("key cutter", 15, {}),
      Item("hole", 10, {}),
      Item("transmitter", 7, {}),
      Item("window", 18, {}),
      Item("mud man", 11, {}),
      Item("wire", 19, {}),
      Item("inscription", 13, {}),
      Item("boulders", static_cast<int>(Location::kNowhere), {}),
      Item("swarck", static_cast<int>(Location::kNowhere), {}),
  };

  // Add items to rooms
  for_each(items_.begin(), items_.end(), [this](const Item& item) {
    int loc = item.location();
    if (loc > 0 && loc <= this->rooms_.size()) {
      this->rooms_[loc - 1].PutItem(&item);
    }
  });
}

void Game::Run() {
  room_ = 1;
  bool running = true;

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

  while (running) {
    std::string input;
    Room* current_room = &CurrentRoom();

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
      continue;
    }

    switch (cmd_.action()) {
      case Action::kQuit:
        running = false;
        break;
      default: {
        std::string obj = cmd_.object();
        const Item* item = FindItem(obj);
        if (item == nullptr) {
          std::cout << "I don't see " << obj << " anywhere." << std::endl;
        } else {
          if (item->HandleAction(cmd_.action())) {
          } else {
            std::cout << "I can't do that." << std::endl;
          }
        }
        break;
      }
    }
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

const Item* Game::FindItem(const std::string name) const {
  auto i = std::find_if(items_.begin(), items_.end(), [name](const Item& item) {
    return (item.name() == name);
  });
  return ((i == items_.end()) ? nullptr : &(*i));
}