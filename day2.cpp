#include <cstring>
#include <fstream>
#include <set>
#include <string>

#include <gflags/gflags.h>

#include "utils.hpp"

DEFINE_string(data_file, "day2-ids.txt", "");
DEFINE_bool(debug, false, "");

void parse_line(std::string line, bool& has_two, bool& has_three);

void part_one() {
  std::vector<std::string> lines = str_file_contents(FLAGS_data_file);
  if (FLAGS_debug) {
    lines = {"abcdef", "bababc", "abbcde", "abcccd", "aabcdd", "abcdee", "ababab"};
  }
  int twos = 0, threes = 0;
  std::for_each(lines.begin(), lines.end(), [&](std::string line) {
    bool has_two, has_three;
    parse_line(line, has_two, has_three);
    if (has_two) {
      twos++;
    }
    if (has_three) {
      threes++;
    }
  });
  std::printf("Twos: %d\nThrees: %d\nChecksum: %d\n", twos, threes, twos * threes);
}

void parse_line(std::string line, bool& has_two, bool& has_three) {
  has_two = has_three = false;
  int char_count[0x100] = {0};
  for_each(line.begin(), line.end(), [&](const unsigned char c) { char_count[c]++; });
  for (int i = 0; i < 0x100; i++) {
    if (char_count[i] == 2) {
      has_two = true;
    } else if (char_count[i] == 3) {
      has_three = true;
    }
  }
}

void match_ids_from_index(std::vector<std::string> ids, int ix);

void part_two() {
  match_ids_from_index(str_file_contents(FLAGS_data_file), 0);
}

void match_ids_from_index(std::vector<std::string> ids, int ix) {
  std::vector<std::string> buckets[0x100];
  for_each(ids.begin(), ids.end(), [&](std::string id) { buckets[id[ix]].push_back(id); });
  for (int ii = 0; ii < 0x100; ii++) {
    if (buckets[ii].empty()) {
      continue;
    }
    bool found = false;
    for_each(buckets[ii].begin(), buckets[ii].end(), [&](std::string a) {
      for (int jj = ii + 1; jj < 0x100; jj++) {
        for_each(buckets[jj].begin(), buckets[jj].end(), [&](std::string b) {
          if (0 == strcmp(a.c_str() + (ix + 1), b.c_str() + (ix + 1))) {
            std::printf(
                "Match! %s|%s|%s <-> %s|%s|%s\n",
                a.substr(0, ix - 1).c_str(),
                a.substr(ix, 1).c_str(),
                a.substr(ix + 1).c_str(),
                b.substr(0, ix - 1).c_str(),
                b.substr(ix, 1).c_str(),
                b.substr(ix + 1).c_str());
          }
        });
      }
    });
    if (!found && buckets[ii].size() > 1) {
      match_ids_from_index(buckets[ii], ix + 1);
    }
  }
}

int main(int argc, char const* argv[]) {
  std::printf("Part one:\n");
  part_one();

  std::printf("\nPart two:\n");
  part_two();
  return 0;
}
