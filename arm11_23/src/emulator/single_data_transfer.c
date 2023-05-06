#include "single_data_transfer.h"

/* Function that executes single data transfer instructions (load/store)
 *
 * PARAM: uint32_t instruction
 * 
 * 32-bit unsigned binary representation of the instruction
 * RETURN: void 
 * 
 * If flag I is set, offset is interpreted as a shifted register
 * Else, the offset is interpreted as an unsigned immediate offset
 * 
 * If flag P is set, pre-indexing without the change in base register's content
 * Else, post-indexing with the change of base register's content
 * 
 * If flag U is set, offset is added in indexing
 * Else, offset is subtracted in indexing
 * 
 * This function uses helper function data_transfer
 * 
 */
void single_data_transfer(uint32_t instruction) {

    if (!check_condition_flag(instruction)) {
        return;
    }

    bool flagI = checkflag(instruction, FLAG_I_SHIFT);
    bool flagP = checkflag(instruction, FLAG_P_SHIFT);
    bool flagU = checkflag(instruction, FLAG_U_SHIFT);
    bool flagL = checkflag(instruction, FLAG_L_SHIFT);
    uint32_t rn = (instruction >> RN_SHIFT) & REGISTER_BITMASK;
    uint32_t rd = (instruction >> RD_SHIFT) & REGISTER_BITMASK;
    uint32_t offset = instruction & OFFSET_BITMASK;
    int sign = NEGATIVE; // sign preset to negative

    if (flagI) {
        if(!flagP && ((offset & ILLEGAL_CHECK_BITMASK) == rn)) {
			perror("Illegal Operation!");
			exit(EXIT_FAILURE);
		}
		offset = shift_reg(offset, ((offset >> 1) & TWO_LEAST_BITS), instruction);
    } 

    if (flagU) {
        sign = POSITIVE;
    }

    if (flagP) {
        rn += sign * offset;
        data_transfer(flagL, ARM.registers[rn], rd);
    } else {
        data_transfer(flagL, ARM.registers[rn], rd);
        ARM.registers[rn] += sign * offset; 
    }
}

bool checkflag(uint32_t instruction, int flag_bit) {
    return (instruction >> flag_bit) & 1;
}

/* Helper function that carries out load/store execution
 *
 * PARAM: int flagL, int base_address, int dest_address
 * 
 * flagL - indicates which of load or store should be executed
 * base_address - the address of the base register
 * dest_address - the address of the destination register
 * 
 * RETURN: void
 * 
 * If flag L is set, word is loaded from memory
 * Else, word is stored into memory
 * 
 */
void data_transfer(bool flagL, uint32_t base_address, uint32_t dest_address) {
    if(flagL) { // load 
		if(base_address + THREE_BYTES < SIZE_OF_MEMORY) {
			ARM.registers[dest_address] = fetchInstruction(base_address);
		} else {
			perror("Segmentation fault: Memory out of bound!");
            exit(EXIT_FAILURE);
		}
	} else { // store 
		uint32_t dest_reg_content = ARM.registers[dest_address];

		if(base_address + THREE_BYTES < SIZE_OF_MEMORY) {
			ARM.memory[base_address] = dest_reg_content & REGISTER_BITMASK;
			ARM.memory[base_address + ONE_BYTE] = 
                (dest_reg_content & (REGISTER_BITMASK << ONE_BYTE_SHIFT)) >> ONE_BYTE_SHIFT;
			ARM.memory[base_address + TWO_BYTES] = 
                (dest_reg_content & (REGISTER_BITMASK << TWO_BYTE_SHIFT)) >> TWO_BYTE_SHIFT;
			ARM.memory[base_address + THREE_BYTES] = 
                (dest_reg_content & (REGISTER_BITMASK << THREE_BYTE_SHIFT)) >> THREE_BYTE_SHIFT;
		} else {
            perror("Segmentation fault: Memory out of bound!");
            exit(EXIT_FAILURE);
        }
	}
}
