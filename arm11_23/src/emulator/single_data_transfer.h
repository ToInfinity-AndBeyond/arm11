#ifndef SINGLE_DATA_TRANSFER_H
#define SINGLE_DATA_TRANSFER_H

#include "arm_structure.h"
#include "check_condition_field.h"
#include "data_processing.h"
#include <stdint.h>
#include <stdbool.h>

// Single_data_transfer constants
#define FLAG_I_SHIFT 25
#define FLAG_P_SHIFT 24
#define FLAG_U_SHIFT 23
#define FLAG_L_SHIFT 20
#define RN_SHIFT 16
#define RD_SHIFT 12

#define REGISTER_BITMASK 15
#define OFFSET_BITMASK 0x0FFF
#define REGISTER_BITMASK 0xFF
#define ILLEGAL_CHECK_BITMASK 15

#define NEGATIVE -1
#define POSITIVE 1
#define TWO_LEAST_BITS 3

#define ONE_BYTE_SHIFT 8
#define TWO_BYTE_SHIFT 16
#define THREE_BYTE_SHIFT 24

#define ONE_BYTE 1
#define TWO_BYTES 2
#define THREE_BYTES 3

// Single_data_transfer function declaration
void single_data_transfer(uint32_t instruction);
void data_transfer(bool flagL, uint32_t base_address, uint32_t dest_address);
bool checkflag(uint32_t instruction, int flag_bit);

#endif
