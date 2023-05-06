#include "pipeline.h"



/* Function that fetches 4-byte instruction from memory
 *
 * PARAM: int32_t address
 * 
 * RETURN: uint32_t
 * 
 * returns 32 bits of integer obtained from memory
 */
uint32_t fetch(int32_t address) {
    uint32_t instruction;
    instruction |= ARM.memory[address];
    instruction |= ARM.memory[address] << BYTE3_SHIFT;
    instruction |= ARM.memory[address] << BYTE2_SHIFT;
    instruction |= ARM.memory[address] << BYTE1_SHIFT;
    return instruction;
}

/*Function that idenfies the instruction by finding the pattern
 * PARAM: uint32_t instruction
 *
 * RETURN: int32_t
 * 
 * Returns one of the five types of instruction by analyzing the pattern
 * of the isntruction
 */

int32_t decode(uint32_t instruction) {
    
	if (instruction == 0) {
      return HALT;
  } else if ((DATA_PROCESSING_PATTERN & instruction) == 0) {
		  return DATA_PROCESSING;
	} else if (MULTIPLY_PATTERN & instruction == 0) {
		  return MULTIPLY;
	} else if ((SINGLE_DATA_TRANSFER_PATTERN & instruction) ==0) {
		  return DATA_PROCESSING;
	} else if ((BRANCH_PATTERN & instruction) == 0) {
		  return BRANCH;
	}
}

/* Function that executes the instruction
 * PARAM: uint32_t type, uint32_t instruction
 * RETURN: void
 * 
 * Executes the 5 different functions according to the type parameter. 
 */

void execute(uint32_t type, uint32_t instruction) {
	switch (type){
	case HALT:
    print_state();
		return 0;
	case DATA_PROCESSING:
		data_processing(instruction);
		return;
    case MULTIPLY:
		multiply(instruction);
		return;
	case SINGLE_DATA_TRANSFER:
		single_data_transfer(instruction);
		return;
	case BRANCH:
		branch(instruction);
		return;
	}
}

/* Function that manages fetch, decode, execute functions, thereby implementing three stage pipeline features
 * 
 * PARAM: void
 * RETURN: void
 * 
 * The function iterates in an infinite whlie loop. Using, if-else statement, we created three different cases.
 * 1) instruction is neither fetched nor decoded
 * 2) instruction is fetched but not decoded
 * 3) insturction is both fetched and decoded
 * 
 * According to each case, we designed appropriate instructions.
 */

void three_stage_pipeline(void) {
  bool is_fetched = false;
  bool is_decoded = false;
	uint32_t instruction;
  uint32_t temp_instruction;
	int32_t instruction_type;

  while (true) {
    if (is_fetched == false && is_decoded == false) {
      instruction = fetch(ARM.registers[PC]);
      is_fetched = true;
    } else if (is_fetched == true && is_decoded == false) {
      instruction_type = decode (instruction);
      temp_instruction = instruction;
      instruction = fetch(ARM.registers[PC]);
      is_decoded = true;
    } else if (is_fetched == true && is_decoded == true) {
      execute(instruction_type, temp_instruction);
      temp_instruction = instruction;
      instruction = fetch(ARM.registers[PC]);
      if (instruction_type == BRANCH) {
        is_fetched = false;
        is_decoded = false;
      }
    }
    ARM.registers[PC] += 4;
  }
}
