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

#include "captive.h"

#include "common.h"

Captive::Captive(Io* io) : Game(io) {
  transmitter_overheating_ = true;
  air_lock_open_ = false;
  turns_ = 0;
}

void Captive::Run() {
  // Print instructions
  io()->WriteInstructions(
      "You have been captured by creatures on an uncharted planet."
      " Unfortunately the planet happens to be unstable, and has been"
      " evacuated."
      " You therefore have to escape before the planet blows up with you on it."
      " The computer has a fairly large number of commands, so if one command"
      " does not work then try another."
      " The first three letters of each command and object need be typed"
      " in, although, if desired, the full word may be entered.");
  Game::Run();
}

void Captive::CreateRooms() {
  // Make rooms
  Room* prison_cell = MakeRoom("Prison cell");
  Room* bell_tower = MakeRoom("Bell tower");
  Room* winding_staircase = MakeRoom("Winding staircase");
  Room* gunpowder_chamber = MakeRoom("Gunpowder chamber");
  Room* rocky_floor_room = MakeRoom("Place with a rocky floor");
  Room* scratches_room = MakeRoom("Wall with scratches on it");
  Room* transmitter_room = MakeRoom("Signal transmitter room");
  Room* chains_room = MakeRoom("Room of chains");
  Room* padded_cell = MakeRoom("Padded cell");
  Room* hole_room = MakeRoom("Area with a hole in the ceiling");
  Room* muddy_area = MakeRoom("Muddy area");
  Room* altar = MakeRoom("Altar");
  Room* monolith_room = MakeRoom("Place beside a monolith");
  Room* dim_passage = MakeRoom("Dimly lit passage");
  Room* locksmiths = MakeRoom("Locksmiths");
  Room* frozen_room = MakeRoom("Frozen room");
  Room* bright_room = MakeRoom("Brightly coloured room");
  Room* observation_room = MakeRoom("Observation point");
  Room* repairs_room = MakeRoom("Repairs room");
  Room* air_lock = MakeRoom("Air lock");
  Room* outside = MakeRoom("Outside of ship");

  // Connect rooms
  prison_cell->ConnectRoom(bell_tower, Direction::kNorth, false);
  bell_tower->ConnectRoom(winding_staircase, Direction::kWest);
  winding_staircase->ConnectRoom(gunpowder_chamber, Direction::kNorth);
  gunpowder_chamber->ConnectRoom(scratches_room, Direction::kWest);
  gunpowder_chamber->ConnectRoom(rocky_floor_room, Direction::kEast);
  rocky_floor_room->ConnectRoom(transmitter_room, Direction::kNorth);
  transmitter_room->ConnectRoom(padded_cell, Direction::kWest);
  transmitter_room->ConnectRoom(chains_room, Direction::kEast);
  chains_room->ConnectRoom(muddy_area, Direction::kSouth);
  chains_room->ConnectRoom(hole_room, Direction::kNorth);
  muddy_area->ConnectRoom(frozen_room, Direction::kEast);
  muddy_area->ConnectRoom(locksmiths, Direction::kSouth);
  hole_room->ConnectRoom(monolith_room, Direction::kEast);
  hole_room->ConnectRoom(altar, Direction::kNorth);
  hole_room->ConnectRoom(dim_passage, Direction::kWest);
  dim_passage->ConnectRoom(bright_room, Direction::kWest);
  bright_room->ConnectRoom(repairs_room, Direction::kWest);
  bright_room->ConnectRoom(observation_room, Direction::kNorth);
  observation_room->ConnectRoom(air_lock, Direction::kEast);
  air_lock->ConnectRoom(outside, Direction::kNorth, false);

  // Add room event handlers
  auto fn = [](Game* game, Room* room) {
    Captive* captive = dynamic_cast<Captive*>(game);
    if (captive->transmitter_overheating()) {
      game->io()->WriteStatus("The transmitter is overheating.");
    }
  };
  transmitter_room->OnTurn(fn);
  transmitter_room->OnEnter(fn);

  muddy_area->OnEnter([](Game* game, Room* room) {
    if (!game->inventory().HasItem("sabre") && room->HasItem("mud man")) {
      game->io()->WriteResponse("A mud-man has just killed you.");
      game->Over();
      return;
    }
  });

  dim_passage->OnEnter([](Game* game, Room* room) {
    Item* torch = game->inventory().GetItem("torch");
    if (!torch || !torch->HasState(ItemState::kLit)) {
      game->io()->WriteResponse(
          "You have fallen into a hole in the dim light.");
      game->Over();
      return;
    }
  });

  StartingRoom(prison_cell);
}

