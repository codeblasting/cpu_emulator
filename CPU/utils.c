#include "utils.h"

void print_code(CODE c){
u_char rmost8 = deapply_code(c) << 24 >> 24;
char reg = -1;
char* addr_mode = is_unary(c) ? 0 : addr_name(c);
if(rmost8 > 5 && rmost8 < 0x12)reg = get_bit(rmost8,0);
if(rmost8 >= 0x60)reg = get_bit(rmost8,3);
 
  printf("CODE_OP: %s\nCODE_r: %d\nCODE_addr_mode: %s\nCODE_val: 0x%x\n",
   op_name(c), reg, addr_mode,c.value);
}

CODE decode(unsigned char* ptr){

     unsigned int p = *(unsigned int*)ptr;
     return apply_code(p);
    
}

CODE apply_code(int bin){
 unsigned char* temp = (unsigned char*)&bin;
 temp[3]  = temp[1];
 temp[1]  = 0;
 return *(CODE*)&bin;
}

int deapply_code(CODE c){
  int res = *(int*)&c;
  unsigned char* temp = (unsigned char*)&res;
  temp[1] = temp[3];
  temp[3] = 0;
  

  return res;
};
  
 int is_unary(CODE c){
   u_char x = deapply_code(c) << 24 >> 24;
    return (x < 0x12 )||(x == 0x26);
 } 

void print_mem(unsigned short index){
  
	unsigned char* bytes = &Mem[index];
	printf("%x ",bytes[0]);
	printf("%x ",bytes[1]);
        printf("\n");
}


void execute(CODE c){
u_short rmost8 = deapply_code(c) << 24 >> 24 ;

  void (*f_ptr)() = (void*)find_op(rmost8).f_ptr;
  short c_val = is_unary(c) ? 0 : c.value;
   if( f_ptr )
    f_ptr();
};


ENTRY find_op(unsigned char op_bits){

if(op_bits >= IN_STOP && op_bits <= IN_MOVAFLG)
       return OP_TABLE8[op_bits];

op_bits >>= 1;
if(op_bits >= IN_NOT && op_bits <= IN_NOPn)
       return OP_TABLE7[op_bits];

op_bits >>= 2;
if(op_bits >= IN_NOP && op_bits <= IN_SUBSP)
       return OP_TABLE5[op_bits];

op_bits >>= 1;
if(op_bits >= IN_ADD && op_bits <= IN_STB)
       return OP_TABLE4[op_bits];


};

char* op_name(CODE c){
  return find_op( deapply_code(c) << 24 >> 24 ).f_name;
};

char* addr_name( CODE c ){

int addr_mode = c.addr_mode;
u_char rmost8 = *IP << 8 >> 8;

if(rmost8 < 0x26){
  addr_mode &= 1;
 addr_mode = (addr_mode) ? ADDR_x : addr_mode;
};
 

	switch(addr_mode){

	         
	case ADDR_i: return "ADDR_i";
	case ADDR_d: return "ADDR_d";
	case ADDR_n: return "ADDR_n";
	case ADDR_s: return "ADDR_s";
	case ADDR_sf:return "ADDR_sf";
	case ADDR_x: return "ADDR_x";
	case ADDR_sx: return "ADDR_sx";
	case ADDR_sfx: return "ADDR_sfx";
	default: return "NOT_FOUND";

        };

  };


int is_stop(CODE c){
  int r = c.main + c.reg + c.addr_mode;
  return r == 0;
};

u_short* get_value(int addr_bits){

	char mask = ~(-1 << addr_bits);
	char OP_byte = *(char*)IP;
        char addr_mode = OP_byte & mask;
        u_short val  = _CODE.value;
        u_short* ret_val;
        u_short reg_x = REG_X;
        rotate_short(&reg_x);
      
        if(addr_bits == 1)
         if(addr_mode == ADDR_d)
           addr_mode = ADDR_x; 
        
        
	
	switch(addr_mode){

	  case ADDR_i:
             ret_val = malloc(2);
             rotate_short(&val);
            *ret_val = val;
             break;

	  case ADDR_d:
           ret_val =  &Mem[val];break;

          case ADDR_n:ret_val =  &Mem[S_PTR(&Mem[val])];break;
          case ADDR_s:ret_val =  &Mem[(SP+val)&0xFFFF];break;

          case ADDR_sf:ret_val =  &Mem[S_PTR(&Mem[(SP+val)&0xFFFF])];break;
          case ADDR_x:ret_val =  &Mem[(val + reg_x)&0xFFFF];break;
          case ADDR_sx:ret_val =  &Mem[(SP + val + reg_x)&0xFFFF];break;

          case ADDR_sfx:ret_val =  &Mem[ (S_PTR(&Mem[(SP + val)&0xFFFF]) + reg_x)&0xFFFF ];break;

        };
 _RET_VAL = ret_val;
 return ret_val;

};


void rotate_short(u_short* addr){

	u_char* OP_BYTE = (u_char*)addr;
	u_short val = 
         OP_BYTE[0] << 8 | OP_BYTE[1];

       *addr = val;	
};

char get_bit(u_short arg, char bit){
  rotate_short(&arg);
  u_short mask =  1 << bit;
  return (arg & mask) >> bit;
};
u_short* get_reg(){

 u_char rmost8 = *IP << 8 >> 8;
 u_char the_bit = (rmost8 < 0x12) ? 0 : 3;

 u_short IP_val = *IP;
 rotate_short(&IP_val);
 u_char rbit = get_bit(IP_val,the_bit);

 return rbit ? &REG_X : &REG_A;

};

void load(int code){

char* p = &code;
static int i = 0;

Mem[i++] = p[0];
Mem[i++] = p[1];
Mem[i++] = p[2];

}



void cpu_loop(){


SP = 0xFB8F;

printf("started cpu\n\n");
_CODE  = apply_code(0xFFFFFF);


while( !is_stop(_CODE) ){
     IP = &Mem[PC];
     _CODE = decode(IP);
     PC += is_unary(_CODE) ? 1 : 3;
     execute(_CODE);
};

printf("stopped cpu\n");
return;

};

void load_prog(char* name){

  FILE* f = fopen(name,"r");
  char buff[1000];
  
  int i = 0;
  for(int c = 0; !feof(f);)
    if( (c=fgetc(f)) >= 48 ) 
     buff[i++] = c;

   buff[i] = 0;
    
  printf("\nbuff: %s\n\n",buff);
 
 char c, *m_ptr = &Mem[0];
  i = 0;
  while(buff[i] != '\0'){
   char f = (buff[i] >= 'A') ? buff[i] - 55 : buff[i] - 48;
   i++;
   char s = (buff[i] >= 'A') ? buff[i] - 55 : buff[i] - 48;
  *(m_ptr++) = f * 16 + s;
  i++;
 
};

for(int i = 0 ; i < 7 ; i++)
 printf("Mem[%d] is %x\n", i, Mem[i]);

};

void print_bytes(void* p, int k){
  u_char* c = p;
  for(int i = 0; i < k; i++)
   printf("[%d]:%x ",i,c[i]);
    printf("\n");

 };

u_short S_PTR(char* arg){
     u_short s = *(u_short*)arg;
     rotate_short(&s);
     return s;
};

 
