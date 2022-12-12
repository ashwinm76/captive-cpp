#include "game.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "action.h"
#include "command.h"

Game::Game() : inventory_("Inventory"), nowhere_("Nowhere") {
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
      new Room("Prison cell", {2, 0, 0, 0}),
      new Room("Bell tower", {0, 1, 0, 3}),
      new Room("Winding staircase", {4, 0, 2, 0}),
      new Room("Gunpowder chamber", {0, 3, 5, 6}),
      new Room("Place with a rocky floor", {7, 0, 0, 4}),
      new Room("Wall with scratches on it", {0, 0, 4, 0}),
      new Room("Signal transmitter room", {0, 5, 8, 9}),
      new Room("Room of chains", {10, 11, 0, 7}),
      new Room("Padded cell", {0, 0, 7, 0}),
      new Room("Area with a hole in the ceiling", {12, 8, 13, 14}),
      new Room("Muddy area", {8, 15, 16, 0}),
      new Room("Altar", {0, 10, 0, 0}),
      new Room("Place beside a monolith", {0, 0, 0, 10}),
      new Room("Dimly lit passage", {0, 0, 10, 17}),
      new Room("Locksmiths", {11, 0, 0, 0}),
      new Room("Frozen room", {0, 0, 0, 11}),
      new Room("Brightly coloured room", {18, 0, 14, 19}),
      new Room("Observation point", {0, 17, 20, 0}),
      new Room("Repairs room", {0, 0, 17, 0}),
      new Room("Air lock", {21, 0, 0, 18}),
      new Room("Outside of ship", {0, 20, 0, 0}),
  };

  // Create action handlers
  get_handler =
      new ActionHandler(Action::kGet, this, [](Game* game, const Item* item) {
        if (game->inventory_.HasItem(item)) {
          std::cout << "You already have " << item->name() << "." << std::endl;
        } else if (!game->CurrentRoom()->HasItem(item)) {
          std::cout << "I don't see " << item->name() << " here." << std::endl;
        } else {
          game->CurrentRoom()->RemoveItem(item);
          game->inventory_.PutItem(item);
          std::cout << "OK" << std::endl;
        }
      });

  drop_handler =
      new ActionHandler(Action::kDrop, this, [](Game* game, const Item* item) {
        if (!game->inventory_.HasItem(item)) {
          std::cout << "You don't have " << item->name() << "." << std::endl;
        } else {
          game->CurrentRoom()->PutItem(item);
          game->inventory_.RemoveItem(item);
          std::cout << "OK" << std::endl;
        }
      });

  // Create items
  Item* item;
  // Grenade
  item = new Item("grenade", {get_handler, drop_handler});
  GetRoom("Gunpowder chamber")->PutItem(item);
  // Rough metal
  item = new Item("rough metal", {get_handler, drop_handler});
  GetRoom("Room of chains")->PutItem(item);
  // Shiny key
  item = new Item("shiny key", {get_handler, drop_handler});
  nowhere_.PutItem(item);
  // Ice block
  item = new Item("ice block", {get_handler, drop_handler});
  GetRoom("Frozen room")->PutItem(item);
  // Gloves
  item = new Item("gloves", {get_handler, drop_handler});
  GetRoom("Place with a rocky floor")->PutItem(item);
  // sabre
  item = new Item("sabre", {get_handler, drop_handler});
  GetRoom("Altar")->PutItem(item);
  // aerial
  item = new Item("aerial", {get_handler, drop_handler});
  nowhere_.PutItem(item);
  // Torch
  item = new Item("torch", {get_handler, drop_handler});
  GetRoom("Winding staircase")->PutItem(item);
  // Headphones
  item = new Item("headphones", {get_handler, drop_handler});
  GetRoom("Padded cell")->PutItem(item);
  // Magnifier
  item = new Item("magnifier", {get_handler, drop_handler});
  GetRoom("Brightly coloured room")->PutItem(item);
  // Locked door
  item = new Item("locked door", {});
  GetRoom("Air lock")->PutItem(item);
  // Door
  item = new Item("door", {});
  GetRoom("Prison cell")->PutItem(item);
  // Bell
  item = new Item("bell", {});
  GetRoom("Bell tower")->PutItem(item);
  // Scratches
  item = new Item("scratches", {});
  GetRoom("Wall with scratches on it")->PutItem(item);
  // Key cutter
  item = new Item("key cutter", {});
  GetRoom("Locksmiths")->PutItem(item);
  // Hole
  item = new Item("hole", {});
  GetRoom("Area with a hole in the ceiling")->PutItem(item);
  // Transmitter
  item = new Item("transmitter", {});
  GetRoom("Signal transmitter room")->PutItem(item);
  // Window
  item = new Item("window", {});
  GetRoom("Observation point")->PutItem(item);
  // Mud man
  item = new Item("mud man", {});
  GetRoom("Muddy area")->PutItem(item);
  // Wire
  item = new Item("wire", {});
  GetRoom("Repairs room")->PutItem(item);
  // Inscription
  item = new Item("inscription", {});
  GetRoom("Place beside a monolith")->PutItem(item);
  // Boulders
  item = new Item("boulders", {});
  nowhere_.PutItem(item);
  // Swarck
  item = new Item("swarck", {});
  nowhere_.PutItem(item);
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
      continue;
    }

    switch (cmd_.action()) {
      case Action::kQuit:
        running = false;
        break;
      default: {
        std::string obj = cmd_.object();
        const Item* item = GetItem(obj);
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

const Item* Game::GetItem(const std::string& name) const {
  const Item* i = rooms_[room_ - 1]->GetItem(name);
  if (i == nullptr) {
    i = inventory_.GetItem(name);
  }
  return i;
}

Room* Game::GetRoom(const std::string& name) {
  auto r =
      std::find_if(rooms_.begin(), rooms_.end(),
                   [&name](const Room* room) { return room->name() == name; });
  return (r == rooms_.end()) ? nullptr : *r;
}