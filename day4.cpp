#include <cstring>
#include <exception>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <string>

#include <gflags/gflags.h>

#include "utils.hpp"

DEFINE_string(data_file, "day4-events.txt", "");
DEFINE_bool(debug, false, "");

struct Event {
  enum Type { BEGIN_SHIFT, WAKE_UP, FALL_ASLEEP };
  int guard_id = -1;
  Type type;
  struct Time {
    int year;
    int month;
    int day;
    int hour;
    int minute;
  } time;
};

// [1518-10-16 00:02] Guard #2837 begins shift
// [1518-06-01 00:30] wakes up
// [1518-10-28 00:42] falls asleep

std::vector<Event> get_events() {
  std::vector<std::string> lines = str_file_contents(FLAGS_data_file);
  if (FLAGS_debug) {
    lines = {"[1518-11-01 00:00] Guard #10 begins shift",
             "[1518-11-01 00:05] falls asleep",
             "[1518-11-01 00:25] wakes up",
             "[1518-11-01 00:30] falls asleep",
             "[1518-11-01 00:55] wakes up",
             "[1518-11-01 23:58] Guard #99 begins shift",
             "[1518-11-02 00:40] falls asleep",
             "[1518-11-02 00:50] wakes up",
             "[1518-11-03 00:05] Guard #10 begins shift",
             "[1518-11-03 00:24] falls asleep",
             "[1518-11-03 00:29] wakes up",
             "[1518-11-04 00:02] Guard #99 begins shift",
             "[1518-11-04 00:36] falls asleep",
             "[1518-11-04 00:46] wakes up",
             "[1518-11-05 00:03] Guard #99 begins shift",
             "[1518-11-05 00:45] falls asleep",
             "[1518-11-05 00:55] wakes up"};
  }
  std::vector<Event> events(lines.size());
  std::transform(lines.begin(), lines.end(), events.begin(), [&](std::string line) {
    Event result;
    if (5 !=
        std::sscanf(
            line.c_str(),
            "[%d-%d-%d %d:%d]",
            &result.time.year,
            &result.time.month,
            &result.time.day,
            &result.time.hour,
            &result.time.minute)) {
      throw std::domain_error("line parse error");
    }
    const char* rest = line.c_str() + 19;
    if (0 == strcmp(rest, "falls asleep")) {
      result.type = Event::Type::FALL_ASLEEP;
    } else if (0 == strcmp(rest, "wakes up")) {
      result.type = Event::Type::WAKE_UP;
    } else if (1 == std::sscanf(rest, "Guard #%d begins shift", &result.guard_id)) {
      result.type = Event::Type::BEGIN_SHIFT;
    } else {
      throw std::domain_error("strbuf parse error");
    }
    return result;
  });
  std::sort(events.begin(), events.end(), [&](const Event& a, const Event& b) {
    if (a.time.year != b.time.year) {
      return a.time.year < b.time.year;
    }
    if (a.time.month != b.time.month) {
      return a.time.month < b.time.month;
    }
    if (a.time.day != b.time.day) {
      return a.time.day < b.time.day;
    }
    if (a.time.hour != b.time.hour) {
      return a.time.hour < b.time.hour;
    }
    if (a.time.minute != b.time.minute) {
      return a.time.minute < b.time.minute;
    }
  });
  int cur_guard_id = -1;
  for_each(events.begin(), events.end(), [&](Event& event) {
    if (event.type == Event::Type::BEGIN_SHIFT) {
      cur_guard_id = event.guard_id;
    } else if (cur_guard_id >= 0) {
      event.guard_id = cur_guard_id;
    } else {
      throw std::domain_error("no current guard id");
    }
  });
  return events;
}

struct GuardTimeline {
  int asleep_count_per_minute[60]{0};
  int total_minutes_asleep = 0;
};

std::map<int, GuardTimeline> get_guard_timelines() {
  const auto& events = get_events();
  std::map<int, GuardTimeline> guard_timelines;
  int start_minute = -1;
  int cur_guard_id = -1;
  for_each(events.begin(), events.end(), [&](const Event& event) {
    switch (event.type) {
      case Event::Type::FALL_ASLEEP: {
        start_minute = event.time.minute;
        cur_guard_id = event.guard_id;
        const auto it = guard_timelines.find(cur_guard_id);
        if (it == guard_timelines.end()) {
          GuardTimeline new_timeline;
          guard_timelines.insert(std::make_pair(cur_guard_id, new_timeline));
        }
        break;
      }
      case Event::Type::WAKE_UP: {
        if (start_minute < 0 || cur_guard_id < 0) {
          throw std::domain_error("wake-up without sleep");
        }
        GuardTimeline& timeline = guard_timelines.at(cur_guard_id);
        for (int jj = start_minute; jj < event.time.minute; jj++) {
          timeline.asleep_count_per_minute[jj]++;
        }
        timeline.total_minutes_asleep += (event.time.minute - start_minute);
        break;
      }
    }
  });
  return guard_timelines;
}

void part_one() {
  const auto& guard_timelines = get_guard_timelines();
  int top_guard_id = -1;
  int top_minutes = -1;
  std::for_each(guard_timelines.begin(), guard_timelines.end(), [&](const auto& pair) {
    if (pair.second.total_minutes_asleep > top_minutes) {
      top_guard_id = pair.first;
      top_minutes = pair.second.total_minutes_asleep;
    }
  });
  const GuardTimeline& timeline = guard_timelines.at(top_guard_id);
  int top_minute = -1;
  int top_minutes_asleep = -1;
  for (int ii = 0; ii < 60; ii++) {
    if (timeline.asleep_count_per_minute[ii] > top_minutes_asleep) {
      top_minute = ii;
      top_minutes_asleep = timeline.asleep_count_per_minute[ii];
    }
  }
  std::printf(
      "Best guard: %d\nTotal minutes: %d\nTop minute: %d\nChecksum: %d\n",
      top_guard_id,
      top_minutes,
      top_minute,
      top_guard_id * top_minute);
}

void part_two() {
  const auto& guard_timelines = get_guard_timelines();
  int top_minute = -1;
  int top_minute_asleep_count = -1;
  int top_guard_id = -1;
  std::for_each(guard_timelines.begin(), guard_timelines.end(), [&](const auto& pair) {
    for (int ii = 0; ii < 60; ii++) {
      if (pair.second.asleep_count_per_minute[ii] > top_minute_asleep_count) {
        top_minute_asleep_count = pair.second.asleep_count_per_minute[ii];
        top_minute = ii;
        top_guard_id = pair.first;
      }
    }
  });
  std::printf(
      "Winning guard: %d\nTotal sleep count: %d\nTop minute: %d\nChecksum: %d\n",
      top_guard_id,
      top_minute_asleep_count,
      top_minute,
      top_guard_id * top_minute);
}

int main(int argc, char const* argv[]) {
  std::printf("Part one:\n");
  part_one();

  std::printf("\nPart two:\n");
  part_two();
  return 0;
}
