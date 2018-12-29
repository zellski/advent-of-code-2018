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

DEFINE_string(data_file, "day8-nodes.txt", "");
DEFINE_bool(debug, false, "");

struct Node {
  Node(std::vector<std::shared_ptr<Node>> children, std::vector<int> metadata)
      : children(children), metadata(metadata) {}
  const std::vector<std::shared_ptr<Node>> children;
  const std::vector<int> metadata;
};

std::vector<int> get_node_data() {
  return int_file_contents(FLAGS_data_file, ' ');
}

std::shared_ptr<Node> read_node(const std::vector<int> data, int& offset) {
  int child_count = data[offset++];
  int metadata_count = data[offset++];
  std::vector<std::shared_ptr<Node>> children;
  for (int ii = 0; ii < child_count; ii++) {
    const auto child = read_node(data, offset);
    children.emplace_back(child);
  }
  std::vector<int> metadata;
  for (int ii = 0; ii < metadata_count; ii++) {
    metadata.emplace_back(data[offset++]);
  }
  return std::make_shared<Node>(children, metadata);
}

std::shared_ptr<Node> read_node(const std::vector<int> data) {
  int offset = 0;
  const auto& result = read_node(data, offset);
  return result;
}

int sum_metadata(std::shared_ptr<const Node> node) {
  int sum = 0;
  for (const auto& ptr : node->children) {
    sum += sum_metadata(ptr);
  }
  for (const int n : node->metadata) {
    sum += n;
  }
  return sum;
}

int node_value(std::shared_ptr<const Node> node) {
  if (node->children.empty()) {
    int sum = 0;
    for (const int n : node->metadata) {
      sum += n;
    }
    return sum;
  }
  int sum = 0;
  for (const uint n : node->metadata) {
    if (n >= 1 && n <= node->children.size()) {
      sum += node_value(node->children.at(n - 1));
    }
  }
  return sum;
}

void part_one() {
  const std::shared_ptr<Node> root = read_node(get_node_data());
  std::printf("Sum of all metadata: %d\n", sum_metadata(root));
}

void part_two() {
  const std::shared_ptr<Node> root = read_node(get_node_data());
  std::printf("Value of root: %d\n", node_value(root));
}

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::printf("Part one:\n");
  part_one();

  std::printf("\nPart two:\n");
  part_two();
  return 0;
}
