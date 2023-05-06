/*
 * ARM Emulator
 *
 * data_processing.c contains functions and helper functionsthat execute a data processing instruction
 * 
 * Group 23
 * Members : jc2520
 * 
 */

#include "data_processing.h"

/* The primary function that executes an instruction
 *
 * PARAM : uint32_t instruction
 * 32 bit binary representation of the instruction
 * 
 * RETURN : void
 * 
 * If the condition field is valid then split the instruction into opcode, rn, rd and operand2
 * The function first checks the Immediate flag to process operand2 into its proper form
 * Then, it checks the value of opcode to execute the appropriate operation (Arithmetic/Logical)
 * It also updates the CPSR flags if the S bit is set
 */

void dataProcessing(uint32_t instruction) {
    // checs the condition field of the instruction
    if(!check_condition_field(instruction)) {
        return;
    }

    // splits the instruction into its useful parts
    uint8_t opcode = (instruction >> OPCODE_SHIFT) & FOUR_BIT_MASK;
    uint8_t rn = (instruction >> RN_SHIFT) & FOUR_BIT_MASK;
    uint8_t rd = (instruction >> RD_SHIFT) & FOUR_BIT_MASK;
    uint32_t operand2 = instruction & OPERAND_MASK;
    uint32_t result = 0;

    // checks the immediate flag and fetches the value of operand2 accordingly
    if(is_immediate_operand_set(instruction)) {
        operand2 = rotate_imm(operand2 & EIGHT_BIT_MASK, operand2 >> ROTATE_SHIFT, opcode, instruction);
    } else {
        operand2 = shift_reg(operand2, opcode, instruction);
    }

    // execution
    if(is_logical(opcode)) {
        execute_logical_op(opcode, rn, operand2, rd);
    } else if(is_arithmetic(opcode)) {
        execute_arithmetic_op(opcode, rn, operand2, rd);
    } else {
        printf("UNDEFINED OPCODE");
    }

    // updates the Z bit and N bit if S bit it set
    if(s_bit_set(instruction)) {
        if(result == 0) {
            set_z_bit(BIT_SET);
        } else {
            set_z_bit(BIT_NOT_SET);
        }
        set_n_bit(result >> N_FLAG_SHIFT);
    }
}

/* Function that checks the Immediate flag 
 *
 * PARAM : uint32_t instruction
 * 32 bit binary representation of the instruction
 * 
 * RETURN : bool
 * Returns true if the Immediate flag = 1, false otherwise
 */
bool is_immediate_operand_set(uint32_t instruction) {
    return (instruction >> I_FLAG_SHIFT) & ONE_BIT_MASK; 
}

/* Function that determines whether the opcode indicates an arithmetic operation 
 *
 * PARAM : uint8_t opcode
 * 8 bit binary representation of the instruction
 * 
 * RETURN : bool
 * Returns true if opcode is either 0100, 0010, 0011, or 1010 
 * (ADD, SUB, RSB, or CMP)
 * Returns false otherwise
 */
bool is_arithmetic(uint8_t opcode) {
    return (opcode == OPCODE_ADD || 
            opcode == OPCODE_SUB || 
            opcode == OPCODE_RSB || 
            opcode == OPCODE_CMP);   
}

/* Function that determines whether the opcode indicates a logical operation
 * 
 * PARAM : uint8_t opcode
 * 8 bit binary representation of the instruction
 * 
 * RETURN : bool
 * Returns true if the opcode is either 0000, 0001, 1000, 1001, 1100, or 1101 
 * (AND, XOR, TST, TEQ, ORR, or MOV)
 * Returns false otherwise
 */
bool is_logical(uint8_t opcode) {
    return (opcode == OPCODE_AND || // and
            opcode == OPCODE_XOR || // xor
            opcode == OPCODE_TST || // tst
            opcode == OPCODE_TEQ || // teq
            opcode == OPCODE_ORR || // orr
            opcode == OPCODE_MOV);  // mov
}

/* Function that checks the Set flag
 *
 * PARAM : uint32_t instruction
 * 32 bit binary representation of the instruction
 * 
 * RETURN : bool
 * Returns true if the Set flag = 1, false otherwise
 */
bool s_bit_set(uint32_t instruction) {
    return (instruction >> S_FLAG_SHIFT) & ONE_BIT_MASK;
}

/* Function that sets the CPSR C flag, given the instruction
 *
 * PARAM  : the value which the C flag should be set to
 *  
 * RETURN : void
 */
void set_c_bit(uint8_t val) {
    if(val) {
        ARM.registers[CPSR] |= C_BIT_SET;
    } else {
        ARM.registers[CPSR] &= ~C_BIT_NOT_SET;
    }
}

/* Function that sets the CPSR Z flag, given the instruction
 *
 * PARAM  : the value which the Z flag should be set to
 *  
 * RETURN : void
 */
void set_z_bit(uint8_t val) {
    if(val) {
        ARM.registers[CPSR] |= Z_BIT_SET;
    } else {
        ARM.registers[CPSR] &= ~Z_BIT_NOT_SET;
    }
}