void Captive::CreateActions() {
  auto is_in_inventory = ActionCondition(
      "You don't have it.", this,
      [](Game* game, Item* item) { return game->inventory().HasItem(item); });

  auto dont_have_item = ActionCondition(
      "You already have it.", this,
      [](Game* game, Item* item) { return !game->inventory().HasItem(item); });

  auto have_item = ActionCondition(
      "You don't have it.", this,
      [](Game* game, Item* item) { return game->inventory().HasItem(item); });

  auto item_in_room =
      ActionCondition("I don't see it here.", this, [](Game* game, Item* item) {
        return game->CurrentRoom()->HasItem(item);
      });

  handlers()["get_handler"] =
      new ActionHandler(Action::kGet, this, [](Game* game, Item* item) {
        game->CurrentRoom()->MoveItem(item, &game->inventory());
        game->io()->WriteResponse("OK.");
      });
  handlers()["get_handler"]->AddCondition(dont_have_item);
  handlers()["get_handler"]->AddCondition(item_in_room);

  handlers()["drop_handler"] =
      new ActionHandler(Action::kDrop, this, [](Game* game, Item* item) {
        Room* room = game->CurrentRoom();
        if (item->Is("ice block") && room->Is("Signal transmitter room")) {
          Captive* captive = dynamic_cast<Captive*>(game);
          Room* rr = captive->GetRoom("Repairs room");
          rr->RemoveItem("wire");
          game->nowhere().MoveItem("aerial", rr);
          captive->transmitter_overheating_ = false;
          game->io()->WriteResponse(
              "The transmitter has cooled down, but it"
              " does not have an aerial.");
        } else if (item->Is("aerial") && room->Is("Signal transmitter room")) {
          game->inventory().MoveItem(item, room);
          game->io()->WriteResponse("The transmitter is fully operational.");
        } else {
          game->inventory().MoveItem(item, game->CurrentRoom());
          game->io()->WriteResponse("OK.");
        }
      });
  handlers()["drop_handler"]->AddCondition(have_item);

  handlers()["throw_handler"] =
      new ActionHandler(Action::kThrow, this, [](Game* game, Item* item) {
        Item* hp = game->inventory().GetItem("headphones");
        // Check if you're wearing headphones when throwing the grenade
        if (!hp || !hp->HasState(ItemState::kWorn)) {
          game->io()->WriteResponse(
              "The noise from the explosion has burst"
              " your ear drums. The shock of this has killed you.");
          game->Over();
          return;
        }
        game->CurrentRoom()->EnableExit(Direction::kNorth);
        game->CurrentRoom()->RemoveItem("boulders");
        game->inventory().RemoveItem(item);
        game->io()->WriteResponse(
            "You have cleared a passage through the boulders.");
      });
  handlers()["throw_handler"]->AddCondition(have_item);

  handlers()["cut_handler"] =
      new ActionHandler(Action::kCut, this, [](Game* game, Item* item) {
        game->inventory().RemoveItem(item);
        game->nowhere().MoveItem("shiny key", game->CurrentRoom());
        game->io()->WriteResponse("OK.");
      });
  handlers()["cut_handler"]->AddCondition(is_in_inventory);

  handlers()["wear_handler"] =
      new ActionHandler(Action::kWear, this, [](Game* game, Item* item) {
        item->AddState(ItemState::kWorn);
        game->io()->WriteResponse("OK.");
      });
  handlers()["wear_handler"]->AddCondition(have_item);

  handlers()["light_handler"] =
      new ActionHandler(Action::kLight, this, [](Game* game, Item* item) {
        if (item->HasState(ItemState::kLit)) {
          game->io()->WriteResponse("It is already lit.");
        } else {
          item->AddState(ItemState::kLit);
          game->io()->WriteResponse("OK.");
        }
      });
  handlers()["light_handler"]->AddCondition(have_item);

  handlers()["kick_handler"] =
      new ActionHandler(Action::kKick, this, [](Game* game, Item* item) {
        game->CurrentRoom()->EnableExit(Direction::kNorth);
        game->CurrentRoom()->RemoveItem(item);
        game->io()->WriteResponse(
            "The hinges were weak and the door has collapsed into a "
            "pile of dust.");
      });
  handlers()["kick_handler"]->AddCondition(item_in_room);

  handlers()["ring_handler"] =
      new ActionHandler(Action::kRing, this, [](Game* game, Item* item) {
        game->io()->WriteResponse(
            "You have woken the dead who do not like "
            "you too much.");
        game->Over();
      });
  handlers()["ring_handler"]->AddCondition(item_in_room);

  handlers()["read_handler"] =
      new ActionHandler(Action::kRead, this, [](Game* game, Item* item) {
        if (item->Is("scratches")) {
          game->io()->WriteResponse(
              "A transmitted signal will allow a door"
              " from the 'air lock'  to be opened.");
        } else {
          game->io()->WriteResponse("The magic word is 'swarck'.");
        }
      });
  handlers()["read_handler"]->AddCondition(item_in_room);

  handlers()["look_handler"] =
      new ActionHandler(Action::kLook, this, [](Game* game, Item* item) {
        if (item->Is("hole")) {
          game->io()->WriteResponse(
              "Something large has fallen through the"
              " hole and flattened you.");
          game->Over();
        } else {
          game->io()->WriteResponse(
              "A space ship can be seen outside.It is"
              " ready to take off.");
        }
      });
  handlers()["look_handler"]->AddCondition(item_in_room);

  handlers()["kill_handler"] =
      new ActionHandler(Action::kKill, this, [](Game* game, Item* item) {
        game->io()->WriteResponse("You have killed the mud-man.");
        game->CurrentRoom()->RemoveItem(item);
      });
  handlers()["kill_handler"]->AddCondition(item_in_room);

  handlers()["say_handler"] =
      new ActionHandler(Action::kSay, this, [](Game* game, Item* item) {
        game->io()->WriteResponse(
            "You have materialised inside your ship"
            " which has immediately taken off.");
        game->Over();
      });

  handlers()["transmit_handler"] =
      new ActionHandler(Action::kTransmit, this, [](Game* game, Item* item) {
        game->io()->WriteResponse(
            "An entrance has appeared into the 'air lock'.");
        dynamic_cast<Captive*>(game)->air_lock_open_ = true;
      });

  handlers()["open_handler"] =
      new ActionHandler(Action::kOpen, this, [](Game* game, Item* item) {
        game->CurrentRoom()->RemoveItem(item);
        game->nowhere().MoveItem("boulders", game->CurrentRoom());
        game->io()->WriteResponse(
            "The door came away in your hands, but the exit is "
            "now blocked by boulders which had been behind the "
            "door.");
      });
  handlers()["open_handler"]->AddCondition(item_in_room);
}

