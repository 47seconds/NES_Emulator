#include "bus.h"

Bus::Bus()
{
    // Clearing RAM in case something is present
    for(auto &i : ram) i = 0x00;     // for i in ram, set i = 0x00

    // Connect CPU to the current instance of bus, i.e. this bus
    cpu.ConnectBus(this);
}

Bus::~Bus()
{
}

void Bus::write(uint16_t addr, uint8_t data)
{
    if (addr >= 0x0000 && addr <= 0xFFFF)       // Range of RAM, i.e. full 64KB
    ram[addr] = data;                          // Write data in RAM(only connected device rn to Bus) at specified address
}

uint8_t Bus::read(uint16_t addr, bool bReadonly = false)
{
    if (addr >= 0x0000 && addr <= 0xFFFF)
    return ram[addr];                          // Returned whatever was stored at that location in RAM

    return 0x00;                               // In case we make it outside range, we can return 0
}