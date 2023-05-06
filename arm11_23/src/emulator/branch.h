#ifndef BRANCH_H
#define BRANCH_H

#include "arm_structure.h"
#include "check_condition_field.h"
#include <stdint.h>

// Constants for branch.c
#define OFFSET_BITMASK 0x00FFFFFF
#define OFFSET_SHIFT 8
#define SIGN_BIT_MASK (-1) << 26

// Function declaration for branch
void branch(uint32_t instruction);

#endif