/* Function that sets the CPSR N flag, given the instruction
 *
 * PARAM  : the value which the N flag should be set to
 *  
 * RETURN : void
 */
void set_n_bit(uint8_t val) {
    if(val) {
        ARM.registers[CPSR] |= N_BIT_SET;
    } else {
        ARM.registers[CPSR] &= ~N_BIT_NOT_SET;
    }
}

/* Function that stores the Immediate value in operand2
 * After rotating it right by the specified value (bits 11-8) * 2 
 *
 * Case Immediate flag = 1
 * 
 * PARAM       : uint32_t operand2, uint8_t opcode, uint32_t instruction
 * val         : contains both the Immediate value (bits 7-0) to be rotated
 * shift_val   : contains the amount it has to be rotated to the right (bits 11-8)
 * opcode      : 8 bit value indicating the type of instruction to be executed
 * instruction : the initial instruction
 * 
 * RETURN : uint32_t
 * The value of operand2 after the rotation
 * 
 * The function receives val and shift_val instead of operand2 because its use in the shift_reg function
 * The function also sets the C bit if the Set flag is set in the instruction
 */
uint32_t rotate_imm(uint32_t val, uint32_t shift_val, uint8_t opcode, uint32_t instruction) {
        
    // rotate by shift_val * 2
    for(int i = 0; i < shift_val * 2; i++) {
        if(val & ONE_BIT_MASK == BIT_SET) {
            val = (val >> LSR_SHIFT) + (BIT_SET << INSTRUCTION_LENGTH);
        } else {
            val >>= LSR_SHIFT;
        }
    }

    // updates the C bit if S bit if the Set flag is set
    if(is_logical(opcode) && s_bit_set(instruction) && (decode(instruction) != SINGLE_DATA_TRANSFER)) {
        set_c_bit(val & MSB_MASK);
    }
    return val;
}

/* Function that takes the register Rm in operand2 (bits 3-0) and stores it in operand2
 * After shifting it according to the shift instruction in operand2 (bits 11-4)
 * 
 * Case Immediate flag = 0
 * 
 * PARAM       : uint32_t operand2, uint8_t opcode, uint32_t instruction
 * operand2    : contains the shift instruction (bits 11-4) and the register to be shifted (bits 3-0)
 * opcode      : 8 bit value indicating the type of instruction to be executed
 * instruction : the initial instruction
 * 
 * RETURN : uint32_t
 * The value of operand2 after the shift
 * 
 * If bit 4 = 1 shift by register, otherwise shift by an constant
 * 
 * Shift by register     : bit 11-8 contains the register Rs,
 *                         And the last byte of Rs is used as the amount to be shifted
 * Shift by and constant : bit 11-7 contains the amount to be shifted
 * 
 * Bits 6-5 indicates the type of shift to be performed on Rm
 * Logical left (lsl)     for 00
 * Logical right (lsr)    for 01
 * Arithmetic right (asr) for 10
 * Rotate right (ror)     for 11
 */   
uint32_t shift_reg(uint32_t operand2, uint8_t opcode, uint32_t instruction) {
    uint8_t shift_type = (operand2 >> SHIFT_TYPE_SHIFT) & TWO_BIT_MASK;
    uint32_t rm_val = ARM.registers[operand2 & FOUR_BIT_MASK];
    uint8_t shift_val;
    
    // check bit 4 to fetch shift register value
    if((operand2 >> SHIFT_BIT) & ONE_BIT_MASK == BIT_SET) {
        shift_val = operand2 >> REGSHIFT_CONSTANT_SHIFT;
    } else {
        uint32_t register_val = ARM.registers[operand2 >> REGSHIFT_REGISTER_SHIFT];
        shift_val = register_val & EIGHT_BIT_MASK;
    }

    // if shift_val is 0, Rm will be used as the second operand
    if(shift_val > 0) {
        switch(shift_type) {
            case LSL_SHIFT: // lsl
                rm_val <<= (shift_val - 1);
                if(is_logical(opcode) && s_bit_set(instruction) && (decode(instruction) != SINGLE_DATA_TRANSFER)) {
                    set_c_bit(rm_val & MSB_SHIFT);
                }
                return rm_val <<= 1;

            case LSR_SHIFT: // lsr
                rm_val >>= (shift_val - 1);
                if(is_logical(opcode) && s_bit_set(instruction) && (decode(instruction) != SINGLE_DATA_TRANSFER)) {
                    set_c_bit(rm_val & ONE_BIT_MASK);
                }
                return rm_val >>= 1;

            case ASR_SHIFT: // asr
                int sign_bit = (rm_val >> INSTRUCTION_LENGTH) & ONE_BIT_MASK;
                for(int i = 0; i < shift_val - 1; i++) {
                    if(sign_bit == BIT_SET) {
                        rm_val = (rm_val >> 1) + MSB_SET;
                    } else {
                        rm_val >>= 1;
                    }
                }
                if(is_logical(opcode) && s_bit_set(instruction) && (decode(instruction) != SINGLE_DATA_TRANSFER)) {
                    set_c_bit(rm_val & ONE_BIT_MASK);
                }
                if(sign_bit == BIT_SET) {
                    rm_val = (rm_val >> 1) + MSB_SET;
                } else {
                    rm_val >>= 1;
                }
                return rm_val;

            case ROR_SHIFT: // ror
                return rotate_imm(rm_val, shift_val / 2, opcode, instruction);

        }
    }
}

