
#include <iostream>
#include "common.h"

std::ostream &operator<<(std::ostream &os, Direction const &d) {
  static const char* dirs[4] = { "North", "South", "East", "West" };
  return os << dirs[static_cast<int>(d)];
}