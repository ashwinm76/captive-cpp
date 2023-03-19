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

#include <fstream>
#include <iostream>
#include <vector>

#include "captive.h"
#include "iotest.h"

int main(int argc, char **argv) {
  std::ifstream ifile;

  if (argc != 2) {
    std::cout << "No stimulus file passed" << std::endl;
    return 1;
  }

  ifile.open(argv[1]);
  std::vector<std::pair<std::string, std::vector<std::string> > > expects;
  std::pair<std::string, std::vector<std::string> > expect;
  if (ifile.is_open()) {
    std::string line;
    while (std::getline(ifile, line)) {
      if (line[0] != '#' && line.size() > 0) {
        if (line[0] == ':') {
          expect.second.push_back(line.substr(1));
        } else {
          if (expect.first != "") {
            expects.push_back(expect);
            expect.first = "";
            expect.second.clear();
          }
          expect.first = line;
        }
      }
    }
    if (expect.first != "") {
      expects.push_back(expect);
    }
    ifile.close();
  } else {
    std::cout << "Can't open " << argv[1] << std::endl;
    return 1;
  }

  IoTest io(expects);
  Captive captive(&io);

  captive.Run();
  return io.passed() ? 0 : 1;
}