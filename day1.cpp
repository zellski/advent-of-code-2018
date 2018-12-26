#include <fstream>
#include <set>
#include <string>

#include <gflags/gflags.h>

DEFINE_string(frequency_file, "day1-frequencies.txt", "");

int main(int argc, char const* argv[]) {
  {
    std::ifstream in_stream{FLAGS_frequency_file};
    std::string line;
    int tot_val = 0;
    while (std::getline(in_stream, line)) {
      int n = std::stoi(line);
      tot_val += n;
    }
    std::printf("Sum of all changes: %d\n", tot_val);
  }

  {
    std::set<int> seen;
    std::ifstream in_stream{FLAGS_frequency_file};
    std::string line;
    int tot_val = 0;
    while (seen.find(tot_val) == seen.end()) {
      seen.insert(tot_val);
      if (in_stream.eof()) {
        in_stream.seekg(0);
      }
      std::getline(in_stream, line);
      tot_val += std::stoi(line);
    }

    std::printf("First repeated frequency: %d\n", tot_val);
  }
  return 0;
}
