#include <cstring>
#include <exception>
#include <fstream>
#include <list>
#include <string>

#include <gflags/gflags.h>

#include "utils.hpp"

DEFINE_string(data_file, "day5-polymer.txt", "");
DEFINE_bool(debug, false, "");

std::string get_polymer() {
  std::string line = single_line_file_contents(FLAGS_data_file);
  if (FLAGS_debug) {
    line = "dabAcCaCBAcCcaDA";
  }
  return line;
}

std::list<char> react_polymer(std::list<char> units) {
  auto current = units.begin();
  // step through line from start to finish
  while (true) {
    if (current == units.end() || std::next(current) == units.end()) {
      break;
    }
    auto next = std::next(current, 1);
    if ((*current & 0x1F) == (*next & 0x1F) && *current != *next) {
      auto new_current = (current == units.begin()) ? units.end() : std::prev(current);
      units.erase(current);
      units.erase(next);
      current = (new_current != units.end()) ? new_current : units.begin();
    } else {
      current = next;
    }
  }
  return units;
}

void part_one() {
  std::string line = get_polymer();
  std::list<char> units{line.begin(), line.end()};
  const auto& new_units = react_polymer(units);
  std::printf("After reduction, %lu units remain.\n", new_units.size());
}

void part_two() {
  std::string line = get_polymer();
  std::list<char> filtered_units(line.length());
  std::list<char> units{line.begin(), line.end()};
  for (char to_remove = 'a'; to_remove <= 'z'; to_remove++) {
    const auto iter =
        std::copy_if(units.begin(), units.end(), filtered_units.begin(), [&](char unit) {
          return (unit & 0x1F) != (to_remove & 0x1F);
        });
    std::list<char> filtered_list{filtered_units.begin(), iter};
    const auto& new_units = react_polymer(filtered_list);
    std::printf(
        "Removing '%c' leaves %lu units, and %lu in fully reacted form.\n",
        to_remove,
        filtered_list.size(),
        new_units.size());
  }
}

int main(int argc, char const* argv[]) {
  std::printf("Part one:\n");
  part_one();

  std::printf("\nPart two:\n");
  part_two();
  return 0;
}
