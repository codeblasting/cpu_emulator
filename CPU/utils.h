
#include "vars.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

u_short S_PTR(char* arg);
void print_code(CODE c);
CODE decode(unsigned char* ptr);
CODE apply_code(int bin);
int deapply_code(CODE c);
int is_unary(CODE c);
void execute(CODE c);
int is_stop(CODE c);
ENTRY find_op(unsigned char op_bits);
char* op_name(CODE c);
char* addr_name( CODE c );
u_short* get_value(int addr_bits);
void rotate_short( u_short* );
u_short* get_reg();
char get_bit(u_short, char);
void cpu_loop();
void load_prog(char*);
void print_bytes(void*, int);
