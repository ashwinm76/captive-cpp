# Captive: an adventure game
This is a game based on the type-in game Captive that appeared in the book
"Creating Adventure Programs on the BBC Micro" by Ian Watt, published in 1983.
Soft copies of this book can be found online. 

The original game is a text adventure where you are given descriptions of your
environment, and you type in commands. You are a captive of aliens on a planet
that is about to blow up, and you have to find a way to escape. You have a
limited number of turns to do this, before the planet blows up. You have an
inventory that can carry a limited number of items (4 in the original game).
Each room can only hold a limited number of items (4 in the original game).

# Map
This is the game map. Room numbers are in square brackets. The room numbers
were used in the original BASIC game, but are not used here. The contents
of a room are in parentheses.

```
                              +--------------+
                              |         [21] |
                              | Outside of   |
                              | Ship         |
                              |              |
                              |              |
                              |              |
               +--------------+-----|  |-----+--------------+
               |         [18] |         [20] |         [12] |
               | Observation  | Air Lock     | Altar        |
               | Point        - (LOCKED DOOR)| (SABRE)      |
               | (WINDOW)       (BOULDERS)   |              |
               |              -              |              |
               |              |              |              |
+--------------+------|  |----+--------------+-----|  |-----+--------------+
|         [19] |         [17] |         [14] |         [10] |         [13] |
| Repairs      | Brightly     | Dimly lit    | Area With    | Place        |
| Room         - Coloured     - Passage      - a Hole in    - Beside a     |
| (WIRE)         Room                          the Ceiling    Monolith     |
| (AERIAL)     - (MAGNIFIER)  -              - (HOLE)       - (INSCRIPTION)|
|              |              |              |              | (SWARCK)     |
+--------------+--------------+--------------+-----|  |-----+--------------+
               |          [9] |          [7] |          [8] |
               | Padded cell  | Signal       | Room of      |
               | (HEADPHONES) - Transmitter  - Chains       |
               |                Room           (ROUGH METAL)|
               |              - (TRANSMITTER)-              |
               |              |              |              |
+--------------+--------------+----|  |------+-----|  |-----+--------------+
|          [6] |          [4] |          [5] |         [11] |         [16] |
| Wall with    | Gunpowder    | Place with   | Muddy Area   | Frozen room  |
| Scratches    - Chamber      - a Rocky      | (MUD MAN)    - (ICE BLOCK)  |
| (SCRATCHES)    (GRENADE)      Floor        |                             |
|              -              - (GLOVES)     |              -              |
|              |              |              |              |              |
+--------------+-----|  |-----+--------------+-----|  |-----+--------------+
               |          [3] |          [2] |         [15] |
               | Winding      | Bell Tower   | Locksmiths   |
               | Staircase    - (BELL)       | (KEY CUTTER) |
               | (TORCH)                     | (SHINY KEY)  |
               |              -              |              |
               |              |              |              |
               +--------------+-----|  |-----+--------------+
                              |          [1] |
                              | Prison Cell  |
                              | (DOOR)       |
                              |              |
                              |              |
                              |              |
                              +--------------+
```
