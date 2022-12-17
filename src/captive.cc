#include "captive.h"

Captive::Captive() {
  transmitter_overheating_ = true;
  air_lock_open_ = false;
  air_lock_door_open_ = false;
  turns_ = 0;

  // Create rooms
  CreateRooms();

  // Create action handlers
  CreateActions();

  // Create items
  CreateItems();
}

void Captive::CreateRooms() {
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
  rooms() = {
      new Room("Prison cell", {-2, 0, 0, 0}),
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
      new Room("Air lock", {-21, 0, 0, 18}),
      new Room("Outside of ship", {0, 20, 0, 0}),
  };
}

void Captive::CreateActions() {
  handlers()["get_handler"] =
      new ActionHandler(Action::kGet, this, [](Game* game, Item* item) {
        if (game->inventory().HasItem(item)) {
          std::cout << "You already have " << item->name() << "." << std::endl;
        } else if (!game->CurrentRoom()->HasItem(item)) {
          std::cout << "I don't see " << item->name() << " here." << std::endl;
        } else {
          Item* gloves;
          if (item->name() == "ice block" &&
              (!(gloves = game->inventory().GetItem("gloves")) ||
               !gloves->HasState(ItemState::kWorn))) {
            std::cout << "It is too cold to carry." << std::endl;
          } else {
            game->CurrentRoom()->RemoveItem(item);
            game->inventory().PutItem(item);
            std::cout << "OK." << std::endl;
          }
        }
      });

  handlers()["drop_handler"] =
      new ActionHandler(Action::kDrop, this, [](Game* game, Item* item) {
        if (!game->inventory().HasItem(item)) {
          std::cout << "You don't have " << item->name() << "." << std::endl;
        } else {
          std::string name = item->name();
          Room* room = game->CurrentRoom();
          if (name == "ice block" &&
              room->name() == "Signal transmitter room") {
            Captive* captive = dynamic_cast<Captive*>(game);
            Room* rr = captive->GetRoom("Repairs room");
            rr->RemoveItem("wire");
            Item* i = game->nowhere().GetItem("aerial");
            game->nowhere().RemoveItem(i);
            rr->PutItem(i);
            captive->transmitter_overheating_ = false;
            std::cout << "The transmitter has cooled down, but it does not "
                         "have an aerial."
                      << std::endl;
          } else if (name == "aerial" &&
                     room->name() == "Signal transmitter room") {
            game->inventory().RemoveItem(item);
            room->PutItem(item);
            std::cout << "The transmitter is fully operational." << std::endl;
          } else {
            game->CurrentRoom()->PutItem(item);
            game->inventory().RemoveItem(item);
            std::cout << "OK." << std::endl;
          }
        }
      });

  handlers()["throw_handler"] =
      new ActionHandler(Action::kThrow, this, [](Game* game, Item* item) {
        Item* hp = game->inventory().GetItem("headphones");
        Captive* captive = dynamic_cast<Captive*>(game);
        if (!game->inventory().HasItem(item)) {
          std::cout << "You don't have " << item->name() << "." << std::endl;
        } else if (item->name() != "grenade" || !captive->air_lock_door_open_) {
          std::cout << "That would not achieve anything." << std::endl;
        } else if (game->CurrentRoom()->name() != "Air lock") {
          std::cout << "That would not achieve anything." << std::endl;
        } else if (!hp || !hp->HasState(ItemState::kWorn)) {
          std::cout << "The noise from the explosion has burst your ear drums."
                    << "The shock of this has killed you." << std::endl;
          game->Over();
        } else {
          game->CurrentRoom()->EnableExit(Direction::kNorth);
          game->CurrentRoom()->RemoveItem(
              game->CurrentRoom()->GetItem("boulders"));
          game->inventory().RemoveItem(item);
          std::cout << "You have cleared a passage through the boulders."
                    << std::endl;
        }
      });

  handlers()["cut_handler"] =
      new ActionHandler(Action::kCut, this, [](Game* game, Item* item) {
        if (!game->inventory().HasItem(item)) {
          std::cout << "You don't have " << item->name() << "." << std::endl;
        } else if (item->name() != "rough metal") {
          std::cout << "You can't do that." << std::endl;
        } else if (game->CurrentRoom()->name() != "Locksmiths") {
          std::cout << "I see no place where it can be cut." << std::endl;
        } else {
          game->inventory().RemoveItem(item);
          item = game->nowhere().GetItem("shiny key");
          game->CurrentRoom()->PutItem(item);
          game->nowhere().RemoveItem(item);
          std::cout << "OK." << std::endl;
        }
      });

  handlers()["wear_handler"] =
      new ActionHandler(Action::kWear, this, [](Game* game, Item* item) {
        if (!game->inventory().HasItem(item)) {
          std::cout << "You don't have " << item->name() << "." << std::endl;
        } else if (item->name() != "headphones" && item->name() != "gloves") {
          std::cout << "You can't do that." << std::endl;
        } else {
          item->AddState(ItemState::kWorn);
          std::cout << "OK." << std::endl;
        }
      });

  handlers()["light_handler"] =
      new ActionHandler(Action::kLight, this, [](Game* game, Item* item) {
        if (!game->inventory().HasItem(item)) {
          std::cout << "You don't have " << item->name() << "." << std::endl;
        } else if (item->name() != "torch") {
          std::cout << "You can't do that." << std::endl;
        } else if (item->HasState(ItemState::kLit)) {
          std::cout << "It is already lit." << std::endl;
        } else {
          item->AddState(ItemState::kLit);
          std::cout << "OK." << std::endl;
        }
      });

  handlers()["kick_handler"] =
      new ActionHandler(Action::kKick, this, [](Game* game, Item* item) {
        if (item->name() != "door") {
          std::cout << "You can't do that." << std::endl;
        } else if (!game->CurrentRoom()->HasItem(item)) {
          std::cout << "I don't see it here." << std::endl;
        } else {
          if (game->CurrentRoom()->name() == "Prison cell") {
            game->CurrentRoom()->EnableExit(Direction::kNorth);
            game->CurrentRoom()->RemoveItem(item);
            std::cout
                << "The hinges were weak and the door has collapsed into a "
                << "pile of dust." << std::endl;
          }
        }
      });

  handlers()["ring_handler"] =
      new ActionHandler(Action::kRing, this, [](Game* game, Item* item) {
        if (item->name() != "bell") {
          std::cout << "You can't do that." << std::endl;
        } else if (!game->CurrentRoom()->HasItem(item)) {
          std::cout << "I don't see it here." << std::endl;
        } else {
          std::cout << "You have woken the dead who do not like you too much."
                    << std::endl;
          game->Over();
        }
      });

  handlers()["read_handler"] =
      new ActionHandler(Action::kRead, this, [](Game* game, Item* item) {
        if (item->name() != "scratches" && item->name() != "inscription") {
          std::cout << "You can't do that." << std::endl;
        } else if (!game->CurrentRoom()->HasItem(item)) {
          std::cout << "I don't see it here." << std::endl;
        } else {
          if (item->name() == "scratches") {
            std::cout << "A transmitted signal will allow a door from the "
                      << "'air lock'  to be opened." << std::endl;
          } else {
            item = game->inventory().GetItem("magnifier");
            if (item == nullptr) {
              std::cout << "The writing is too small to read." << std::endl;
            } else {
              std::cout << "The magic word is 'swarck'." << std::endl;
            }
          }
        }
      });

  handlers()["look_handler"] =
      new ActionHandler(Action::kLook, this, [](Game* game, Item* item) {
        if (item->name() != "hole" && item->name() != "window") {
          std::cout << "That would not achieve anything." << std::endl;
        } else if (!game->CurrentRoom()->HasItem(item)) {
          std::cout << "I don't see it here." << std::endl;
        } else {
          if (item->name() == "hole") {
            std::cout << "Something large has fallen through the hole and "
                      << "flattened you." << std::endl;
            game->Over();
          } else {
            std::cout << "A space ship can be seen outside.It is ready to "
                      << "take off." << std::endl;
          }
        }
      });

  handlers()["kill_handler"] =
      new ActionHandler(Action::kKill, this, [](Game* game, Item* item) {
        if (item->name() != "mud man") {
          std::cout << "You can't do that." << std::endl;
        } else if (!game->CurrentRoom()->HasItem(item)) {
          std::cout << "I don't see it here." << std::endl;
        } else {
          std::cout << "You have killed the mud-man." << std::endl;
          game->CurrentRoom()->RemoveItem(item);
        }
      });

  handlers()["say_handler"] =
      new ActionHandler(Action::kSay, this, [](Game* game, Item* item) {
        if (item->name() != "swarck") {
          std::cout << "That would not achieve anything." << std::endl;
        } else if (game->CurrentRoom()->name() != "Outside of ship") {
          std::cout << "Nothing happens." << std::endl;
        } else {
          std::cout << "You have materialised inside your ship which has "
                    << "immediately taken off." << std::endl;
          game->Over();
        }
      });

  handlers()["transmit_handler"] =
      new ActionHandler(Action::kTransmit, this, [](Game* game, Item* item) {
        Item* aerial = game->GetItem("aerial");
        Room* room = game->CurrentRoom();
        if (room->name() != "Signal transmitter room") {
          std::cout << "You can't do that." << std::endl;
        } else if (!room->HasItem(aerial)) {
          std::cout << "The transmitter is missing an aerial." << std::endl;
        } else {
          std::cout << "An entrance has appeared into the 'air lock'."
                    << std::endl;
          dynamic_cast<Captive*>(game)->air_lock_open_ = true;
        }
      });

  handlers()["open_handler"] =
      new ActionHandler(Action::kOpen, this, [](Game* game, Item* item) {
        if (item->name() != "locked door") {
          std::cout << "You can't do that." << std::endl;
        } else if (!game->CurrentRoom()->HasItem(item)) {
          std::cout << "I don't see it here." << std::endl;
        } else {
          if (!game->inventory().HasItem("shiny key")) {
            std::cout << "You have no key." << std::endl;
          } else {
            game->CurrentRoom()->RemoveItem(item);
            item = game->nowhere().GetItem("boulders");
            game->CurrentRoom()->PutItem(item);
            game->nowhere().RemoveItem(item);
            dynamic_cast<Captive*>(game)->air_lock_door_open_ = true;
            std::cout << "The door came away in your hands, but the exit is "
                      << "now blocked by boulders which had been behind the "
                      << "door." << std::endl;
          }
        }
      });
}

