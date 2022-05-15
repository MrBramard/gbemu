#include "common.h"

typedef struct {
    u8 entry[4];
    u8 logo[0x30];
    char title[16];
    u16 new_licensee_code;
    u8 sgb_flag;
    u8 cart_type;
    u8 rom_size;
    u8 ram_size;
    u8 destination_code;
    u8 old_licensee_code;
    u8 mask_rom_version_nb;
    u8 header_checksum;
    u16 global_checksum;
} header; 

typedef struct {
    char filename[1024];
    u32 rom_size;
    u8 *rom_data;
    header *header;
} cart_context;

bool cart_load(char *cart);
u8 cart_read(u16 address);
void cart_write(u16 address, u8 value);