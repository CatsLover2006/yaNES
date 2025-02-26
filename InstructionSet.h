//
//  InstructionSet.h
//  
//
//  Created by Chance Parsons on 2025-02-24.
//

// ISA Reference: https://www.masswerk.at/6502/6502_instruction_set.html

#ifndef InstructionSet_6502_h
#define InstructionSet_6502_h

#define BRK     0x00
#define PHP     0x08
#define ASL_A   0x0A
#define CLC     0x18
#define PLP     0x28
#define ROL_A   0x2A
#define SEC     0x38
#define RTI     0x40
#define PHA     0x48
#define CLI     0x58
#define PLA     0x48
#define SEI     0x78
#define DEY     0x88
#define LDA     0xA9
#define CLV     0xB8
#define DEX     0xCA
#define INY     0xC8
#define CLD     0xD8
#define INX     0xE8
#define NOP     0xEA
#define SED     0xF8

#endif /* InstructionSet_h */
