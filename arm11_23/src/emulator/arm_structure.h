#ifndef ARM_STRUCTURE_H
#define ARM_STRUCTURE_H

#include <stdint.h>

#define SIZE_OF_MEMORY 65536
#define NUMBER_OF_REGISTERS 17
#define PC 15

/* 
 * Struct Machine that contains array of memory with type uint8_t and size of MEMORY_SIZE
 * and array of registers with type uint32_t and size of REGISTERS_NUMBER
 */

struct State {
	uint8_t memory[SIZE_OF_MEMORY];
	int32_t registers[NUMBER_OF_REGISTERS];
};

// declares the variable ARM of the type Machine
struct State ARM;

#endif
