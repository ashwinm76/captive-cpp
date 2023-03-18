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

  StartingRoom(prison_cell);
}

void Captive::CreateActions() {
  handlers()["get_handler"] =
      new ActionHandler(Action::kGet, this, [](Game* game, Item* item) {
        if (game->DontHaveItem(item) && game->ItemInCurrentRoom(item)) {
          Item* gloves;
          if (item->Is("ice block") &&
              (!(gloves = game->inventory().GetItem("gloves")) ||
               !gloves->HasState(ItemState::kWorn))) {
            game->io()->WriteResponse("It is too cold to carry.");
          } else {
            game->CurrentRoom()->MoveItem(item, &game->inventory());
            game->io()->WriteResponse("OK.");
          }
        }
      });

  handlers()["drop_handler"] =
      new ActionHandler(Action::kDrop, this, [](Game* game, Item* item) {
        if (game->HaveItem(item)) {
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
          } else if (item->Is("aerial") &&
                     room->Is("Signal transmitter room")) {
            game->inventory().MoveItem(item, room);
            game->io()->WriteResponse("The transmitter is fully operational.");
          } else {
            game->inventory().MoveItem(item, game->CurrentRoom());
            game->io()->WriteResponse("OK.");
          }
        }
      });

  handlers()["throw_handler"] =
      new ActionHandler(Action::kThrow, this, [](Game* game, Item* item) {
        Item* hp = game->inventory().GetItem("headphones");
        if (game->HaveItem(item)) {
          // Only throw the grenade at the boulders
          if (!item->Is("grenade") ||
              !game->CurrentRoom()->HasItem("boulders")) {
            game->io()->WriteResponse("That would not achieve anything.");
            return;
          }
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
        }
      });

  handlers()["cut_handler"] =
      new ActionHandler(Action::kCut, this, [](Game* game, Item* item) {
        if (game->HaveItem(item)) {
          if (!game->CurrentRoom()->Is("Locksmiths")) {
            game->io()->WriteResponse("I see no place where it can be cut.");
          } else {
            game->inventory().RemoveItem(item);
            game->nowhere().MoveItem("shiny key", game->CurrentRoom());
            game->io()->WriteResponse("OK.");
          }
        }
      });

  handlers()["wear_handler"] =
      new ActionHandler(Action::kWear, this, [](Game* game, Item* item) {
        if (game->HaveItem(item)) {
          item->AddState(ItemState::kWorn);
          game->io()->WriteResponse("OK.");
        }
      });

  handlers()["light_handler"] =
      new ActionHandler(Action::kLight, this, [](Game* game, Item* item) {
        if (game->HaveItem(item)) {
          if (item->HasState(ItemState::kLit)) {
            game->io()->WriteResponse("It is already lit.");
          } else {
            item->AddState(ItemState::kLit);
            game->io()->WriteResponse("OK.");
          }
        }
      });

  handlers()["kick_handler"] =
      new ActionHandler(Action::kKick, this, [](Game* game, Item* item) {
        if (game->ItemInCurrentRoom(item)) {
          if (game->CurrentRoom()->Is("Prison cell")) {
            game->CurrentRoom()->EnableExit(Direction::kNorth);
            game->CurrentRoom()->RemoveItem(item);
            game->io()->WriteResponse(
                "The hinges were weak and the door has collapsed into a "
                "pile of dust.");
          } else {
            game->io()->WriteResponse("Nothing happens.");
          }
        }
      });

  handlers()["ring_handler"] =
      new ActionHandler(Action::kRing, this, [](Game* game, Item* item) {
        if (game->ItemInCurrentRoom(item)) {
          game->io()->WriteResponse(
              "You have woken the dead who do not like "
              "you too much.");
          game->Over();
        }
      });

  handlers()["read_handler"] =
      new ActionHandler(Action::kRead, this, [](Game* game, Item* item) {
        if (game->ItemInCurrentRoom(item)) {
          if (item->Is("scratches")) {
            game->io()->WriteResponse(
                "A transmitted signal will allow a door"
                " from the 'air lock'  to be opened.");
          } else {
            item = game->inventory().GetItem("magnifier");
            if (item == nullptr) {
              game->io()->WriteResponse("The writing is too small to read.");
            } else {
              game->io()->WriteResponse("The magic word is 'swarck'.");
            }
          }
        }
      });

  handlers()["look_handler"] =
      new ActionHandler(Action::kLook, this, [](Game* game, Item* item) {
        if (game->ItemInCurrentRoom(item)) {
          if (!item->Is("hole") && !item->Is("window")) {
            game->io()->WriteResponse("That would not achieve anything.");
          } else {
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
          }
        }
      });

  handlers()["kill_handler"] =
      new ActionHandler(Action::kKill, this, [](Game* game, Item* item) {
        if (game->ItemInCurrentRoom(item)) {
          game->io()->WriteResponse("You have killed the mud-man.");
          game->CurrentRoom()->RemoveItem(item);
        }
      });

  handlers()["say_handler"] =
      new ActionHandler(Action::kSay, this, [](Game* game, Item* item) {
        if (!item->Is("swarck")) {
          game->io()->WriteResponse("That would not achieve anything.");
        } else if (!game->CurrentRoom()->Is("Outside of ship")) {
          game->io()->WriteResponse("Nothing happens.");
        } else {
          game->io()->WriteResponse(
              "You have materialised inside your ship"
              " which has immediately taken off.");
          game->Over();
        }
      });

  handlers()["transmit_handler"] =
      new ActionHandler(Action::kTransmit, this, [](Game* game, Item* item) {
        Item* aerial = game->GetItem("aerial");
        Room* room = game->CurrentRoom();
        if (!room->Is("Signal transmitter room")) {
          game->io()->WriteResponse("There is no transmitter here.");
        } else if (!room->HasItem(aerial)) {
          game->io()->WriteResponse("The transmitter is missing an aerial.");
        } else {
          game->io()->WriteResponse(
              "An entrance has appeared into the 'air lock'.");
          dynamic_cast<Captive*>(game)->air_lock_open_ = true;
        }
      });

  handlers()["open_handler"] =
      new ActionHandler(Action::kOpen, this, [](Game* game, Item* item) {
        if (game->ItemInCurrentRoom(item)) {
          if (item->Is("door")) {
            game->io()->WriteResponse("It is stuck tight.");
          } else if (!dynamic_cast<Captive*>(game)->air_lock_open_) {
            game->io()->WriteResponse("The air lock is disabled.");
          } else if (!game->inventory().HasItem("shiny key")) {
            game->io()->WriteResponse("You have no key.");
          } else {
            game->CurrentRoom()->RemoveItem(item);
            game->nowhere().MoveItem("boulders", game->CurrentRoom());
            game->io()->WriteResponse(
                "The door came away in your hands, but the exit is "
                "now blocked by boulders which had been behind the "
                "door.");
          }
        }
      });
}

