//
//  InstructionSet.h
//  
//
//  Created by Chance Parsons on 2025-02-24.
//

// ISA Reference: https://www.masswerk.at/6502/6502_instruction_set.html
// Assumes NMOS

#ifndef InstructionSet_6502_h
#define InstructionSet_6502_h

#define BRK     0x00
#define ORA_I   0x01
#define ORA_0   0x05
#define PHP     0x08
#define ORA     0x09
#define ASL     0x0A
#define ANC     0x0B // Illegal Opcode
#define ORA_A   0x0D
#define ORA_IY  0x11
#define ORA_0X  0x15
#define CLC     0x18
#define ORA_AY  0x19
#define ORA_AX  0x1D
#define AND_I   0x21
#define BIT_0   0x24
#define AND_0   0x25
#define PLP     0x28
#define AND     0x29
#define ROL     0x2A
#define ANC_R   0x2B // Illegal Opcode
#define BIT_A   0x2C
#define AND_A   0x2D
#define JSR     0x30
#define AND_IY  0x31
#define AND_0X  0x35
#define SEC     0x38
#define AND_AY  0x39
#define AND_AX  0x3D
#define RTI     0x40
#define EOR_I   0x41
#define EOR_0   0x45
#define PHA     0x48
#define EOR     0x49
#define LSR     0x4A
#define ALR     0x4B // Illegal Opcode
#define JMP     0x4C
#define EOR_A   0x4D
#define EOR_IY  0x51
#define EOR_0X  0x55
#define CLI     0x58
#define EOR_AY  0x59
#define EOR_AX  0x5D
#define RTS     0x60
#define ADC_I   0x61
#define ADC_0   0x65
#define PLA     0x68
#define ADC     0x69
#define ROR     0x6A
#define ARR     0x6B // Illegal Opcode
#define JMP_I   0x6C
#define ADC_A   0x6D
#define ADC_0X  0x75
#define SEI     0x78
#define ADC_AY  0x79
#define ADC_AX  0x7D
#define STY_0   0x84
#define STA_0   0x85
#define STX_0   0x86
#define DEY     0x88
#define TXA     0x8A
#define XAA     0x8B // Illegal Opcode
#define STY_0X  0x94
#define STA_0X  0x95
#define STX_0Y  0x96
#define TYA     0x98
#define TXS     0x9A
#define LDY     0xA0
#define LDA_I   0xA1
#define LDX     0xA2
#define LAX_I   0xA3 // Illegal Opcode
#define LDY_0   0xA4
#define LDA_0   0xA5
#define LDX_0   0xA6
#define LAX_0   0xA7 // Illegal Opcode
#define TAY     0xA8
#define LDA     0xA9
#define TAX     0xAA
#define LXA     0xAB // Illegal Opcode
#define LDY_A   0xAC
#define LDA_A   0xAD
#define LDX_A   0xAE
#define LAX_A   0xAF // Illegal Opcode
#define LDA_IY  0xB1
#define LAX_IY  0xB3 // Illegal Opcode
#define LDY_0X  0xB4
#define LDA_0X  0xB5
#define LDX_0Y  0xB6
#define LAX_0Y  0xB7 // Illegal Opcode
#define CLV     0xB8
#define LDA_AY  0xB9
#define TSX     0xBA
#define LDY_AX  0xBC
#define LDA_AX  0xBD
#define LDX_AY  0xBE
#define LAX_AY  0xBF // Illegal Opcode
#define CPY     0xC0
#define CMP_I   0xC1
#define CPY_0   0xC4
#define CMP_0   0xC5
#define INY     0xC8
#define CMP     0xC9
#define DEX     0xCA
#define CPY_A   0xCC
#define CMP_A   0xCD
#define CMP_IY  0xD1
#define CMP_0X  0xD5
#define CLD     0xD8
#define CMP_AY  0xD9
#define CMP_AX  0xDD
#define CPX     0xE0
#define SBC_I   0xE1
#define CPX_0   0xE4
#define SBC_0   0xE5
#define INX     0xE8
#define SBC     0xE9
#define NOP     0xEA
#define USBC    0xEB // Illegal Opcode
#define CPX_A   0xEC
#define SBC_A   0xED
#define SBC_IY  0xF1
#define SBC_0X  0xF5
#define SED     0xF8
#define SBC_AY  0xF9
#define SBC_AX  0xFD

#endif /* InstructionSet_h */
