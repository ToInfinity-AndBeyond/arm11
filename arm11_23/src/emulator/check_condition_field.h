#ifndef CHECK_CONDITION_FIELD_H
#define CHECK_CONDITION_FIELD_H

#include "arm_structure.h"
#include <stdint.h>
#include <stdbool.h>

#define CPSR 16

#define ONE_BIT_MASK 1

#define V_FLAG_SHIFT 28
#define C_FLAG_SHIFT 29
#define Z_FLAG_SHIFT 30
#define N_FLAG_SHIFT 31
#define CONDITION_SHIFT 28

#define BIT_SET 1
#define BIT_NOT_SET 0

// COND Codes and their interpretation
#define EQUAL 0
#define NOT_EQUAL 1
#define GREATER_OR_EQUAL 10
#define LESS_THAN 11
#define GREATER_THAN 12
#define LESS_THAN_OR_EQUAL 13
#define ALWAYS 14

bool check_condition_field(uint32_t instruction);

#endif