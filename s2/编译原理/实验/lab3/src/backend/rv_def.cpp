#include "backend/rv_def.h"

/* ---------- rv_def.h ---------- */
std::string rv::toString(rv::rvREG r)
{
    switch (r)
    {
    case rv::rvREG::ZERO:
        return "zero";
    case rv::rvREG::RA:
        return "ra";
    case rv::rvREG::SP:
        return "sp";
    case rv::rvREG::GP:
        return "gp";
    case rv::rvREG::TP:
        return "tp";
    case rv::rvREG::T0:
        return "t0";
    case rv::rvREG::T1:
        return "t1";
    case rv::rvREG::T2:
        return "t2";
    case rv::rvREG::S0:
        return "s0";
    case rv::rvREG::S1:
        return "s1";
    case rv::rvREG::A0:
        return "a0";
    case rv::rvREG::A1:
        return "a1";
    case rv::rvREG::A2:
        return "a2";
    case rv::rvREG::A3:
        return "a3";
    case rv::rvREG::A4:
        return "a4";
    case rv::rvREG::A5:
        return "a5";
    case rv::rvREG::A6:
        return "a6";
    case rv::rvREG::A7:
        return "a7";
    case rv::rvREG::S2:
        return "s2";
    case rv::rvREG::S3:
        return "s3";
    case rv::rvREG::S4:
        return "s4";
    case rv::rvREG::S5:
        return "s5";
    case rv::rvREG::S6:
        return "s6";
    case rv::rvREG::S7:
        return "s7";
    case rv::rvREG::S8:
        return "s8";
    case rv::rvREG::S9:
        return "s9";
    case rv::rvREG::S10:
        return "s10";
    case rv::rvREG::S11:
        return "s11";
    case rv::rvREG::T3:
        return "t3";
    case rv::rvREG::T4:
        return "t4";
    case rv::rvREG::T5:
        return "t5";
    case rv::rvREG::T6:
        return "t6";
    default:
        return "Error rvREG";
    }
}

std::string rv::toString(rv::rvFREG r)
{
    switch (r)
    {
    case rv::rvFREG::F0:
        return "f0";
    case rv::rvFREG::F1:
        return "f1";
    case rv::rvFREG::F2:
        return "f2";
    case rv::rvFREG::F3:
        return "f3";
    case rv::rvFREG::F4:
        return "f4";
    case rv::rvFREG::F5:
        return "f5";
    case rv::rvFREG::F6:
        return "f6";
    case rv::rvFREG::F7:
        return "f7";
    case rv::rvFREG::FS0:
        return "fs0";
    case rv::rvFREG::FS1:
        return "fs1";
    case rv::rvFREG::FA0:
        return "fa0";
    case rv::rvFREG::FA1:
        return "fa1";
    case rv::rvFREG::FA2:
        return "fa2";
    case rv::rvFREG::FA3:
        return "fa3";
    case rv::rvFREG::FA4:
        return "fa4";
    case rv::rvFREG::FA5:
        return "fa5";
    case rv::rvFREG::FA6:
        return "fa6";
    case rv::rvFREG::FA7:
        return "fa7";
    case rv::rvFREG::FS2:
        return "fs2";
    case rv::rvFREG::FS3:
        return "fs3";
    case rv::rvFREG::FS4:
        return "fs4";
    case rv::rvFREG::FS5:
        return "fs5";
    case rv::rvFREG::FS6:
        return "fs6";
    case rv::rvFREG::FS7:
        return "fs7";
    case rv::rvFREG::FS8:
        return "fs8";
    case rv::rvFREG::FS9:
        return "fs9";
    case rv::rvFREG::FS10:
        return "fs10";
    case rv::rvFREG::FS11:
        return "fs11";
    case rv::rvFREG::FT8:
        return "ft8";
    case rv::rvFREG::FT9:
        return "ft9";
    case rv::rvFREG::FT10:
        return "ft10";
    case rv::rvFREG::FT11:
        return "ft11";
    default:
        return "Error rvFREG";
    }
}