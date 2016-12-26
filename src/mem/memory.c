#include <mem/memory.h>
#include <ctx/types.h>

unsigned short g_cp8_mmap[CP8_MEMORY_SZ];

void set_maddr(unsigned short addr, unsigned short value) {
    g_cp8_mmap[addr] = value;
}

unsigned short get_maddr(unsigned short addr) {
    return g_cp8_mmap[addr];
}
