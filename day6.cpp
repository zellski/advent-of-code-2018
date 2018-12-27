#include <cstring>
#include <exception>
#include <experimental/random>
#include <fstream>
#include <list>
#include <string>

#include <gflags/gflags.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "utils.hpp"

DEFINE_string(data_file, "day6-coords.txt", "");
DEFINE_bool(debug, false, "");
DEFINE_int32(padding, 50, "");

struct Coordinate {
  Coordinate() : Coordinate(0, 0) {}
  Coordinate(int x, int y) : x(x), y(y) {}
  int x;
  int y;
  int area = 0;
  struct {
    uint8_t r = std::experimental::randint(128, 255);
    uint8_t g = std::experimental::randint(128, 255);
    uint8_t b = std::experimental::randint(128, 255);
  } colour;
  bool invalidated = false;
};

std::vector<Coordinate> get_coordinates() {
  std::vector<std::string> lines = str_file_contents(FLAGS_data_file);
  if (FLAGS_debug) {
    lines = {"1, 1", "1, 6", "8, 3", "3, 4", "5, 5", "8, 9"};
  }
  std::vector<Coordinate> result(lines.size());
  std::transform(lines.begin(), lines.end(), result.begin(), [&](std::string line) -> Coordinate {
    int x, y;
    if (2 != sscanf(line.c_str(), "%d,%d", &x, &y)) {
      throw std::domain_error("failed to parse coordinate");
    }
    return {x, y};
  });
  return result;
}

struct Square {
  Coordinate* owner = nullptr;
  int16_t distance = -1;
};

void part_one() {
  int x_min = std::numeric_limits<int>::max();
  int x_max = std::numeric_limits<int>::min();
  int y_min = std::numeric_limits<int>::max();
  int y_max = std::numeric_limits<int>::min();

  std::vector<Coordinate> coords = get_coordinates();

  for_each(coords.begin(), coords.end(), [&](const Coordinate& coord) {
    x_min = std::min(x_min, coord.x);
    x_max = std::max(x_max, coord.x);
    y_min = std::min(y_min, coord.y);
    y_max = std::max(y_max, coord.y);
  });
  std::printf("Bounding box: (%d, %d) - (%d, %d)\n", x_min, y_min, x_max, y_max);

  int width = (1 + x_max - x_min) + 2 * FLAGS_padding;
  int height = (1 + y_max - y_min) + 2 * FLAGS_padding;
  uint8_t pixels[width * height * 3] = {0};
  Square squares[width][height];
  for (int xx = 0; xx < width; xx++) {
    for (int yy = 0; yy < height; yy++) {
      Square& square = squares[yy][xx];
      {
        int x = x_min + xx - FLAGS_padding;
        int y = y_min + yy - FLAGS_padding;
        for (int ii = 0; ii < coords.size(); ii++) {
          Coordinate& coord = coords[ii];
          int dist = std::abs(coord.x - x) + std::abs(coord.y - y);
          if (square.distance == -1 || dist < square.distance) {
            square.distance = dist;
            square.owner = &coord;
          } else if (dist == square.distance) {
            square.owner = nullptr;
          }
        }
      }
      if (square.owner != nullptr) {
        square.owner->area++;
        pixels[3 * (width * yy + xx) + 0x00] = square.owner->colour.r;
        pixels[3 * (width * yy + xx) + 0x01] = square.owner->colour.g;
        pixels[3 * (width * yy + xx) + 0x02] = square.owner->colour.b;
        if (xx == 0 || xx == width - 1 || yy == 0 || yy == width - 1) {
          square.owner->invalidated = true;
        }
      }
    }
  }
  std::sort(coords.begin(), coords.end(), [&](const Coordinate& a, const Coordinate& b) {
    return a.area < b.area;
  });
  for_each(coords.begin(), coords.end(), [&](const Coordinate& coord) {
    if (coord.invalidated) {
      std::printf("Coordinate (%3d, %3d) has been invalidated!\n", coord.x, coord.y);
    } else {
      std::printf(
          "Coordinate (%3d, %3d)[#%02X%02X%02X] has area: %d\n",
          coord.x,
          coord.y,
          coord.colour.r,
          coord.colour.g,
          coord.colour.b,
          coord.area);
    }
  });
  stbi_write_png("day6-map.png", width, height, 3, (void*)pixels, 0);
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
