#ifndef print_state_h
#define print_state_h

#include "arm_structure.h"
#include <stdint.h>

#define NUM_OF_REGISTERS 17
#define GENERAL_PURPOSE_REGISTERS 12
#define PC 15
#define CPSR 16
#define SIZE_OF_MEMORY 65536
#define INSTRUCTION_SIZE 4
#define STOP 0

#define BYTE3_SHIFT 8
#define BYTE2_SHIFT 16
#define BYTE1_SHIFT 24

uint32_t print_instruction(int32_t address);
void print_state(void);

#endif