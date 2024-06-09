#ifndef GENERARATOR_H
#define GENERARATOR_H

#include "ir/ir.h"
#include "backend/rv_def.h"
#include "backend/rv_inst_impl.h"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>
using namespace std;
namespace backend
{

  struct StackVarMap
  {
    unordered_map<string, int> _table;
  };
  struct CalleeSavedReg
  {
    vector<rv::rvREG> _calleeSavedReg{rv::rvREG::S0, rv::rvREG::S1, rv::rvREG::S2, rv::rvREG::S3, rv::rvREG::S4,
                                      rv::rvREG::S5, rv::rvREG::S6, rv::rvREG::S7, rv::rvREG::S8, rv::rvREG::S9,
                                      rv::rvREG::S10, rv::rvREG::S11, rv::rvREG::RA};
  };
  struct IntRegs
  {
    vector<rv::rvREG> _intRegs{rv::rvREG::T0, rv::rvREG::T1, rv::rvREG::T2, rv::rvREG::T3, rv::rvREG::T4,
                               rv::rvREG::T5, rv::rvREG::T6, rv::rvREG::S0, rv::rvREG::S1, rv::rvREG::S2,
                               rv::rvREG::S3, rv::rvREG::S4, rv::rvREG::S5, rv::rvREG::S6, rv::rvREG::S7,
                               rv::rvREG::S8, rv::rvREG::S9, rv::rvREG::S10, rv::rvREG::S11};
  };
  struct IntPremRegs
  {
    vector<rv::rvREG> _intPremRegs{rv::rvREG::A0, rv::rvREG::A1, rv::rvREG::A2, rv::rvREG::A3, rv::rvREG::A4,
                                   rv::rvREG::A5, rv::rvREG::A6, rv::rvREG::A7};
  };
  struct CalleeSavedFloatReg
  {
    vector<rv::rvFREG> _calleeSavedFloatReg{rv::rvFREG::FS0, rv::rvFREG::FS1, rv::rvFREG::FS2, rv::rvFREG::FS3,
                                            rv::rvFREG::FS4, rv::rvFREG::FS5, rv::rvFREG::FS6, rv::rvFREG::FS7,
                                            rv::rvFREG::FS8, rv::rvFREG::FS9, rv::rvFREG::FS10, rv::rvFREG::FS11};
  };
  struct FloatPremRegs
  {
    vector<rv::rvFREG> _floatPremRegs{rv::rvFREG::FA0, rv::rvFREG::FA1, rv::rvFREG::FA2, rv::rvFREG::FA3,
                                      rv::rvFREG::FA4, rv::rvFREG::FA5, rv::rvFREG::FA6, rv::rvFREG::FA7};
  };
  struct FloatRegs
  {
    vector<rv::rvFREG> _floatRegs{rv::rvFREG::FS0, rv::rvFREG::FS1, rv::rvFREG::FS2, rv::rvFREG::FS3, rv::rvFREG::FS4,
                                  rv::rvFREG::FS5, rv::rvFREG::FS6, rv::rvFREG::FS7, rv::rvFREG::FS8, rv::rvFREG::FS9,
                                  rv::rvFREG::FS10, rv::rvFREG::FS11, rv::rvFREG::FT8, rv::rvFREG::FT9,
                                  rv::rvFREG::FT10, rv::rvFREG::FT11};
  };

  struct Generator
  {
    const ir::Program &program;
    ofstream &fout;
    int stack_size;
    unordered_set<rv::rvREG> free_int_regs;
    unordered_set<rv::rvFREG> free_float_regs;
    int IRPC;
    int label_count = 0;
    unordered_map<int, string> jump_label_map;
    int stackDistrustedBytes;
    StackVarMap *stackVarMap = new StackVarMap();

    CalleeSavedReg *calleeSavedReg = new CalleeSavedReg;
    IntRegs intRegs;
    IntPremRegs intPremRegs;

    CalleeSavedFloatReg *calleeSavedFloatReg = new CalleeSavedFloatReg();
    FloatRegs floatRegs;
    FloatPremRegs floatPremRegs;
    bool previousInstrIsNeq = false;

    Generator(ir::Program &, ofstream &);
    
    void gen();
    /*
      solve_global_data
          processIntFloatDeclarations
          processInitializedArrays
          processUninitializedArrays
    */
    bool is_global(const string &);
    void solve_global_data();
    void processIntFloatDeclarations(const std::vector<ir::Instruction *> &, std::ofstream &);
    void processInitializedArrays(std::unordered_map<std::string, ir::GlobalVal> &,
                                  const std::vector<ir::Instruction *> &, std::ofstream &);
    void processUninitializedArrays(const unordered_map<string, ir::GlobalVal> &, ofstream &);

    void solve_func(const ir::Function &);
    void initFunc(const ir::Function *);
    void resetState();
    unordered_set<string> collectOperands(const ir::Function *function);
    void saveCalleeRegisters(int &, const ir::Function *);
    void saveFunctionParameters(int &, const ir::Function *function);
    void allocateStackSpaceForOperands(int &currentOffset, const ir::Function *function);
    void processInstructions(const ir::Function &func);

    rv::rvREG allocLoadintReg(ir::Operand);
    rv::rvREG allocIntReg();
    void freeIntReg(rv::rvREG);
    void saveIntOperand(ir::Operand, rv::rvREG);

    rv::rvFREG allocLoadfloatReg(ir::Operand);
    rv::rvFREG allocFloatReg();
    void freeFloatReg(rv::rvFREG);
    void saveFloatOperand(ir::Operand, rv::rvFREG);
    // alu 运算
    void generate_alu(const ir::Instruction &);

    // 逻辑运算
    void generateComparison(const ir::Instruction &);

    // 访存与指针
    void generate_alloc(const ir::Instruction &);
    void generate_address_calculation(const ir::Instruction &, rv::rvREG &, rv::rvREG &, int &);
    rv::rvREG generate_destination_register(const ir::Operand &);
    void generate_ld_sw(const ir::Instruction &);
    void generate_getptr(const ir::Instruction &);

    // 调用返回
    void generate__return(const ir::Instruction &);
    void storeOperandOnStack(const ir::Operand &, int);
    void storeOperand(const ir::Operand &, int);
    void generate_call(const ir::Instruction &);

    // goto
    void generate__goto(const ir::Instruction &);

    // 移位
    void generate_mov(const ir::Instruction &);

    // 空
    void generate___unuse__(const ir::Instruction &);
    void generate___cvt_f2i(const ir::Instruction &);
    void generate___cvt_i2f(const ir::Instruction &);
    using AddFuncPtr = void (Generator::*)(const ir::Instruction &);
  };
}

#endif
