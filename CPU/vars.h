

enum addr_modes{

	ADDR_i,
	ADDR_d,
	ADDR_n,
	ADDR_s,
	ADDR_sf,
	ADDR_x,
	ADDR_sx,
	ADDR_sfx

};


 enum bit8{ IN_STOP, //0
            IN_RET,  //1
            IN_RETR, //2
            IN_MOVSPA, //3
            IN_MOVFLGA, //4
            IN_MOVAFLG  //5
          };

 enum bit7{ IN_NOT = 3,
            IN_NEG,
            IN_ASL,
            IN_ASR,
            IN_ROL,
            IN_ROR,
            IN_BR,
            IN_BRLE,
            IN_BRLT,
            IN_BREQ,
            IN_BRNE,
            IN_BRGE,
            IN_BRGT,
            IN_BRV,
            IN_BRC,
            IN_CALL,
            IN_NOPn
           };

 enum bit5{ IN_NOP = 5,
            IN_DECI,
            IN_DECO,
            IN_HEXO,
            IN_STRO,
            IN_ADDSP,
            IN_SUBSP
           };


 enum bit4{ IN_ADD = 6,
            IN_SUB,
            IN_AND,
	    IN_OR,
	    IN_CPW,
	    IN_CPB,
	    IN_LDW,
	    IN_LDB,
	    IN_STW,
	    IN_STB
          };

typedef struct { void* f_ptr;char f_name[10]; }ENTRY;
typedef unsigned short u_short;
typedef unsigned char  u_char;

const ENTRY OP_TABLE8[];
const ENTRY OP_TABLE7[];
const ENTRY OP_TABLE5[];
const ENTRY OP_TABLE4[];


u_char Mem[65536];
u_short REG_A;
u_short REG_X;
u_short SP;
u_short PC;
u_short N,Z,V,C;
u_short* IP;
	 
typedef struct {
  
  u_char addr_mode:3;
  u_char reg:1;
  u_char main:4;
  u_short value:16;
  
  
}CODE;

CODE _CODE;
u_short* _RET_VAL;

