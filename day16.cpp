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

DEFINE_string(cases_data_file, "day16-test-cases.txt", "");
DEFINE_string(program_data_file, "day16-test-program.txt", "");
DEFINE_bool(debug, false, "");

// order not yet known
enum OpCode {
  NONE = -1,
  ADDR,
  ADDI,
  MULR,
  MULI,
  BANR,
  BANI,
  BORR,
  BORI,
  SETR,
  SETI,
  GTIR,
  GTRI,
  GTRR,
  EQIR,
  EQRI,
  EQRR
};

std::string describe_opcode(OpCode opcode) {
  switch (opcode) {
    case ADDR:
      return "ADDR";
    case ADDI:
      return "ADDI";
    case MULR:
      return "MULR";
    case MULI:
      return "MULI";
    case BANR:
      return "BANR";
    case BANI:
      return "BANI";
    case BORR:
      return "BORR";
    case BORI:
      return "BORI";
    case SETR:
      return "SETR";
    case SETI:
      return "SETI";
    case GTIR:
      return "GTIR";
    case GTRI:
      return "GTRI";
    case GTRR:
      return "GTRR";
    case EQIR:
      return "EQIR";
    case EQRI:
      return "EQRI";
    case EQRR:
      return "EQRR";
    default:
      throw std::domain_error("unknown opcode");
  }
}

// this is calculated below, and was manually entered here afterwards for simplicity
OpCode int_to_opcode(int n) {
  switch (n) {
    case 0x00:
      return EQRI;
    case 0x01:
      return MULR;
    case 0x02:
      return GTRI;
    case 0x03:
      return GTRR;
    case 0x04:
      return BANR;
    case 0x05:
      return ADDI;
    case 0x06:
      return SETI;
    case 0x07:
      return GTIR;
    case 0x08:
      return MULI;
    case 0x09:
      return BORI;
    case 0x0A:
      return SETR;
    case 0x0B:
      return ADDR;
    case 0x0C:
      return BANI;
    case 0x0D:
      return BORR;
    case 0x0E:
      return EQIR;
    case 0x0F:
      return EQRR;
    default:
      throw std::domain_error("unknown opcode");
  }
}

struct Instruction {
  OpCode op_code;
  int in_A;
  int in_B;
  int out_C;
};

struct State {
  int regs[4] = {0, 0, 0, 0};
  bool operator==(const State& other) const {
    return (regs[0] == other.regs[0]) && (regs[1] == other.regs[1]) && (regs[2] == other.regs[2]) &&
        regs[3] == other.regs[3];
  }
  bool operator!=(const State& other) const {
    return !(*this == other);
  }
};

struct TestCase {
  State in;
  Instruction instr;
  State out;
};

std::vector<TestCase> get_test_cases() {
  std::vector<std::string> lines = str_file_contents(FLAGS_cases_data_file);
  if (FLAGS_debug) {
    lines = {"Before: [3, 2, 1, 1]", "9 2 1 2", "After:  [3, 2, 2, 1]", ""};
  }
  std::vector<TestCase> result;
  for (size_t ii = 0; ii < lines.size(); ii += 4) {
    TestCase test_case;
    if (4 !=
        sscanf(
            lines[ii].c_str(),
            "Before: [%d, %d, %d, %d]",
            &test_case.in.regs[0],
            &test_case.in.regs[1],
            &test_case.in.regs[2],
            &test_case.in.regs[3])) {
      throw std::domain_error("in_regs parse error");
    }
    int op_num;
    if (4 !=
        sscanf(
            lines[ii + 1].c_str(),
            "%d %d %d %d",
            &op_num,
            &test_case.instr.in_A,
            &test_case.instr.in_B,
            &test_case.instr.out_C)) {
      throw std::domain_error("instr parse error");
    }
    test_case.instr.op_code = int_to_opcode(op_num);
    if (4 !=
        sscanf(
            lines[ii + 2].c_str(),
            "After: [%d, %d, %d, %d]",
            &test_case.out.regs[0],
            &test_case.out.regs[1],
            &test_case.out.regs[2],
            &test_case.out.regs[3])) {
      throw std::domain_error("in_regs parse error");
    }
    result.push_back(test_case);
  }
  return result;
}

std::vector<Instruction> get_test_program() {
  std::vector<std::string> lines = str_file_contents(FLAGS_program_data_file);
  std::vector<Instruction> result;
  for (size_t ii = 0; ii < lines.size(); ii++) {
    Instruction instr;
    int op_num;
    if (4 !=
        sscanf(lines[ii].c_str(), "%d %d %d %d", &op_num, &instr.in_A, &instr.in_B, &instr.out_C)) {
      throw std::domain_error("instr parse error");
    }
    instr.op_code = int_to_opcode(op_num);
    result.push_back(instr);
  }
  return result;
}

