#pragma once            // Avoiding header files calling clashes

#include "bus.h"
#include <string>
#include <vector>
#include <map>

// Basically, we will use bus so CPU can communicate with RAM and other connected devicies to it.

class Bus;              // Declared a Bus class to connect to CPU

class olc6502
{
public:
    olc6502();
    ~olc6502();

    void ConnectBus(Bus *n)
    {
        bus = n;                // We take a pointer arg of Bus class in function and set the bus point equal to it
    }

    // Produces a map of strings, with keys equivalent to instruction start locations
	// in memory, for the specified address range
	std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);

public:
    enum FLAGS6502        // Created an enum for the bits of the status register
    {
        C = (1 << 0),     // Carry bit
        Z = (1 << 1),     // Zero
        I = (1 << 2),     // Disable Interrupts
        D = (1 << 3),     // Decimal Mode (not used in this implementation)
        B = (1 << 4),     // Break
        U = (1 << 5),     // Unused
        V = (1 << 6),     // Overflow
        N = (1 << 7),     // Negative
    };

    // Other registers:
    uint8_t a = 0x00;           // Accumulator Register
    uint8_t x = 0x00;           // X Register
    uint8_t y = 0x00;           // Y Register
    uint8_t stkp = 0x00;        // Stack pointer pointing to location on bus
    uint16_t pc = 0x0000;       // Program Counter
    uint8_t status = 0x00;      // Status Register

    // Addressing Modes:
    uint8_t IMP();  uint8_t IMM();
    uint8_t ZP0();  uint8_t ZPX();
    uint8_t ZPY();  uint8_t REL();
    uint8_t ABS();  uint8_t ABX();
    uint8_t ABY();  uint8_t IND();
    uint8_t IZX();  uint8_t IZY();

    // Opcodes:
    uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

    uint8_t XXX(); // For capturing illegal opcodes

    void clock();
    void reset();
    void irq();
    void nmi();

    // Indicates the current instruction has completed by returning true. This is
	// a utility function to enable "step-by-step" execution, without manually 
	// clocking every cycle
	bool complete();

    // Intructions work with data needs to fetch data from given source. So we will fetch data with fetch function and will store the data temporarily in our fetched variable
    uint8_t fetch();
    uint8_t fetched = 0x00;

    // Depending on Addressing Mode, we need to read from defferent locations of the memory
    uint16_t addr_abs = 0x0000;         // Storing the location address
    uint16_t temp        = 0x0000; // A convenience variable used everywhere
    uint16_t addr_rel = 0x0000;         // All used memory addresses end up in here
    uint8_t  opcode   = 0x00;           // Opcode we are currently working with
    uint8_t  cycles   = 0;              // Cycles left for the duration of this instruction
    uint32_t clock_count = 0;	   // A global accumulation of the number of clocks


private:
    Bus *bus = nullptr;         // Declaring bus pointer of Bus class

    // Read and Write functions of CPU
    uint8_t read(uint16_t addr);    
    void write(uint16_t addr, uint8_t data);

    // Convenience function to access status register
    uint8_t GetFlag(FLAGS6502 f);
    void    SetFlag(FLAGS6502 f, bool v);

    // The Instruction will look like this:
    struct INSTRUCTION
    {
        std::string name;                               // Mnemonic of the instruction
        uint8_t (olc6502::*operate)(void) = nullptr;    // opcode function pointer
        uint8_t (olc6502::*addrmode)(void) = nullptr;   // address mode function pointer
        uint8_t cycles   =    0;                        // clock counts needed for instruction to complete
    };

    std::vector<INSTRUCTION> lookup;                    // Storing all Instruction in a vector

};