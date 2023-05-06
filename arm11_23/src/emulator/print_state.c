/*
 * ARM Emulator
 *
 * print_state.c contains the functions that print the state of the ARM
 * 
 * Group 23
 * Members : jc2520
 */

#include "print_state.h"

/* Function that prints the 4 byte or 16 bit instruction at a given memory address
 *
 * PARAM : int32_t address
 * 32 bit signed integer that represents an address in memory
 * g
 * RETURN : uint32_t
 * 32 bit binary representation of the corresponding instruction
 * Memory is stored in little endian
 */

uint32_t print_instruction(int32_t address) {
	uint32_t byte4 = ARM.memory[address + 3];
	uint32_t byte3 = ARM.memory[address + 2] << BYTE3_SHIFT;
	uint32_t byte2 = ARM.memory[address + 1] << BYTE2_SHIFT;
	uint32_t byte1 = ARM.memory[address] << BYTE1_SHIFT;
	return byte4 | byte3 | byte2 | byte1;
}

/* Function that prints the state of the ARM
 *
 * PARAM : void
 * 
 * RETURN : void
 */
void print_state(void) {

	// registers
	printf("<Registers>\n");
	for(int32_t i = 0; i < NUM_OF_REGISTERS; i++) {
		if(i <= GENERAL_PURPOSE_REGISTERS) {
			printf("$%d :", i);
		} else if(i == PC) {
			printf("PC  :");
		} else if(i == CPSR) {
			printf("CPSR:");
		} else {
			continue;
		}

		pritnf("%11d (0x%08x)\n", ARM.registers[i], ARM.registers[i]);
	}

	// memory
	printf("<Memory in use>\n");
	for(int32_t i = 0; i < SIZE_OF_MEMORY; i += INSTRUCTION_SIZE) {
		if(fetchInstruction(i) != STOP) {
			printf("0x%08x: 0x%08x\n", i, printInstruction(i));
		}
	}
}