void Captive::CreateItems() {
  // Dummy item for the transmit action
  nothing() = *MakeItem("nothing", "", {Action::kTransmit});
  // Only allow transmission in the transmitter room, when the aerial has
  // been fitted.
  nothing().AddCondition(
      Action::kTransmit,
      ActionCondition(
          "There is no transmitter here.", this, [](Game* game, Item* item) {
            return game->CurrentRoom()->Is("Signal transmitter room");
          }));
  nothing().AddCondition(
      Action::kTransmit,
      ActionCondition("The transmitter is missing an aerial.", this,
                      [](Game* game, Item* item) {
                        return game->CurrentRoom()->HasItem("aerial");
                      }));

  // Grenade
  Item* grenade = MakeItem("grenade", "Gunpowder chamber",
                           {Action::kGet, Action::kDrop, Action::kThrow});
  // Only throw the grenade at the boulders
  grenade->AddCondition(
      Action::kThrow,
      ActionCondition("That would not achieve anything.", this,
                      [](Game* game, Item* item) {
                        return game->CurrentRoom()->HasItem("boulders");
                      }));

  // Rough metal
  Item* rough_metal = MakeItem("rough metal", "Room of chains",
                               {Action::kGet, Action::kDrop, Action::kCut});
  // Only cut the rough metal at the locksmiths
  rough_metal->AddCondition(
      Action::kCut,
      ActionCondition("I see no place where it can be cut.", this,
                      [](Game* game, Item* item) {
                        return game->CurrentRoom()->Is("Locksmiths");
                      }));

  // Shiny key
  MakeItem("shiny key", "Nowhere", {Action::kGet, Action::kDrop});

  // Ice block
  Item* ice_block =
      MakeItem("ice block", "Frozen room", {Action::kGet, Action::kDrop});
  // Only carry the ice block if you're wearing gloves
  ice_block->AddCondition(
      Action::kGet,
      ActionCondition("It is too cold to carry.", this,
                      [](Game* game, Item* item) {
                        Item* gloves;
                        return (gloves = game->inventory().GetItem("gloves")) &&
                               gloves->HasState(ItemState::kWorn);
                      }));
  // Gloves
  MakeItem("gloves", "Place with a rocky floor",
           {Action::kGet, Action::kDrop, Action::kWear});

  // Sabre
  MakeItem("sabre", "Altar", {Action::kGet, Action::kDrop});

  // Aerial
  MakeItem("aerial", "Nowhere", {Action::kGet, Action::kDrop});

  // Torch
  MakeItem("torch", "Winding staircase",
           {Action::kGet, Action::kDrop, Action::kLight});

  // Headphones
  MakeItem("headphones", "Padded cell",
           {Action::kGet, Action::kDrop, Action::kWear});

  // Magnifier
  MakeItem("magnifier", "Brightly coloured room",
           {Action::kGet, Action::kDrop});

  // Locked door
  Item* locked_door = MakeItem("locked door", "Air lock", {Action::kOpen});
  // The locked door can't be opened without a key or if the airlock is closed.
  locked_door->AddCondition(
      Action::kOpen,
      ActionCondition("The air lock is disabled.", this,
                      [](Game* game, Item* item) {
                        return dynamic_cast<Captive*>(game)->air_lock_open_;
                      }));
  locked_door->AddCondition(
      Action::kOpen,
      ActionCondition("You have no key.", this, [](Game* game, Item* item) {
        return game->inventory().HasItem("shiny key");
      }));

  // Door
  Item* door = MakeItem("door", "Prison cell", {Action::kOpen, Action::kKick});
  // The door is stuck and cannot be opened.
  door->AddCondition(Action::kOpen, ActionCondition("It is stuck tight.", this,
                                                    [](Game* game, Item* item) {
                                                      return false;
                                                    }));

  // Bell
  MakeItem("bell", "Bell tower", {Action::kRing});

  // Scratches
  MakeItem("scratches", "Wall with scratches on it", {Action::kRead});

  // Key cutter
  MakeItem("key cutter", "Locksmiths");

  // Hole
  MakeItem("hole", "Area with a hole in the ceiling", {Action::kLook});

  // Transmitter
  MakeItem("transmitter", "Signal transmitter room");

  // Window
  MakeItem("window", "Observation point", {Action::kLook});

  // Mud man
  MakeItem("mud man", "Muddy area", {Action::kKill});

  // Wire
  MakeItem("wire", "Repairs room");

  // Inscription
  Item* inscription =
      MakeItem("inscription", "Place beside a monolith", {Action::kRead});
  // Only read the inscription of you have the magnifier.
  inscription->AddCondition(
      Action::kRead, ActionCondition("The writing is too small to read.", this,
                                     [](Game* game, Item* item) {
                                       return game->inventory().GetItem(
                                                  "magnifier") != nullptr;
                                     }));

  // Boulders
  MakeItem("boulders", "Nowhere");

  // Swarck
  Item* swarck = MakeItem("swarck", "Nowhere", {Action::kSay});
  // Saying swarck only works outside the ship
  swarck->AddCondition(
      Action::kSay,
      ActionCondition("Nothing happens.", this, [](Game* game, Item* item) {
        return game->CurrentRoom()->Is("Outside of ship");
      }));
}

void Captive::AfterTurn() {
  turns_++;

  if (!running()) return;

  if (turns_ >= 20 && turns_ < 40) {
    io()->WriteStatus("A rumbling sound can be heard.");
  } else if (turns_ >= 40 && turns_ < 60) {
    io()->WriteStatus("The noise is becoming louder.");
  } else if (turns_ >= 60 && turns_ < 80) {
    io()->WriteStatus("The ground is starting to shake.");
  } else if (turns_ >= 80 && turns_ < 100) {
    io()->WriteStatus("I'd advise you to get out quickly.");
  } else if (turns_ >= 100 && turns_ < 120) {
    io()->WriteStatus("The roof is caving in.");
  } else if (turns_ >= 120) {
    io()->WriteStatus("The planet has blown up.");
    Over();
  }
}