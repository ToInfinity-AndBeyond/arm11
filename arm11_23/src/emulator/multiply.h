#ifndef MULTIPLY_H
#define MULTIPLY_H

#include "arm_structure.h"
#include "check_condition_field.h"
#include <stdint.h>

#define CPSR 16

#define ONE_BIT_MASK 1
#define FOUR_BIT_MASK 0xf
#define Z_FLAG_MASK 1 << 30
#define N_FLAG_MASK 1 << 31

#define MUL_REG1_SHIFT 16
#define MUL_REG2_SHIFT 12
#define MUL_REG3_SHIFT 8
#define MUL_ACC_BIT_SHIFT 21
#define MUL_COND_BIT_SHIFT 20

void multiply(uint32_t instruction);

#endif