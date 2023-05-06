#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include "arm_structure.h"
#include "pipeline.h"
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#define INSTRUCTION_LENGTH 31
#define CPSR 16

#define ONE_BIT_MASK 1
#define TWO_BIT_MASK 3
#define FOUR_BIT_MASK 0xf
#define EIGHT_BIT_MASK 0xff
#define OPERAND_MASK 0xfff
#define MSB_MASK 1 << 31

#define ROTATE_SHIFT 8
#define OPCODE_SHIFT 21
#define RN_SHIFT 16
#define RD_SHIFT 12
#define SHIFT_TYPE_SHIFT 5
#define SHIFT_BIT 4
#define REGSHIFT_CONSTANT_SHIFT 7
#define REGSHIFT_REGISTER_SHIFT 8
#define MSB_SHIFT 1 << 31
#define N_FLAG_SHIFT 31
#define I_FLAG_SHIFT 25
#define S_FLAG_SHIFT 20

#define BIT_SET 1
#define BIT_NOT_SET 0
#define MSB_SET 1 << 31
#define C_BIT_SET 1 << 29
#define C_BIT_NOT_SET 0 << 29
#define Z_BIT_SET 1 << 30
#define Z_BIT_NOT_SET 0 << 30
#define N_BIT_SET 1 << 31
#define N_BIT_NOT_SET 0 << 31

// Operand2 Shift codes and their interpretations
#define LSL_SHIFT 0
#define LSR_SHIFT 1
#define ASR_SHIFT 2
#define ROR_SHIFT 3

// Opcodes and their interpretations
#define OPCODE_AND 0
#define OPCODE_XOR 1
#define OPCODE_SUB 2
#define OPCODE_RSB 3
#define OPCODE_ADD 4
#define OPCODE_TST 8
#define OPCODE_TEQ 9
#define OPCODE_CMP 10
#define OPCODE_ORR 12
#define OPCODE_MOV 13


void data_processing(uint32_t instruction);
bool is_immediate_operand_set(uint32_t instruction);
bool is_arithmetic(uint8_t opcode);
bool is_logical(uint8_t opcode);
bool s_bit_set(uint32_t instruction);
void set_c_bit(uint8_t val);
void set_z_bit(uint8_t val);
void set_n_bit(uint8_t val);

#endif