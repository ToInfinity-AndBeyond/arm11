/*
 * ARM Emulator
 *
 * check_condition_field.c contains the function that checks the condition of an instruction
 * 
 * Group 23
 * Members : jc2520
 */

#include "check_condition_field.h"


/* Function returns true if the Condition code matches the CPSR flag requirement
 * False otherwise
 *
 * PARAM : uint32_t instruction
 * 32 bit binary representation of the instruction
 * 
 * RETURN : bool
 * 
 * The Condition codes and their interpretation are as following
 * 0000 (equal)              : Z set
 * 0001 (not equal)          : Z not set
 * 1010 (greater or equal)   : N equals V
 * 1011 (less than)          : N not equal to V
 * 1100 (greater than)       : Z not set AND (N equals V)
 * 1101 (less than or equal) : Z set OR (N not equal to V)
 * 1110 (always)             : no condition in CPSR (ignored)
 */

bool check_condition_field(uint32_t instruction) {

    //split instruction to its condition code (bits 31-28) and fetch the CPSR flags
    uint32_t v_flag = (ARM.registers[CPSR] >> V_FLAG_SHIFT) & ONE_BIT_MASK;
    uint32_t z_flag = (ARM.registers[CPSR] >> Z_FLAG_SHIFT) & ONE_BIT_MASK;
    uint32_t n_flag = (ARM.registers[CPSR] >> N_FLAG_SHIFT) & ONE_BIT_MASK;
    uint8_t condition = instruction >> CONDITION_SHIFT;

    return
        condition == EQUAL              && z_flag == BIT_SET                           ||
        condition == NOT_EQUAL          && z_flag == BIT_NOT_SET                       ||
        condition == GREATER_OR_EQUAL   && n_flag == v_flag                            ||
        condition == LESS_THAN          && n_flag != v_flag                            ||
        condition == GREATER_THAN       && (z_flag == BIT_NOT_SET && n_flag == v_flag) ||
        condition == LESS_THAN_OR_EQUAL && (z_flag == BIT_SET || n_flag != v_flag)     ||
        condition == ALWAYS;
}
