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
  std::vector<std::string> input;
  std::vector<std::string> output;
  std::ifstream ifile;

  if (argc != 2) {
    std::cout << "No stimulus file passed" << std::endl;
    return 1;
  }

  ifile.open(argv[1]);
  if (ifile.is_open()) {
    std::string line;
    while (std::getline(ifile, line)) {
      if (line[0] != '#' && line.size() > 0) {
        if (line[0] != ':') {
          input.push_back(line);
        } else {
          output.push_back(line.substr(1));
        }
      }
    }
    ifile.close();
  } else {
    std::cout << "Can't open " << argv[1] << std::endl;
    return 1;
  }

  IoTest io(input, output);
  Captive captive(&io);

  captive.Run();
  return io.passed() ? 0 : 1;
}