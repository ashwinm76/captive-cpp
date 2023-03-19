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

class ScoreboardItem {
 public:
  ScoreboardItem(const std::string& input)
      : has_wildcard_(false), input_(input) {}
  void Add(const std::string& s) { items_.push_back(s); }
  void AddWildcard() { has_wildcard_ = true; }
  bool Empty() {
    return items_.size() == 0 && extras_.size() == 0;
  }
  void Check(const std::string& s) {
    auto f = std::find(items_.begin(), items_.end(), s);
    std::cout << "Check: " << s << std::endl;
    if (f != items_.end()) {
      items_.erase(f);
      std::cout << "Check: ERASE" << std::endl;
    } else if (!has_wildcard_) {
      extras_.push_back(s);
      std::cout << "Check: EXTRA" << std::endl;
    }
  }
  std::string input() { return input_; }
  std::string expects() {
    std::stringstream s;
    for (auto i : items_) s << i << ":";
    return s.str();
  }
  std::string extras() {
    std::stringstream s;
    for (auto i : extras_) s << i << ":";
    return s.str();
  }

 private:
  std::vector<std::string> items_;
  std::vector<std::string> extras_;
  bool has_wildcard_;
  std::string input_;
};

std::ostream& operator<<(std::ostream& os, ScoreboardItem s) {
  return os << "[I:]" << s.input() << "[O:]" << s.expects() << "[X:]"
            << s.extras() << std::endl;
}

class IoTest : public IoStd {
 public:
  IoTest(
      std::vector<std::pair<std::string, std::vector<std::string> > > expects)
      : IoStd(80), in_count_(0) {
    for (auto e : expects) {
      auto s = ScoreboardItem(e.first);
      for (auto o : e.second) {
        if (o == "*")
          s.AddWildcard();
        else
          s.Add(o);
      }
      scoreboard_.push_back(s);
    }
  }

  bool passed() {
    bool passed = true;
    for (auto s : scoreboard_) {
      if (!s.Empty()) {
        passed = false;
        std::cout << "[SCOREBOARD ITEM]" << s << std::endl;
      }
    }
    return passed;
  }

  void WriteStatus(const std::string& s) {
    std::cout << "?" << s << std::endl;
    scoreboard_[in_count_ - 1].Check(s);
  }

  void WriteResponse(const std::string& s) {
    std::cout << ":" << s << std::endl;
    scoreboard_[in_count_ - 1].Check(s);
  }

  void WriteResponse(const std::list<const std::string>& m) {
    std::string r;
    for (auto s : m) {
      r += s;
    }
    std::cout << ":" << r << std::endl;
    scoreboard_[in_count_ - 1].Check(r);
  }

  std::string ReadCommand(const std::string& prompt) {
    std::string input;

    std::cout << std::endl << prompt;
    if (in_count_ < scoreboard_.size()) {
      input = scoreboard_[in_count_++].input();
    } else {
      input = "quit";
    }
    std::cout << input << std::endl;
    return input;
  }

 private:
  int in_count_;
  std::vector<ScoreboardItem> scoreboard_;
};

#endif
