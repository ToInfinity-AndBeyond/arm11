/*
 * ARM Emulator
 *
 * multiply.c contains the function that executes a multiply instruction
 * 
 * Group 23
 * Members : jc2520
 */

#include "multiply.h"

/* Primary function that executes a multiply instruction
 * 
 * PARAM : uint32_t instruction
 * 32 bit binary representation of the instruction
 * 
 * RETURN : void
 * 
 * If the Accumulate bit = 1, Rd := Rm x Rs + Rn
 * Otherwise Rd := Rm x Rs
 * 
 * If the S bit has been set, the N and Z flags of CPSR should be updated
 */

void multiply(uint32_t instruction) {

    // checks condition of the instruction
    if (!check_condition_field(instruction)) {
        return;
    }

    uint32_t rd = (instruction >> MUL_REG1_SHIFT) & FOUR_BIT_MASK;
    uint32_t rn = (instruction >> MUL_REG2_SHIFT) & FOUR_BIT_MASK;
    uint32_t rs = (instruction >> MUL_REG3_SHIFT) & FOUR_BIT_MASK;
    uint32_t rm = instruction & FOUR_BIT_MASK;
    uint8_t accumulator = (instruction >> MUL_ACC_BIT_SHIFT) & ONE_BIT_MASK;
    uint8_t set_condition = (instruction >> MUL_COND_BIT_SHIFT) & ONE_BIT_MASK;

    if (accumulator) {
        ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs] + ARM.registers[rn];
    } else {
        ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs];
    }

    if (set_condition) {
        
        // updates the N flag
        uint32_t n_flag = ARM.registers[rd] & N_FLAG_MASK;
        ARM.registers[CPSR] |= n_flag;

        // updates the Z flag
        if (ARM.registers[rd] == 0) {
            ARM.registers[CPSR] |= Z_FLAG_MASK;
        } else {
            ARM.registers[CPSR] &= ~Z_FLAG_MASK;
        }
    }
}