void Captive::CreateItems() {
  // Dummy item for the transmit action
  nothing() = *MakeItem("nothing", "", {Action::kTransmit});

  // Grenade
  MakeItem("grenade", "Gunpowder chamber",
           {Action::kGet, Action::kDrop, Action::kThrow});
  // Rough metal
  MakeItem("rough metal", "Room of chains",
           {Action::kGet, Action::kDrop, Action::kCut});
  // Shiny key
  MakeItem("shiny key", "Nowhere", {Action::kGet, Action::kDrop});
  // Ice block
  MakeItem("ice block", "Frozen room", {Action::kGet, Action::kDrop});
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
  MakeItem("locked door", "Air lock", {Action::kOpen});
  // Door
  MakeItem("door", "Prison cell", {Action::kOpen, Action::kKick});
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
  MakeItem("inscription", "Place beside a monolith", {Action::kRead});
  // Boulders
  MakeItem("boulders", "Nowhere");
  // Swarck
  MakeItem("swarck", "Nowhere", {Action::kSay});
}

void Captive::AfterTurn() {
  turns_++;

  if (!running()) return;

  if (CurrentRoom()->Is("Muddy area") && !inventory().HasItem("sabre") &&
      CurrentRoom()->HasItem("mud man")) {
    io()->WriteResponse("A mud-man has just killed you.");
    Over();
    return;
  }
  if (CurrentRoom()->Is("Dimly lit passage")) {
    Item* torch = inventory().GetItem("torch");
    if (!torch || !torch->HasState(ItemState::kLit)) {
      io()->WriteResponse("You have fallen into a hole in the dim light.");
      Over();
      return;
    }
  }
  if (CurrentRoom()->Is("Signal transmitter room") &&
      transmitter_overheating_) {
    io()->WriteStatus("The transmitter is overheating.");
  }
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