#ifndef CAPTIVE_CAPTIVE_H_
#define CAPTIVE_CAPTIVE_H_

enum class Direction {
  kNone = -1,
  kNorth,
  kSouth,
  kEast,
  kWest
};

std::ostream &operator<<(std::ostream &os, Direction const &d);

#endif