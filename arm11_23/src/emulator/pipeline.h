#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "print_state.h"
#include "arm_structure.h"

#define DATA_PROCESSING_PATTERN 0x0c000000
#define MULTIPLY_PATTERN 0x0fc00060
#define SINGLE_DATA_TRANSFER_PATTERN 0x08600000
#define BRANCH_PATTERN 0x05000000
#define BYTE3_SHIFT 8
#define BYTE2_SHIFT 16
#define BYTE1_SHIFT 24


enum instruction_type {
  HALT = 0,
  DATA_PROCESSING,
  MULTIPLY,
  SINGLE_DATA_TRANSFER,
  BRANCH
};

uint32_t fetch(int32_t address);

int32_t decode(uint32_t instruction);

void execute(uint32_t type, uint32_t instruction);

void three_stage_pipeline(void);

#endif
