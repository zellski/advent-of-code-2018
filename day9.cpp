#include <cstring>
#include <exception>
#include <experimental/random>
#include <experimental/set>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>

#include <gflags/gflags.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "utils.hpp"

DEFINE_int32(players, 9, "");
DEFINE_int32(last_marble, 25, "");
DEFINE_bool(debug, false, "");

template <class Iterator, typename T>
void print_circle(std::list<T> list, Iterator current, int player) {
  char buf[10];
  std::string str;
  if (player < 0) {
    str = "[--] ";
  } else {
    std::sprintf(buf, "[%2d] ", player + 1);
    str += buf;
  }
  for (auto iter = list.begin(); iter != list.end(); iter++) {
    if (*iter == *current) {
      sprintf(buf, "(%d)", *iter);
    } else {
      sprintf(buf, " %d ", *iter);
    }
    str += buf;
  }
  std::printf("%s\n", str.c_str());
}

void part_one() {
  std::list<int> circle{0};
  auto current_position = circle.begin();
  std::vector<long> scores(FLAGS_players);
  int current_player = 0;
  if (FLAGS_debug) {
    print_circle(circle, current_position, -1);
  }
  for (int next_marble = 1; next_marble <= FLAGS_last_marble; next_marble++) {
    if ((next_marble % 23) != 0) {
      current_position++;
      if (current_position == circle.end()) {
        current_position = circle.begin();
      }
      current_position++;
      current_position = circle.insert(current_position, next_marble);
      if (current_position == circle.end()) {
        current_position = circle.begin();
      }

    } else {
      scores.at(current_player) += next_marble;
      for (int ii = 0; ii < 7; ii++) {
        if (current_position == circle.begin()) {
          current_position = circle.end();
        }
        current_position--;
      }
      scores.at(current_player) += *current_position;
      current_position = circle.erase(current_position);
    }
    if (FLAGS_debug) {
      print_circle(circle, current_position, current_player);
    }
    current_player = (current_player + 1) % FLAGS_players;
  }
  long max_score = 0;
  int max_player = 0;
  for (int player = 0; player < FLAGS_players; player++) {
    if (FLAGS_debug) {
      std::printf("Player %2d: score = %lu\n", (player + 1), scores.at(player));
    }
    if (scores.at(player) > max_score) {
      max_score = scores.at(player);
      max_player = player;
    }
  }
  std::printf("Player %d scored the most, %lu points.\n", max_player, max_score);
}

void part_two() {}

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::printf("Part one:\n");
  part_one();

  std::printf("\nPart two:\n");
  part_two();
  return 0;
}
