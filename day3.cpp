#include <cstring>
#include <exception>
#include <fstream>
#include <set>
#include <string>

#include <gflags/gflags.h>

#include "utils.hpp"

DEFINE_string(data_file, "day3-claims.txt", "");
DEFINE_bool(debug, false, "");

struct Claim {
  int id;
  int left;
  int top;
  int width;
  int height;

  bool overlaps(const Claim& other) const {
    return (left < other.left + other.width) && (left + width >= other.left) &&
        (top < other.top + other.height) && (top + height >= other.top);
  }
};

std::vector<Claim> get_claims() {
  std::vector<std::string> lines = str_file_contents(FLAGS_data_file);
  if (FLAGS_debug) {
    lines = {"abcdef", "bababc", "abbcde", "abcccd", "aabcdd", "abcdee", "ababab"};
  }
  std::vector<Claim> claims(lines.size());
  std::transform(lines.begin(), lines.end(), claims.begin(), [&](std::string line) {
    Claim result;
    if (5 !=
        std::sscanf(
            line.c_str(),
            "#%d @ %d,%d: %dx%d",
            &result.id,
            &result.left,
            &result.top,
            &result.width,
            &result.height)) {
      throw std::domain_error("parse error");
    }
    return result;
  });
  return claims;
}

void part_one() {
  const auto& claims = get_claims();

  std::set<std::pair<int, int>> claimed_inches;
  std::set<std::pair<int, int>> double_claimed_inches;
  for_each(claims.begin(), claims.end(), [&](const Claim& claim) {
    for (int xx = 0; xx < claim.width; xx++) {
      for (int yy = 0; yy < claim.height; yy++) {
        std::pair<int, int> point = {claim.left + xx, claim.top + yy};
        if (claimed_inches.find(point) != claimed_inches.end()) {
          double_claimed_inches.insert(point);
        }
        claimed_inches.insert(point);
      }
    }
  });
  std::printf("Doubly claimed squares: %lu\n", double_claimed_inches.size());
}

void part_two() {
  const auto& claims = get_claims();

  for (int ii = 0; ii < claims.size(); ii++) {
    bool overlap = false;
    for (int jj = 0; jj < claims.size(); jj++) {
      if (ii == jj) {
        continue;
      }
      if (claims[ii].overlaps(claims[jj])) {
        overlap = true;
        break;
      }
    }
    if (!overlap) {
      std::printf("Eureka! Claim #%d doesn't have any overlaps.\n", claims[ii].id);
    }
  }
  std::printf("Bugger. No answer found.\n");
}

int main(int argc, char const* argv[]) {
  std::printf("Part one:\n");
  part_one();

  std::printf("\nPart two:\n");
  part_two();
  return 0;
}
