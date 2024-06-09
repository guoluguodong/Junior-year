#ifndef RV_DEF_H
#define RV_DEF_H

#include<string>

namespace rv {

// rv interger registers
enum class rvREG {
    ZERO,
    RA,
    SP,
    GP,
    TP,
    T0,
    T1,
    T2,
    S0,
    S1,
    A0,
    A1,
    A2,
    S5,
    S6,
    S7,
    A3,
    A4,
    A5,
    A6,
    A7,
    S2,
    S3,
    S4,
    S8,
    S9,
    S10,
    S11,
    T3,
    T4,
    T5,
    T6

};
std::string toString(rvREG r);
enum class rvFREG {
    F0,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    FS0,
    FS1,
    FA0,
    FA1,
    FA2,
    FA3,
    FA4,
    FA5,
    FA6,
    FA7,
    FS2,
    FS3,
    FS4,
    FS5,
    FS6,
    FS7,
    FS8,
    FS9,
    FS10,
    FS11,
    FT8,
    FT9,
    FT10,
    FT11,
};

std::string toString(rvFREG r);

enum class rvOPCODE {
    // RV32I Base Integer Instructions
    ADD, SUB, XOR, OR, AND, SLL, SRL, SRA, SLT, SLTU,       // arithmetic & logic
    ADDI, XORI, ORI, ANDI, SLLI, SRLI, SRAI, SLTI, SLTIU,   // immediate
    LW, SW,                                                 // load & store
    BEQ, BNE, BLT, BGE, BLTU, BGEU,                         // conditional branch
    JAL, JALR,                                              // jump

    // RV32M Multiply Extension

    // RV32F / D Floating-Point Extensions

    // Pseudo Instructions
    LA, LI, MOV, J,                                     
};
std::string toString(rvOPCODE r); 

} // namespace rv

#endif