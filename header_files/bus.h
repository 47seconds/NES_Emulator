#pragma once            // Avoiding header files calling clashes

#include <cstdint>      // Using extensive units for memory allocation
#include "olc6502.h"
#include <array>

// Basically, we will use bus so CPU can communicate with RAM and other connected devicies to it.

class Bus
{
public:
    Bus();
    ~Bus();

public:                 // Devices connected to this bus
    olc6502 cpu;        // Our CPU

    // Fake RAM of 64KB for now ----> uint8_t = 8bit = 1byte (unit), 64*1024 = 64 * 1 Kilo Byte (1024*1 byte (8bits))
    std::array<uint8_t, 64*1024> ram;


public:                // Bus used to read or write data
    void write(uint16_t addr, uint8_t data);     // 16 bit location address and 8bit data to be written
    uint8_t read(uint16_t addr, bool bReadonly = false);    // 16 bit location address to read from

};