void Captive::CreateItems() {
  Item* item;
  ActionHandler* get_handler = handlers()["get_handler"];
  ActionHandler* drop_handler = handlers()["drop_handler"];
  ActionHandler* throw_handler = handlers()["throw_handler"];
  ActionHandler* cut_handler = handlers()["cut_handler"];
  ActionHandler* wear_handler = handlers()["wear_handler"];
  ActionHandler* light_handler = handlers()["light_handler"];
  ActionHandler* kick_handler = handlers()["kick_handler"];
  ActionHandler* ring_handler = handlers()["ring_handler"];
  ActionHandler* read_handler = handlers()["read_handler"];
  ActionHandler* look_handler = handlers()["look_handler"];
  ActionHandler* kill_handler = handlers()["kill_handler"];
  ActionHandler* say_handler = handlers()["say_handler"];
  ActionHandler* transmit_handler = handlers()["transmit_handler"];
  ActionHandler* open_handler = handlers()["open_handler"];

  item = new Item("nothing", {transmit_handler});

  // Grenade
  item = new Item("grenade", {get_handler, drop_handler, throw_handler});
  GetRoom("Gunpowder chamber")->PutItem(item);
  // Rough metal
  item = new Item("rough metal", {get_handler, drop_handler, cut_handler});
  GetRoom("Room of chains")->PutItem(item);
  // Shiny key
  item = new Item("shiny key", {get_handler, drop_handler});
  nowhere().PutItem(item);
  // Ice block
  item = new Item("ice block", {get_handler, drop_handler});
  GetRoom("Frozen room")->PutItem(item);
  // Gloves
  item = new Item("gloves", {get_handler, drop_handler, wear_handler});
  GetRoom("Place with a rocky floor")->PutItem(item);
  // sabre
  item = new Item("sabre", {get_handler, drop_handler});
  GetRoom("Altar")->PutItem(item);
  // aerial
  item = new Item("aerial", {get_handler, drop_handler});
  nowhere().PutItem(item);
  // Torch
  item = new Item("torch", {get_handler, drop_handler, light_handler});
  GetRoom("Winding staircase")->PutItem(item);
  // Headphones
  item = new Item("headphones", {get_handler, drop_handler, wear_handler});
  GetRoom("Padded cell")->PutItem(item);
  // Magnifier
  item = new Item("magnifier", {get_handler, drop_handler});
  GetRoom("Brightly coloured room")->PutItem(item);
  // Locked door
  item = new Item("locked door", {open_handler});
  GetRoom("Air lock")->PutItem(item);
  // Door
  item = new Item("door", {kick_handler});
  GetRoom("Prison cell")->PutItem(item);
  // Bell
  item = new Item("bell", {ring_handler});
  GetRoom("Bell tower")->PutItem(item);
  // Scratches
  item = new Item("scratches", {read_handler});
  GetRoom("Wall with scratches on it")->PutItem(item);
  // Key cutter
  item = new Item("key cutter", {});
  GetRoom("Locksmiths")->PutItem(item);
  // Hole
  item = new Item("hole", {look_handler});
  GetRoom("Area with a hole in the ceiling")->PutItem(item);
  // Transmitter
  item = new Item("transmitter", {});
  GetRoom("Signal transmitter room")->PutItem(item);
  // Window
  item = new Item("window", {look_handler});
  GetRoom("Observation point")->PutItem(item);
  // Mud man
  item = new Item("mud man", {kill_handler});
  GetRoom("Muddy area")->PutItem(item);
  // Wire
  item = new Item("wire", {});
  GetRoom("Repairs room")->PutItem(item);
  // Inscription
  item = new Item("inscription", {read_handler});
  GetRoom("Place beside a monolith")->PutItem(item);
  // Boulders
  item = new Item("boulders", {});
  nowhere().PutItem(item);
  // Swarck
  item = new Item("swarck", {say_handler});
  nowhere().PutItem(item);

  nothing().AddActionHandler(transmit_handler);
}

