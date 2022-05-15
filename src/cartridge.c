#include <stdio.h>
#include <stdlib.h>
#include "cartridge.h"

static cart_context ctx;

static char* NEW_LICENSEE_CODE[0xA5] = {
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s’pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)"
};

static char* CART_TYPE[] = {
    [0x00] = "ROM ONLY",
    [0x01] = "MBC1",
    [0x02] = "MBC1+RAM",
    [0x03] = "MBC1+RAM+BATTERY",
    [0x05] = "MBC2",
    [0x06] = "MBC2+BATTERY",
    [0x08] = "ROM+RAM 1",
    [0x09] = "ROM+RAM+BATTERY 1",
    [0x0B] = "MMM01",
    [0x0C] = "MMM01+RAM",
    [0x0D] = "MMM01+RAM+BATTERY",
    [0x0F] = "MBC3+TIMER+BATTERY",
    [0x10] = "MBC3+TIMER+RAM+BATTERY 2",
    [0x11] = "MBC3",
    [0x12] = "MBC3+RAM 2",
    [0x13] = "MBC3+RAM+BATTERY 2",
    [0x19] = "MBC5",
    [0x1A] = "MBC5+RAM",
    [0x1B] = "MBC5+RAM+BATTERY",
    [0x1C] = "MBC5+RUMBLE",
    [0x1D] = "MBC5+RUMBLE+RAM",
    [0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
    [0x20] = "MBC6",
    [0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
    [0xFC] = "POCKET CAMERA",
    [0xFD] = "BANDAI TAMA5",
    [0xFE] = "HuC3",
    [0xFF] = "HuC1+RAM+BATTERY"
};

static char* ROM_SIZE[] = {
    [0x00] = "32 KByte",
    [0x01] = "64 KByte",
    [0x02] = "128 KByte",
    [0x03] = "256 KByte",
    [0x04] = "512 KByte",
    [0x05] = "1 MByte",
    [0x06] = "2 MByte",
    [0x07] = "4 MByte",
    [0x08] = "8 MByte",
    [0x52] = "1.1 MByte",
    [0x53] = "1.2 MByte",
    [0x54] = "1.5 MByte"
};

static char* RAM_SIZE[] = {
    [0x00] = "0",
    [0x01] = "-",
    [0x02] = "8 KB",
    [0x03] = "32 KB",
    [0x04] = "128 KB",
    [0x05] = "64 KB"
};

const char* cart_lic_name() {
    if(ctx.header->new_licensee_code <= 0xA4) {
        return NEW_LICENSEE_CODE[ctx.header->new_licensee_code];
    }

    return "UNKNOWN";
}

bool cart_load(char *cart) {
    snprintf(ctx.filename, sizeof(ctx.filename), "%s", cart);

    FILE* cart_file = fopen(cart, "r");
    
    if(!cart_file) {
        printf("Error! Could not open file %s\n", cart);
        return false;
    }

    printf("Opened : %s\n", ctx.filename);


    // ????????????????????????
    fseek(cart_file, 0, SEEK_END);
    ctx.rom_size = ftell(cart_file);

    rewind(cart_file);
    // ????????????????????????

    ctx.rom_data = malloc(ctx.rom_size);
    fread(ctx.rom_data, ctx.rom_size, 1, cart_file);
    fclose(cart_file);

    ctx.header = (header*)(ctx.rom_data + 0x0100);
    ctx.header->title[15] = 0;

    printf("Cartridge loaded :\n");
    printf("\tTitle : %s\n", ctx.header->title);
    printf("\tType : %X (%s)\n", ctx.header->cart_type, CART_TYPE[ctx.header->cart_type]);
    printf("\tRom Size : %X (%s)\n", ctx.header->rom_size, ROM_SIZE[ctx.header->rom_size]);
    printf("\tRam Size : %X (%s)\n", ctx.header->ram_size, RAM_SIZE[ctx.header->ram_size]);
    printf("\tLIC Code : %X (%s)\n", ctx.header->new_licensee_code, cart_lic_name());
    printf("\tRom Vers : %X (%s)\n", ctx.header->mask_rom_version_nb);

    int x = 0;
    u16 i = 0x014C;
    bool passed = false;
    while(i <= ctx.rom_data[0x014C]) {
        x = x - i - 1;
    }

    printf("\tChecksum : %X (%s)\n", ctx.header->header_checksum, x == ctx.rom_data[0x014C] ? "PASSED" : "NOT PASSED");
    if(x != ctx.rom_data[0x014C]) {
        return false;
    }

    return true;
}

u8 cart_read(u16 address) {
    return ctx.rom_data[address];
}

void card_write(u16 address, u8 value) {
    //ctx.rom_data[address] = value;
    printf("No implementation\n");
    exit(-2);
}