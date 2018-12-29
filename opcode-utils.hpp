#pragma once

#include <exception>
#include <string>

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

// this is calculated in day 16, and was manually entered here afterwards for simplicity
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