/* Function that executes a arithmetic operation (ADD, SUB, RSB, CMP)
 *
 * PARAM        : uint8_t opcode, uint8_t rn, uint32_t operand2_val, uint8_t rd
 * opcode       : indicates the instruction to be executed
 * rn           : the supplied register
 * operand2_val : the value of operand2
 * rd           : the register in which the result will be stored
 *
 * RETURN : uint32_t
 * The result after the arithmetic operation is executed
 * The result will be in register rd for ADD, SUB and RSB
 * It will be stored in a separate uint32_t value for CMP
 */
uint32_t execute_arithmetic_op(uint8_t opcode, uint8_t rn, uint32_t operand2_val, uint8_t rd) {
    
    uint32_t result;

    switch(opcode) {
      
        case OPCODE_ADD: // add
            if(check_overflow(ARM.registers[rn], operand2_val)) {
                set_c_bit(BIT_SET);
            } else {
                set_c_bit(BIT_NOT_SET);
            }
            ARM.registers[rd] = ARM.registers[rn] + operand2_val;
            return ARM.registers[rd];

        case OPCODE_SUB: // sub
            if(operand2_val > ARM.registers[rn]) {
                set_c_bit(BIT_NOT_SET);
            } else if(check_overflow(ARM.registers[rn], ~(operand2_val) + 1)) {
                set_c_bit(BIT_NOT_SET);
            } else {
                set_c_bit(BIT_SET);
            }
            ARM.registers[rd] = ARM.registers[rn] - operand2_val;
            return ARM.registers[rd];

        case OPCODE_RSB: // rsb
            if(ARM.registers[rn] > operand2_val) {
                set_c_bit(BIT_NOT_SET);
            } else if(check_overflow(operand2_val, ~(ARM.registers[rn]) + 1)) {
                set_c_bit(BIT_NOT_SET);
            } else {
                set_c_bit(BIT_SET);
            }
            ARM.registers[rd] = operand2_val - ARM.registers[rn];
            return ARM.registers[rd];

        case OPCODE_CMP: // cmp
            if(operand2_val > ARM.registers[rn]) {
                set_c_bit(BIT_NOT_SET);
            } else if(check_overflow(ARM.registers[rn], ~(operand2_val) + 1)) {
                set_c_bit(BIT_NOT_SET);
            } else {
                set_c_bit(BIT_SET);
            }
            result = ARM.registers[rn] - operand2_val;
            return result;

        default:
            printf("Not an executable arithmetic operation");
            return 0;
    }
}

/* Function that executes a logical operation (AND, XOR, TST, TEQ, ORR, MOV)
 *
 * PARAM        : uint8_t opcode, uint8_t rn, uint32_t operand2_val, uint8_t rd
 * opcode       : indicates the instruction to be executed
 * rn           : the supplied register
 * operand2_val : the value of operand2
 * rd           : the register in which the result will be stored
 *
 * RETURN : uint32_t
 * The result after the logical operation is executed
 * The result will be in register rd for AND, XOR, ORR, and MOV
 */
uint32_t execute_logical_op(uint8_t opcode, uint8_t rn, uint32_t operand2_val, uint8_t rd) {

    switch(opcode) {

        case OPCODE_AND: // and
            ARM.registers[rd] = ARM.registers[rn] & operand2_val;
            return ARM.registers[rd];

        case OPCODE_XOR: // xor
            ARM.registers[rd] = ARM.registers[rn] ^ operand2_val;
            return ARM.registers[rd];

        case OPCODE_TST: // tst
            return ARM.registers[rn] & operand2_val;

        case OPCODE_TEQ: // teq
            return ARM.registers[rn] ^ operand2_val;

        case OPCODE_ORR: // orr
            ARM.registers[rd] = ARM.registers[rn] | operand2_val;
            return ARM.registers[rd];

        case OPCODE_MOV: // mov
            ARM.registers[rd] = operand2_val;
            return ARM.registers[rd];
        
        default:
            printf("Not an executable logical operation");
            return 0;
    }
}

/* Function that checks whether an arithmetic overflow has occurred 
 *
 * PARAM  : uint32_t num1, uint32_t num2
 * num1   : first value
 * num2   : second value
 * 
 * RETURN : bool
 * Returns true if overflow occurs, false otherwise
 */
bool check_overflow(uint32_t num1, uint32_t num2) {
    return num1 > (INT_MAX - num2) || num2 > (INT_MAX - num1);
}