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

#ifndef CAPTIVE_SRC_IOTEST_H
#define CAPTIVE_SRC_IOTEST_H

#include <vector>

#include "iostd.h"

class IoTest : public IoStd {
 public:
  IoTest(std::vector<std::string> inputs, std::vector<std::string> outputs)
      : IoStd(80),
        passed_(true),
        inputs_(inputs),
        outputs_(outputs),
        in_count_(0) {}

  bool passed() { return passed_; }

  void WriteResponse(const std::string& s) {
    std::cout << s << std::endl;
    if (outputs_[in_count_-1] != "*" && passed_)
      passed_ = (s == outputs_[in_count_-1]);
  }

  void WriteResponse(const std::list<const std::string>& m) {
    for (auto s : m) {
      std::cout << s;
      if (outputs_[in_count_-1] != "*" && passed_)
        passed_ = (s == outputs_[in_count_-1]);
    }
    std::cout << std::endl;
  }

  std::string ReadCommand(const std::string& prompt) {
    std::string input;

    std::cout << std::endl << prompt;
    if (in_count_ < inputs_.size()) {
      input = inputs_[in_count_++];
    } else {
      input = "quit";
    }
    std::cout << input << std::endl;
    return input;
  }

 private:
  bool passed_;
  std::vector<std::string> inputs_;
  std::vector<std::string> outputs_;
  int in_count_;
};

#endif
