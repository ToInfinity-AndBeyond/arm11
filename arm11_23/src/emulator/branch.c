#include "branch.h"


/* Function that executes branch instructions
 *
 * PARAM: u_int32_t instruction
 * 32-bit unsigned binary representation of the instruction
 * 
 * RETURN: void 
 * 
 * Next instruction in PC is modified according to the 24 bit signed offset
 *
 */
void branch(uint32_t instruction) {
    if (!check_condition_field(instruction)) {
       return;
    }

    int32_t offset = instruction & OFFSET_BITMASK;
    int32_t sign_bit = 0;
    
	if ((offset << OFFSET_SHIFT) < 0) {
		sign_bit = SIGN_BIT_MASK;
	}
	offset = offset * sizeof(instruction) + sign_bit;
	ARM.registers[PC] += offset - sizeof(instruction);
}
