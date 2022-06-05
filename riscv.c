#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "riscv.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int U_TYPE = 3;
const int UNKNOWN_TYPE = 4;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
static int get_op_type(char *op)
{
    const char *r_type_op[] = {"add", "sub", "and", "or", "xor", "slt", "sll", "sra"};
    const char *i_type_op[] = {"addi", "andi", "ori", "xori", "slti"};
    const char *mem_type_op[] = {"lw", "lb", "sw", "sb"};
    const char *u_type_op[] = {"lui"};
    for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(r_type_op[i], op) == 0)
        {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(i_type_op[i], op) == 0)
        {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(mem_type_op[i], op) == 0)
        {
            return MEM_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(u_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(u_type_op[i], op) == 0)
        {
            return U_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
// TODO: create any additional variables to store the state of the interpreter


	char* temp_dest_reg;
	char* offset_temp;
	char* temp_reg_1;
	char* temp_reg_2;
	char* dest_mem_temp;
    int reg_1;
    int reg_2_imm;
    int offset;
	int temp;
	int temp_2;
	int dest_mem;
	int value_reg_1;
	int value_reg_2;
	int dest_reg;


void init(registers_t *starting_registers)
{
    registers = starting_registers;
    // TODO: initialize any additional variables needed for state
    table = ht_init(128);
}

// TODO: create any necessary helper functions
int get_num(char *token) {
	
	char *hex = "0x";
	char *neg_hex = "-0x";
	char *reg = "x";
	char temp_1[2];
	char temp_2[3];
	char temp_3[4];
	char *end;
	int value;
	memset(temp_1, '\0', sizeof(temp_1));
	memset(temp_2, '\0', sizeof(temp_2));
	memset(temp_3, '\0', sizeof(temp_3));
	strncpy(temp_1, token, 1);
	strncpy(temp_2, token, 2);
	strncpy(temp_3, token, 3);
	if (strcmp(hex, temp_2) == 0) {
		value = (int) strtol(token, &end, 16);
		return value;
	}

	else if (strcmp(neg_hex, temp_3) == 0) {
		value = (int) strtol(token, &end, 16);
                return value;
	}

	else if (strcmp(reg, temp_1) == 0) {
		value = (int) strtol(token+1, &end, 10);
		return value;
	}

	else {
		value = (int) strtol(token, &end, 10);
		return value;
	}

}

char *trim_space(char* instruct) {
	while (isspace(*instruct)) {
		instruct++;
	}
	return instruct;

}

int sign_extend_i(int number) {
	
	number = number%0x1000;
        if ((number & 0x00000800) != 0) {
                number = number | 0xfffff000;
        }

	return number;
}

int sign_extend_lb(int number) {
	
	if ((number & 0x00000080) != 0) {
		number = number | 0xffffff00;
	}

	return number;

}

void step(char *instruction)
{
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE)
    {
        return;
    }

    // TODO: write logic for evaluating instruction on current interpreter state
    temp_dest_reg = strsep(&instruction, ",");
	temp_dest_reg = trim_space(temp_dest_reg);
	dest_reg = get_num(temp_dest_reg);


	if ((op_type == 0) || (op_type == 1)) {

		if (dest_reg == 0) {
			return;
		}

		temp_reg_1 = strsep(&instruction, ",");
		temp_reg_1 = trim_space(temp_reg_1);
		reg_1 = get_num(temp_reg_1);
		temp_reg_2 = trim_space(instruction);
		reg_2_imm = get_num(temp_reg_2);

		value_reg_1 = registers->r[reg_1];
		
		if (op_type == 0) {

			value_reg_2 = registers->r[reg_2_imm];
			
			if (strcmp("add", op) == 0) {

				registers->r[dest_reg] = value_reg_1 + value_reg_2;

			}

			if (strcmp("sub", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 - value_reg_2;

                        }

			if (strcmp("and", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 & value_reg_2;

                        }

			if (strcmp("or", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 | value_reg_2;

                        }

			if (strcmp("xor", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 ^ value_reg_2;

                        }

			if (strcmp("nor", op) == 0) {

                                registers->r[dest_reg] = ~(value_reg_1 | value_reg_2);

                        }

			if (strcmp("slt", op) == 0) {

                                if (value_reg_1 < value_reg_2) {
					registers->r[dest_reg] = 1;
				}

				else {
					registers->r[dest_reg] = 0;
				}


                        }

			if (strcmp("sll", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 << (0x0000001f & value_reg_2);

                        }

			if (strcmp("sra", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 >> (0x0000001f & value_reg_2);

                        }

		}

		if (op_type == 1) {

			reg_2_imm = sign_extend_i(reg_2_imm);

			if (strcmp("addi", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 + reg_2_imm;

                        }

			if (strcmp("andi", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 & reg_2_imm;

                        }

			if (strcmp("ori", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 | reg_2_imm;

                        }

			if (strcmp("xori", op) == 0) {

                                registers->r[dest_reg] = value_reg_1 ^ reg_2_imm;

                        }

			if (strcmp("slti", op) == 0) {

                                if (value_reg_1 < reg_2_imm) {
                                        registers->r[dest_reg] = 1;
                                }

                                else {
                                        registers->r[dest_reg] = 0;
                                }


                        }

		}

	}
	
	

	if (op_type == 2) {

		offset_temp = strsep(&instruction, "(");
		offset_temp = trim_space(offset_temp);
		offset = get_num(offset_temp);

		dest_mem_temp = trim_space(instruction);

		dest_mem = registers->r[get_num(dest_mem_temp)];

		if (strcmp("lw", op) == 0) {
		
			if (dest_reg == 0) {
				return;
			}

			temp = ht_get(table, dest_mem+offset+3);
			temp = temp << 8;
			temp += ht_get(table, dest_mem+offset+2);
                        temp = temp << 8;
			temp += ht_get(table, dest_mem+offset+1);
                        temp = temp << 8;
			temp += ht_get(table, dest_mem+offset);
			registers->r[dest_reg] = temp;

		}

		if (strcmp("lb", op) == 0) {

                	if (dest_reg == 0) {
				return;
			}

			temp = ht_get(table, dest_mem+offset);	
			temp = sign_extend_lb(temp);
                        registers->r[dest_reg] = temp;
                        
                }

		if (strcmp("sw", op) == 0) {

                        
			ht_add(table, dest_mem + offset + 3, ((registers->r[dest_reg] >> 24) & 0x000000ff));
			ht_add(table, dest_mem + offset + 2, ((registers->r[dest_reg] >> 16) & 0x000000ff));
			ht_add(table, dest_mem + offset + 1, ((registers->r[dest_reg] >> 8) & 0x000000ff));
			ht_add(table, dest_mem + offset, ((registers->r[dest_reg]) & 0x000000ff));

                }

		if (strcmp("sb", op) == 0) {

			temp = (registers->r[dest_reg] & 0x000000ff);
			ht_add(table, dest_mem + offset, temp);
                
		}
		
	}

	if (op_type == 3) {

		if (dest_reg == 0) {
			return;
		}

		temp_reg_2 = trim_space(instruction);
		reg_2_imm = get_num(temp_reg_2);
		registers->r[dest_reg] = reg_2_imm << 12;

	}

}
