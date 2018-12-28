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

DEFINE_string(data_file, "day7-dependencies.txt", "");
DEFINE_bool(debug, false, "");

struct Step {
  Step(char id) : id(id) {}
  char id;
  std::vector<char> needs;
  std::vector<char> yields;
  int completed = -1;
};

std::map<char, std::shared_ptr<Step>> steps;
std::shared_ptr<Step> getStep(char id) {
  if (steps.find(id) == steps.end()) {
    std::shared_ptr<Step> step = std::make_shared<Step>(id);
    steps.insert(std::make_pair(id, step));
    return step;
  }
  return steps.at(id);
};

void populate_steps() {
  std::vector<std::string> lines = str_file_contents(FLAGS_data_file);

  for_each(lines.begin(), lines.end(), [&](const std::string line) {
    char step_char, yield_char;
    if (2 !=
        std::sscanf(
            line.c_str(),
            "Step %c must be finished before step %c can begin.",
            &step_char,
            &yield_char)) {
      throw std::domain_error("parse error");
    }
    const auto step = getStep(step_char);
    step->yields.emplace_back(yield_char);

    const auto yield = getStep(yield_char);
    yield->needs.emplace_back(step_char);
  });
}

void part_one() {
  std::set<char, std::less<char>> available;
  for_each(steps.begin(), steps.end(), [&](const auto pair) {
    const std::shared_ptr<Step> step = pair.second;
    if (step->needs.empty()) {
      available.insert(step->id);
    }
  });
  std::set<char> done;
  std::string order;
  while (true) {
    auto it = available.begin();
    if (it == available.end()) {
      break;
    }
    const auto step = steps.at(*it);
    available.erase(it);
    done.insert(step->id);
    order += std::string(1, step->id);
    for_each(step->yields.begin(), step->yields.end(), [&](char yield) {
      // did we already do this?
      if (done.find(yield) != done.end()) {
        // yep: return early
        return;
      }
      const auto yieldStep = steps.at(yield);
      for (const auto& need : yieldStep->needs) {
        // allow unmet dependency on current step!
        if (need == step->id) {
          continue;
        }
        if (done.find(need) == done.end()) {
          // this dependency is unmet: return early
          return;
        }
      }
      // otherwise add this step as available
      available.insert(yield);
    });
  }
  std::printf("Order traversed: %s\n", order.c_str());
}

void part_two() {
  std::set<char> available;
  for_each(steps.begin(), steps.end(), [&](const auto pair) {
    const std::shared_ptr<Step> step = pair.second;
    if (step->needs.empty()) {
      available.insert(step->id);
    }
  });
  std::set<char> processing;
  std::set<char> done;
  std::string order;
  int free_workers = 5;
  int second = 0;
  while (true) {
    // first, see what steps, if any, just completed
    std::vector<char> done_processing;
    std::copy_if(
        processing.begin(),
        processing.end(),
        std::back_inserter(done_processing),
        [&](const char c) -> bool { return steps.at(c)->completed == second; });
    for (char step : done_processing) {
      processing.erase(step);
      done.insert(step);
      order += std::string(1, step);
      free_workers++;
      std::printf("[%4d] Task %c completed (%d workers now free).\n", second, step, free_workers);
      const auto ptr = steps.at(step);

      for_each(ptr->yields.begin(), ptr->yields.end(), [&](char yield) {
        // did we already do this?
        if (done.find(yield) != done.end()) {
          // yep: return early
          return;
        }
        // are we we already doing this?
        if (processing.find(yield) != processing.end()) {
          // yep: return early
          return;
        }
        const auto yieldStep = steps.at(yield);
        for (const auto& need : yieldStep->needs) {
          // allow unmet dependency on current step!
          if (need == step) {
            continue;
          }
          if (done.find(need) == done.end()) {
            // this dependency is unmet: return early
            return;
          }
        }
        // otherwise add this step as available
        std::printf("[%4d] Dependent %c now available to be worked on.\n", second, yield);
        available.insert(yield);
      });
    }
    // check if we're done
    if (available.empty() && processing.empty()) {
      break;
    }

    // if there are available workers && anything to work on, get to it
    while (free_workers > 0 && !available.empty()) {
      char step = *(available.begin());
      const auto ptr = steps.at(step);
      available.erase(step);
      processing.insert(step);
      ptr->completed = second + 60 + (1 + step - 'A');
      free_workers--;
      std::printf("[%4d] Task %c started (%d workers now free).\n", second, step, free_workers);
    }
    // finally, step time forward
    second++;
  }
  std::printf("After %d seconds, order traversed: %s\n", second, order.c_str());
}

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  populate_steps();

  std::printf("Part one:\n");
  part_one();

  std::printf("\nPart two:\n");
  part_two();
  return 0;
}
