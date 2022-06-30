#include "codes.h"

 void STOP(){
   printf("STOP FUNCTION\n");
 };

void RET(){

 PC = S_PTR(&Mem[SP]);
 rotate_short(&PC);

 SP += 2;

}
void RETR(){};
void MOVSPA(){

  REG_A = SP;
  rotate_short(&REG_A);
}
void MOVFLGA(){
  u_short n = N << 3;//15-3 = 12
  u_short z = Z << 2;
  u_short v = V << 1;
  u_short c = C;

  REG_A = n | z | v | c;

 };

void MOVAFLG(){
 
  N = get_bit(REG_A,3);
  Z = get_bit(REG_A,2);
  V = get_bit(REG_A,1);
  C = get_bit(REG_A,0);
	

}

void NOT(){
u_short* reg = get_reg(); 
*reg  = ~*reg;

N = (*reg < 0);
Z = (*reg == 0);
 
}
void NEG(){

 u_short* reg = get_reg();
 rotate_short(reg);
 *reg=-*reg;
 rotate_short(reg);
 
 N = get_bit(*reg,15);
 Z = (*reg == 0);
 V = (*reg == 32768);

};


void ASL(){

 u_short* reg = get_reg();
 C = get_bit(*reg,15);

 *reg <<= 1;

 N = get_bit(*reg,15);
 Z = (*reg == 0);
 V = (C == get_bit(*reg,15));
 
};
void ASR(){

u_short* reg = get_reg();
 C = get_bit(*reg,0);
 *reg >>= 1;

 N = (*reg < 0);
 Z = (*reg == 0);

} 
 
void ROL(){
u_short* reg = get_reg();
ASL();
*reg |= C;
};


void ROR(){
 u_short* reg = get_reg();
 C  = *reg & 1;
 ASR();
 *reg |= ( C<<15 );

};

void BR(){
 u_short val = *get_value(1);
 rotate_short(&val);
 PC = val;
}

void BRLE(){
 if(N|Z)
  BR();
}

void BRLT(){
 if(N)
  BR();
}
  
void BREQ(){
 if(Z)
  BR();
}
void BRNE(){
 if(!Z)
  BR();
};
void BRGE(){
 if(!N)
  BR();
};
void BRGT(){
 if(!N & !Z)
  BR();
};
void BRV(){
 if(V)
  BR();
}
void BRC(){
 if(C)
  BR();
};

void CALL(){
 SP -= 2;
 Mem[SP] = PC;
 u_short val = *get_value(1);
 rotate_short(&val);
 PC = val;
};

void NOPn(){};
void NOP(){};


void DECI(){

  printf("INPUT DECI: \n");
  u_short* ptr = get_value(3);
  scanf("%hu",ptr);
  rotate_short(ptr);
 
  Z = (*ptr == 0);
  N = get_bit(*ptr,15);
 
  
};


void DECO(){

 short val = *get_value(3);
  rotate_short(&val);
  printf("DEC_OUT:%d\n",val);
}

void HEXO(){
 u_short val = *get_value(3);
 printf("HEX_OUT:%x\n",val);
}
 
void STRO(){
 char* ptr = get_value(3);
  printf("STRO: %s\n",ptr);
};

void ADDSP(){
 
 u_short val = *get_value(3);
 rotate_short(&val);
 SP+=val;

};

void SUBSP(){
 
 u_short val = *get_value(3);
 rotate_short(&val);
 val *= -1;
 SP+=val;
 

};

void ADD(){

 u_short* reg = get_reg();
 u_short val = *get_value(3);
 u_char reg0 = get_bit(reg,15);
 u_char val0 = get_bit(val,15);

  
 rotate_short(reg);
 rotate_short(&val);
 u_short result = *reg + val;
 rotate_short(&result);
 u_char res0 = get_bit(result,15);
   
 C = ( (reg0 != val0) && !res0 ) || (reg0 & val0)
      || ( (reg0 ^ val0) && !res0 ); 
 V = (reg0 == val0) && (val0 != res0);
 N = get_bit(result,15);
 Z = (result  == 0);
 
 *reg = result;
};

void SUB(){

 u_short* reg = get_reg();
 u_short val = *get_value(3);
 u_char reg0 = get_bit(reg,15);
 
 rotate_short(&val);
 val = val * -1;
 rotate_short(reg);

 u_short result = *reg + val;
 rotate_short(&result);
 u_char res0 = get_bit(result,15);
 rotate_short(&val);
 u_short val0 = get_bit(val,15);
  
  
   
 C = ( (reg0 != val0) && !res0 ) || (reg0 & val0) 
     || ( (reg0 ^ val0) && !res0 );
 V = (reg0 == val0) && (val0 != res0);
 N = get_bit(result,15);
 Z = (result  == 0);
 
 *reg = result;
 


};

