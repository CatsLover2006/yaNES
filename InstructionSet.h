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
#define JSR     0x30
#define SEC     0x38
#define RTI     0x40
#define PHA     0x48
#define JMP     0x4C
#define CLI     0x58
#define RTS     0x60
#define PLA     0x68
#define ROR_A   0x6A
#define JMP_I   0x6C
#define SEI     0x78
#define STY_0   0x84
#define STA_0   0x85
#define STX_0   0x86
#define DEY     0x88
#define TXA     0x8A
#define STY_0X  0x94
#define STA_0X  0x95
#define STX_0Y  0x96
#define TYA     0x98
#define TXS     0x9A
#define LDY     0xA0
#define LDX     0xA2
#define LDY_0   0xA4
#define LDA_0   0xA5
#define LDX_0   0xA6
#define LAX_0   0xA7 // Illegal Opcode
#define TAY     0xA8
#define LDA     0xA9
#define TAX     0xAA
#define LXA     0xAB // Illegal Opcode
#define LDY_0X  0xB4
#define LDA_0X  0xB5
#define LDX_0Y  0xB6
#define LAX_0Y  0xB7 // Illegal Opcode
#define CLV     0xB8
#define TSX     0xBA
#define DEX     0xCA
#define INY     0xC8
#define CLD     0xD8
#define INX     0xE8
#define NOP     0xEA
#define SED     0xF8

#endif /* InstructionSet_h */
