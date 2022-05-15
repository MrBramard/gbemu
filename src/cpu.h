#include "common.h"

typedef struct {
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 sp;
    u16 pc;
} registers;

typedef struct {
    u16 curr_address;
    u16 next_address;
    u8 curr_opcode;
} cpu_context;