void AND(){
 
	u_short* reg = get_reg();
	u_short val = *get_value(3);
        rotate_short(&val);
        *reg &= val;

        N = (*reg<0);
        Z = (*reg==0);
	
}
void OR(){

	u_short* reg = get_reg();
	u_short val = *get_value(3);
        *reg |= val;
        N = (*reg<0);
        Z = (*reg==0);	

};

void CPW(){
  
 u_short reg = *get_reg();
 u_short val = *get_value(3);
 u_char reg0 = get_bit(reg,15);

 
 
 rotate_short(&reg);
 rotate_short(&val);
 val *=-1;
 u_short result = reg + val;
 rotate_short(&result);
 rotate_short(&val);
 u_char val0 = get_bit(val,15);
 u_char res0 = get_bit(result,15);
 
 C = ( (reg0 != val0) && !res0 ) || (reg0 & val0) 
       || ( (reg0 ^ val0) && !res0 );
 V = (reg0 == val0) && (val0 != res0);
 N = res0;
 Z = (result  == 0);
 N = N ^ V;


};

void CPB(){


 u_short reg = *(u_short*)get_reg();
 u_short val = *get_value(3);
 
 rotate_short(&reg);
 rotate_short(&val);
 val *=-1;
 reg &= 0x00FF;
 val &= 0x00FF;

 u_short result = reg + val;
 rotate_short(&result);
 result &= 0xFF00;
 
 C = get_bit(result,8);
 V = 0;
 Z = (result  == 0);
 N = get_bit(result,15);


};

void LDW(){

u_short* reg = get_reg();
u_short* val = get_value(3);
if((_CODE.value == 0xfc15) &&
 (_CODE.addr_mode == ADDR_d )){
  printf("need word input\n");
  *reg = getchar();
  getchar();
}else
  *reg = *val;

  
N = get_bit(*reg,15);
Z = (*reg == 0);
}

void LDB(){

u_char* reg = get_reg();
u_char* val = get_value(3);
//


//
if((_CODE.value == 0xfc15)
  &&(_CODE.addr_mode == ADDR_d)){
 printf("need byte input\n");
  reg[1] = getchar();
   getchar();
}
  else
    reg[1] = 
     (_CODE.addr_mode == ADDR_i) ? val[1] : val[0];

N = 0;
Z = (*reg == 0);

};

void STW(){

u_short* ptr = get_value(3);
u_short* reg = get_reg();
  *ptr = *reg;
   
if((_CODE.value == 0xfc16) &&
 (_CODE.addr_mode == ADDR_d )){
  printf("OUT_WORD: ");
  printf("%c\n",*reg); 
};

};


void STB(){

u_char* ptr = get_value(3);
u_char* reg = get_reg();
ptr[0] = reg[1];

if((_CODE.value == 0xfc16) &&
 (_CODE.addr_mode == ADDR_d )){
  printf("OUT_BYTE: ");
  printf("%c\n",reg[1]); 
};


};

const ENTRY OP_TABLE8[] = {
	    {STOP, "STOP"}, 
            {RET, "RET"},  
            {RETR, "RETR"},
            {MOVSPA, "MOVSPA"}, 
            {MOVFLGA, "MOVFLGA"}, 
	    {MOVAFLG, "MOVAFLG"}  
          };

const ENTRY OP_TABLE7[] = { {},{},{},
            {NOT ,"NOT"}, // IN_NOT = 3;
            {NEG ,"NEG"},
            {ASL ,"ASL"},
            {ASR ,"ASR"},
            {ROL ,"ROL"},
            {ROR ,"ROR"},
            {BR ,"BR"},
            {BRLE ,"BRLE"},
            {BRLT ,"BRLT"},
            {BREQ ,"BREQ"},
            {BRNE ,"BRNE"},
            {BRGE ,"BRGE"},
            {BRGT ,"BRGT"},
            {BRV ,"BRV"},
            {BRC ,"BRC"},
            {CALL ,"CALL"},
            {NOP ,"NOPn"}
           };

const ENTRY OP_TABLE5[] = { {},{},{},{},{},
            {NOP ,"NOP"}, // IN_NOP = 5;
            {DECI ,"DECI"},
            {DECO ,"DECO"},
            {HEXO ,"HEXO"},
            {STRO ,"STRO"},
            {ADDSP ,"ADDSP"},
            {SUBSP ,"SUBSP"}
           };

const ENTRY OP_TABLE4[] = { {},{},{},{},{},{},
            {ADD ,"ADD"},//IN_ADD = 6,
            {SUB ,"SUB"},
            {AND ,"AND"},
	    {OR ,"OR"},
	    {CPW ,"CPW"},
	    {CPB ,"CPB"},
	    {LDW ,"LDW"},
	    {LDB ,"LDB"},
	    {STW ,"STW"},
	    {STB ,"STB"}
          };






