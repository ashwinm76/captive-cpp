# Limits
- Inventory limit: 4 items
- Room capacity: 4 items

# Room names
1. Prison cell
2. Bell tower
3. Winding staircase
4. Gunpowder chamber
5. Place with a rocky floor
6. Wall with scratches on it
7. Signal transmitter room
8. Room of chains
9. Padded cell
10. Area with a hole in the ceiling
11. Muddy area
12. Altar
13. Place beside a monolith
14. Dimly lit passage
15. Locksmiths
16. Frozen room
17. Brightly coloured room
18. Observation point
19. Repairs room
20. Air lock
21. Outside of ship

# Map
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

# Objects
- Grenade: initial room 4
  - get, drop
  - throw 
    - if (room != 20)
        say("That would not achieve anything.")
    - if (!wearing(headphones))
        say("The noise from the explosion has burst your ear drums.The shock of this has killed you.")
      else
        remove grenade, boulders
        say("You have cleared a passage through the boulders.")
- Rough metal: initial room 8
  - get, drop
  - cut
    - if (!room == 15)
        say("I see no place where it can be cut.")
      else
        remove rough metal
        place_in_room shiny key
        say("The piece of metal has been cut into a key.")
- Shiny key
  - get, drop
- Ice block: initial room 16
  - get
    - if (!wearing(gloves))
        say("It is too cold to carry.")
  - drop
- Gloves: initial room 5
  - get, drop
  - wear
- Sabre: initial room 12
  - get, drop
- Aerial
  - get, drop
- Torch: initial room 3
  - get, drop
  - light
- Headphones: initial room 9
  - get, drop
  - wear
- Magnifier: initial room 17
  - get, drop
- Locked door: room 20
  - open
    - if (!have(shiny key))
        say("I have no key.")
      else
        remove locked door
        place_in_room boulders
        say("The door came away in your hands,but the exit is now blocked by boulders which had been behind the door.")
- Door: room 1
  - kick
    - remove door
      say("The hinges were weak and the door has collapsed into a pile of dust.")
- Bell: room 2
  - ring
    - say("You have woken the dead who do not like you too much.")
      game_over
- Scratches: room 6
  - read
    - say("A transmitted signal will allow a door from the 'air lock'  to be opened.")
- Key cutter: room 15
- Hole: room 10
  - look
    - say("Something large has fallen through the hole and flattened you.")
      game_over
- Transmitter: room 7
- Window: room 18
  - look
    - say("A space ship can be seen outside.It is ready to take off.")
- Mud Man: room 11
  - kill
    - say("You have killed the mud-man.")
- Wire: room 19
- Inscription: room 13
  - read
    - if (!have(magnifier))
        say("The writing is too small to read.")
      else
        say("The magic word is 'swarck'.")
- Boulders
- Swarck
  - say
    if (room != 21)
      say("Nothing happens.")
    else
      say("You have materialised inside your ship which has immediately taken off.")  
      game_success

# Actions
- Transmit
  - if (room != 7 || !room_contains(aerial))
      say("I cant do that.")
    else
      set_state(air_lock_open, true)
      say("An entrance has appeared into the 'air lock'.")
- N
  - if (room_contains(door))
      say("The door is in the way.")
    else if(room_contains(locked door) || room_contains(boulders))
      say("Your exit is blocked.")
    else if (room_contains(mud man))
      say("You cannot pass the mud-man.")
    else
      move_n
- S
  - if (room_contains(mud man))
      say("You cannot pass the mud-man.")
    else 
      move_s
- E
  - if (room_contains(mud man))
      say("You cannot pass the mud-man.")
    else if (room == 18 && !get_state(air_lock_opened))
      say("You cannot pass into the 'air lock'.")
    else 
      move_e
- W
  - move_w

# Actions summary
- throw (grenade, only room 20)
- cut (rough metal, only room 15)
- wear (gloves, headphones)
- light (torch)
- kick (door)
- ring (bell)
- read (scratches, inscription)
- look (hole, window)
- kill (mud man)
- say (swarck)
- transmit (only room 7 & Aerial in room 7)