State execute(Instruction instr, const State& in) {
  State out = in;
  switch (instr.op_code) {
    case NONE:
      throw std::domain_error("NONE opcode is not valid");
    case ADDR:
      out.regs[instr.out_C] = in.regs[instr.in_A] + in.regs[instr.in_B];
      break;
    case ADDI:
      out.regs[instr.out_C] = in.regs[instr.in_A] + instr.in_B;
      break;
    case MULR:
      out.regs[instr.out_C] = in.regs[instr.in_A] * in.regs[instr.in_B];
      break;
    case MULI:
      out.regs[instr.out_C] = in.regs[instr.in_A] * instr.in_B;
      break;
    case BANR:
      out.regs[instr.out_C] = in.regs[instr.in_A] & in.regs[instr.in_B];
      break;
    case BANI:
      out.regs[instr.out_C] = in.regs[instr.in_A] & instr.in_B;
      break;
    case BORR:
      out.regs[instr.out_C] = in.regs[instr.in_A] | in.regs[instr.in_B];
      break;
    case BORI:
      out.regs[instr.out_C] = in.regs[instr.in_A] | instr.in_B;
      break;
    case SETR:
      out.regs[instr.out_C] = in.regs[instr.in_A];
      break;
    case SETI:
      out.regs[instr.out_C] = instr.in_A;
      break;
    case GTIR:
      out.regs[instr.out_C] = instr.in_A > in.regs[instr.in_B] ? 1 : 0;
      break;
    case GTRI:
      out.regs[instr.out_C] = in.regs[instr.in_A] > instr.in_B ? 1 : 0;
      break;
    case GTRR:
      out.regs[instr.out_C] = in.regs[instr.in_A] > in.regs[instr.in_B] ? 1 : 0;
      break;
    case EQIR:
      out.regs[instr.out_C] = instr.in_A == in.regs[instr.in_B] ? 1 : 0;
      break;
    case EQRI:
      out.regs[instr.out_C] = in.regs[instr.in_A] == instr.in_B ? 1 : 0;
      break;
    case EQRR:
      out.regs[instr.out_C] = in.regs[instr.in_A] == in.regs[instr.in_B] ? 1 : 0;
      break;
  }
  if (FLAGS_debug) {
    std::printf(
        "[%d, %d, %d, %d] & (op = %s, A = %d, B = %d, C = %d) -> [%d, %d, %d, %d]\n",
        in.regs[0],
        in.regs[1],
        in.regs[2],
        in.regs[3],
        describe_opcode(instr.op_code).c_str(),
        instr.in_A,
        instr.in_B,
        instr.out_C,
        out.regs[0],
        out.regs[1],
        out.regs[2],
        out.regs[3]);
  }
  return out;
}

void part_one() {
  int behaves_like_three_or_more_count = 0;
  const auto& test_cases = get_test_cases();
  for (TestCase test_case : test_cases) {
    int matches = 0;
    for (int op_num = 0; op_num < 0x10; op_num++) {
      OpCode code = int_to_opcode(op_num);
      Instruction instr = test_case.instr;
      instr.op_code = code;
      State out = execute(instr, test_case.in);
      if (out == test_case.out) {
        matches++;
      }
    }
    if (matches >= 3) {
      behaves_like_three_or_more_count++;
    }
  }
  std::printf(
      "Cases (out of %lu) that behave like three or more opcodes: %d\n",
      test_cases.size(),
      behaves_like_three_or_more_count);
}

// code to create mapping
void build_mapping() {
  const auto& test_cases = get_test_cases();

  std::set<OpCode> mapped_opcodes;
  std::vector<OpCode> opcode_mapping(0x10, OpCode::NONE);
  std::vector<std::set<OpCode>> possible_opcodes(0x10);
  for (int op_num = 0; op_num < 0x10; op_num++) {
    possible_opcodes.at(op_num) = {ADDR,
                                   ADDI,
                                   MULR,
                                   MULI,
                                   BANR,
                                   BANI,
                                   BORR,
                                   BORI,
                                   SETR,
                                   SETI,
                                   GTIR,
                                   GTRI,
                                   GTRR,
                                   EQIR,
                                   EQRI,
                                   EQRR};
  }
  while (mapped_opcodes.size() < 0x10) {
    for (TestCase test_case : test_cases) {
      for (int op_num = 0; op_num < 0x10; op_num++) {
        OpCode code = int_to_opcode(op_num);
        Instruction instr = test_case.instr;
        instr.op_code = code;
        State out = execute(instr, test_case.in);
        if (out != test_case.out) {
          possible_opcodes.at(test_case.instr.op_code).erase(code);
        }
      }
    }
    for (int op_num = 0; op_num < 0x10; op_num++) {
      if (opcode_mapping[op_num] != OpCode::NONE) {
        continue;
      }
      std::set<OpCode> set = possible_opcodes[op_num];
      for (const OpCode mapped_opcode : mapped_opcodes) {
        set.erase(mapped_opcode);
      }
      if (set.empty()) {
        std::printf("Fuck! Op #%d has no candidates. Aborting early.\n", op_num);
        return;
      }
      if (set.size() > 1) {
        // nothing certain yet, keep looping
        continue;
      }
      OpCode code = *set.begin();
      mapped_opcodes.insert(code);
      opcode_mapping[op_num] = code;
    }
  }
  for (int op_num = 0; op_num < 0x10; op_num++) {
    std::printf("Op 0x%X -> %s\n", op_num, describe_opcode(opcode_mapping[op_num]).c_str());
  }
}

void part_two() {
  State state;
  for (const Instruction& instr : get_test_program()) {
    state = execute(instr, state);
  }
  std::printf("After completion, register 0 holds the value: %d\n", state.regs[0]);
}

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::printf("Part one:\n");
  part_one();

  std::printf("Build mapping:\n");
  build_mapping();

  std::printf("\nPart two:\n");
  part_two();
  return 0;
}