void Captive::AfterTurn() {
  turns_++;
  Room* room = CurrentRoom();
  std::string room_name = room->name();

  if (!running()) return;

  if (room_name == "Muddy area" && !inventory().HasItem("sabre") &&
      room->HasItem("mud man")) {
    std::cout << "A mud-man has just killed you." << std::endl;
    Over();
    return;
  }
  if (room_name == "Dimly lit passage") {
    Item* torch = inventory().GetItem("torch");
    if (!torch || !torch->HasState(ItemState::kLit)) {
      std::cout << "You have fallen into a hole in the dim light." << std::endl;
      Over();
      return;
    }
  }
  if (room_name == "Signal transmitter room" && transmitter_overheating_) {
    std::cout << "The transmitter is overheating." << std::endl;
  }
  if (turns_ >= 20 && turns_ < 40) {
    std::cout << "A rumbling sound can be heard." << std::endl;
  } else if (turns_ >= 40 && turns_ < 60) {
    std::cout << "The noise is becoming louder." << std::endl;
  } else if (turns_ >= 60 && turns_ < 80) {
    std::cout << "The ground is starting to shake." << std::endl;
  } else if (turns_ >= 80 && turns_ < 100) {
    std::cout << "I'd advise you to get out quickly." << std::endl;
  } else if (turns_ >= 100 && turns_ < 120) {
    std::cout << "The roof is caving in." << std::endl;
  } else if (turns_ >= 120) {
    std::cout << "The planet has blown up." << std::endl;
    Over();
  }
}