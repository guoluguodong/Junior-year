#include "backend/generator.h"
#include "backend/rv_def.h"

#include <assert.h>
#include <unordered_map>
#include <map>
#include <functional> // 用于std::function
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <sstream>
#define TODO // assert(0 && "todo")

// 定义一个函数指针类型，该函数指针指向 backend::Generator 类的成员函数
typedef void (backend::Generator::*GenerateFunc)(const ir::Instruction &);

// 协助处理一些重复逻辑的函数
string deletefloat(string s)
{
    string a = s;
    size_t dotPos = s.find('.');
    if (dotPos != string::npos)
    { // 如果找到小数点
      // 删除小数点及其后面的部分
        a = s.substr(0, dotPos);
    }
    return a;
}
// 声明变量
void outputGlobalDeclaration(ofstream &fout, const string &name, int size)
{
    fout << "\t.globl\t" << name << "\n";
    fout << "\t.type\t" << name << ", @object\n";
    fout << "\t.size\t" << name << ", " << size << "\n";
    fout << "\t.align\t" << "4\n";
}
// 写函数头
void writeFunctionHeader(ofstream &fout, const string &functionName)
{
    fout << "\t.globl\t" << functionName << "\n";
    fout << "\t.type\t" << functionName << ", @function\n";
    fout << functionName << ":\n";
}

void setLabel(ofstream &fout, const string &lable)
{
    // 汇编中包含label
    fout << lable << ":\n";
}

void setWord(ofstream &fout, string value)
{
    fout << "\t.word\t" << value << "\n";
}
void setFloat(ofstream &fout, string value)
{
    fout << "\t.float\t" << value << "\n";
}

// TODO
string binaryToUnsignedIntString(string binary)
{
    bitset<32> bits(binary);
    unsigned int unsignedInt = bits.to_ulong();
    return to_string(unsignedInt);
}
// 将浮点数转换为IEEE 754格式的二进制字符串
string floatToLong(float num)
{
    stringstream ss;
    union
    {
        float input;
        int output;
    } data;
    data.input = num;
    bitset<sizeof(float) * 8> bits(data.output);
    ss << bits;
    return binaryToUnsignedIntString(ss.str());
}
string bit_extend(float a)
{
    uint32_t bits;
    // 使用联合体来查看浮点数的位模式
    union
    {
        float f;
        uint32_t u;
    } float_union;
    float_union.f = a;
    bits = float_union.u;
    char octalStr[9];
    snprintf(octalStr, sizeof(octalStr), "%08X", float_union.u);
    char finalStr[13]; // "0x" + 8位16进制字符串 + 终止符
    snprintf(finalStr, sizeof(finalStr), "0x%s", octalStr);
    return finalStr;
}

// backend::Generator函数定义
backend::Generator::Generator(ir::Program &p, ofstream &f) : program(p), fout(f) {}

void backend::Generator::gen()
{
    // 处理全局变量
    solve_global_data();
    // 进入代码段
    fout << "\t.text\n";
    // 按顺序处理每个函数
    for (int i = 0; i < int(this->program.functions.size()) - 1; i++)
        solve_func(this->program.functions[i]);
}
// 判断全局变量
bool backend::Generator::is_global(const string &operandName)
{
    return any_of(
        this->program.globalVal.begin(),
        this->program.globalVal.end(),
        [operandName](const auto &globalval)
        { return globalval.val.name == operandName; });
}
// ============================初始化全局变量===========================
void backend::Generator::solve_global_data()
{
    // 处理全局变量（包括整型/浮点型变量/数组）
    fout << "\t.data\n";
    const ir::Function &globalFunc = this->program.functions.back(); // 语义分析中最后一个是最大的全局函数
    processIntFloatDeclarations(globalFunc.InstVec, fout);
    unordered_map<string, ir::GlobalVal> arrayInitialVals;
    for (const ir::GlobalVal &arrayVal : this->program.globalVal)
    {
        if (arrayVal.val.type == ir::Type::IntPtr)
        {
            arrayInitialVals.emplace(arrayVal.val.name, arrayVal);
        }
    }
    processInitializedArrays(arrayInitialVals, globalFunc.InstVec, fout);
    processUninitializedArrays(arrayInitialVals, fout);
}

// 处理整型与浮点型变量声明
void backend::Generator::processIntFloatDeclarations(const vector<ir::Instruction *> &instructions, ofstream &fout)
{
    for (auto instrIter = instructions.begin(); instrIter != instructions.end(); ++instrIter)
    {
        const ir::Instruction *instr = *instrIter;
        ir::Operand dest = instr->des;
        ir::Operand op1 = instr->op1;
        ir::Operand op2 = instr->op2;
        ir::Operator oper = instr->op;
        bool isDefOperation = (oper == ir::Operator::def || oper == ir::Operator::fdef);
        if (!isDefOperation)
        {
            continue;
        }
        outputGlobalDeclaration(fout, dest.name, 4);
        setLabel(fout, dest.name);

        if (dest.type == ir::Type::Int || dest.type == ir::Type::IntLiteral)
        {
            string value = deletefloat(op1.name);
            setWord(fout, value);
        }
        else if (dest.type == ir::Type::Float || dest.type == ir::Type::FloatLiteral)
        {
            string value = op1.name;
            setFloat(fout, value);
        }
    }
}
// 初始化數組
void backend::Generator::processInitializedArrays(unordered_map<string, ir::GlobalVal> &arrayInitialVals,
                                                  const vector<ir::Instruction *> &instructions, ofstream &fout)
{
    for (auto instrIter = instructions.begin(); instrIter != instructions.end(); ++instrIter)
    {
        const ir::Instruction *instr = *instrIter;
        ir::Operand dest = instr->des;
        ir::Operand op1 = instr->op1;
        ir::Operand op2 = instr->op2;
        ir::Operator oper = instr->op;

        if (oper == ir::Operator::store)
        {
            auto arrayValIter = arrayInitialVals.find(op1.name);
            if (arrayValIter != arrayInitialVals.end())
            {
                ir::GlobalVal &arrayVal = arrayValIter->second;
                int length = arrayVal.maxlen;
                outputGlobalDeclaration(fout, op1.name, length * 4);
                setLabel(fout, op1.name);
                arrayInitialVals.erase(arrayValIter);
            }

            if (dest.type == ir::Type::IntLiteral || dest.type == ir::Type::Int)
            {
                string value = deletefloat(dest.name);
                setWord(fout, value);
            }
            else if (dest.type == ir::Type::Float || dest.type == ir::Type::FloatLiteral)
            {
                string value = dest.name;
                setWord(fout, value);
            }
        }
    }
}
// 将所有没有进行初始化的数组放入bss中
void backend::Generator::processUninitializedArrays(const unordered_map<string, ir::GlobalVal> &arrayInitialVals, ofstream &fout)
{
    if (arrayInitialVals.size() > 0)
    {
        fout << "\t.bss\n";
        for (auto it = arrayInitialVals.begin(); it != arrayInitialVals.end(); ++it)
        {
            const string &arrayName = it->first;
            const ir::GlobalVal &arrayVal = it->second;
            int sizeInBytes = arrayVal.maxlen * 4;

            outputGlobalDeclaration(fout, arrayName, sizeInBytes);
            setLabel(fout, arrayName);
            fout << "\t.space\t" << sizeInBytes << "\n";
        }
    }
}

// ============================初始化函数===========================
void backend::Generator::solve_func(const ir::Function &func)
{
    writeFunctionHeader(fout, func.name);
    initFunc(&func);
    processInstructions(func);
}

// 进入函数，清空栈、跳转表、寄存器
void backend::Generator::resetState()
{
    stackVarMap->_table.clear();
    jump_label_map.clear();
    IRPC = -1;
    stackDistrustedBytes = 0;
    free_int_regs.clear();
    free_int_regs.insert(this->intRegs._intRegs.begin(), this->intRegs._intRegs.end());
    free_float_regs.clear();
    free_float_regs.insert(this->floatRegs._floatRegs.begin(), this->floatRegs._floatRegs.end());
}
// 统计操作数
unordered_set<string> backend::Generator::collectOperands(const ir::Function *function)
{
    unordered_set<string> operands;

    for (const auto &param : function->ParameterList)
    {
        operands.insert(param.name);
    }
    auto collectOperandsFromInstruction = [this, &operands](const ir::Operand &operand)
    {
        if (operand.type != ir::Type::null && operand.type != ir::Type::IntLiteral)
        {
            if (!is_global(operand.name))
            {
                operands.insert(operand.name);
            }
        }
    };
    for (auto it = function->InstVec.begin(); it != function->InstVec.end(); ++it)
    {
        collectOperandsFromInstruction((*it)->des);
        collectOperandsFromInstruction((*it)->op1);
        collectOperandsFromInstruction((*it)->op2);
    }
    return operands;
}
// calleeSavedReg  calleeSavedFloatReg 与  函数的stack关联起来
void backend::Generator::saveCalleeRegisters(int &currentOffset, const ir::Function *function)
{
    auto it = this->calleeSavedReg->_calleeSavedReg.begin();
    while (it != this->calleeSavedReg->_calleeSavedReg.end())
    {
        const auto &reg = *it;
        fout << "\tsw\t" << toString(reg) << "," << currentOffset << "(sp)\n";
        stackVarMap->_table["calleeSavedReg" + toString(reg)] = currentOffset;
        currentOffset += 4;
        ++it;
    }
    auto itf = this->calleeSavedFloatReg->_calleeSavedFloatReg.begin();
    while (itf != this->calleeSavedFloatReg->_calleeSavedFloatReg.end())
    {
        const auto &reg = *itf;
        fout << "\tfsw\t" << toString(reg) << "," << currentOffset << "(sp)\n";
        stackVarMap->_table["calleeSavedFloatReg" + toString(reg)] = currentOffset;
        currentOffset += 4;
        ++itf;
    }
}
// 存函数参数
void backend::Generator::saveFunctionParameters(int &currentOffset, const ir::Function *function)
{
    int intNum = 0, floatNum = 0;
    for (size_t i = 0; i < function->ParameterList.size(); ++i)
    {
        // 优先把参数放到寄存器中
        // 对于超过8个参数的情况，这些参数不会被放置在寄存器中，而是直接压入栈中。
        if (function->ParameterList[i].type == ir::Type::Int || function->ParameterList[i].type == ir::Type::IntLiteral || function->ParameterList[i].type == ir::Type::IntPtr || function->ParameterList[i].type == ir::Type::FloatPtr)
        {
            if (intNum < intPremRegs._intPremRegs.size())
            {
                fout << "\tsw\t" << toString(this->intPremRegs._intPremRegs[intNum]) << ", " << currentOffset << "(sp)\n";
            }
            else
            {
                // 超过8个的参数已经在调用者的栈中。
                // 被调用函数需要从栈中读取这些参数，并将它们存储到自己的栈帧中。
                // 先从栈取出来，再转移到另一个位置
                fout << "\tlw\tt1, " << this->stack_size + (i - 8) * 4 << "(sp)\n";
                fout << "\tsw\tt1, " << currentOffset << "(sp)\n";
            }
            intNum++;
        }
        else if (function->ParameterList[i].type == ir::Type::Float || function->ParameterList[i].type == ir::Type::FloatLiteral)
        {
            // 优先把参数放到寄存器中
            // 对于超过8个参数的情况，这些参数不会被放置在寄存器中，而是直接压入栈中。
            if (floatNum < floatPremRegs._floatPremRegs.size())
            {
                fout << "\tfsw\t" << toString(this->floatPremRegs._floatPremRegs[floatNum]) << ", " << currentOffset << "(sp)\n";
            }
            else
            {
                // 超过8个的参数已经在调用者的栈中。
                // 被调用函数需要从栈中读取这些参数，并将它们存储到自己的栈帧中。
                // 先从栈取出来，再转移到另一个位置
                fout << "\tflw\tft1, " << this->stack_size + (i - 8) * 4 << "(sp)\n";
                fout << "\tfsw\tft1, " << currentOffset << "(sp)\n";
            }
            floatNum++;
        }
        stackVarMap->_table[function->ParameterList[i].name] = currentOffset;
        currentOffset += 4;
    }
}

// 操作放到栈中
void backend::Generator::allocateStackSpaceForOperands(int &currentOffset, const ir::Function *function)
{
    auto allocateStackSpace = [this, &currentOffset](const ir::Operand &operand)
    {
        if (operand.type != ir::Type::null && operand.type != ir::Type::IntLiteral && operand.type != ir::Type::FloatLiteral)
        {
            if (!is_global(operand.name) && stackVarMap->_table.find(operand.name) == stackVarMap->_table.end())
            {
                stackVarMap->_table[operand.name] = currentOffset;
                currentOffset += 4;
            }
        }
    };

    for (const auto &instr : function->InstVec)
    {
        allocateStackSpace(instr->des);
        allocateStackSpace(instr->op1);
        allocateStackSpace(instr->op2);
    }
}
// 处理函数中的指令
void backend::Generator::processInstructions(const ir::Function &func)
{

    //   using AddFuncPtr = void (Generator::*)(const ir::Instruction&);
    // 生成指令，就是根据操作符去调用相应的指令，用map对应指令和函数指针
    unordered_map<ir::Operator, AddFuncPtr> function_map =
        {
            {ir::Operator::add, &Generator::generate_alu},
            {ir::Operator::sub, &Generator::generate_alu},
            {ir::Operator::mul, &Generator::generate_alu},
            {ir::Operator::div, &Generator::generate_alu},
            {ir::Operator::mod, &Generator::generate_alu},
            {ir::Operator::lss, &Generator::generateComparison},
            {ir::Operator::leq, &Generator::generateComparison},
            {ir::Operator::gtr, &Generator::generateComparison},
            {ir::Operator::geq, &Generator::generateComparison},
            {ir::Operator::eq, &Generator::generateComparison},
            {ir::Operator::neq, &Generator::generateComparison},
            {ir::Operator::_not, &Generator::generateComparison},
            {ir::Operator::_and, &Generator::generateComparison},
            {ir::Operator::_or, &Generator::generateComparison},
            {ir::Operator::alloc, &Generator::generate_alloc},
            {ir::Operator::load, &Generator::generate_ld_sw},
            {ir::Operator::store, &Generator::generate_ld_sw},
            {ir::Operator::getptr, &Generator::generate_getptr},
            {ir::Operator::_return, &Generator::generate__return},
            {ir::Operator::call, &Generator::generate_call},
            {ir::Operator::_goto, &Generator::generate__goto},
            {ir::Operator::def, &Generator::generate_mov},
            {ir::Operator::mov, &Generator::generate_mov},
            {ir::Operator::fdef, &Generator::generate_mov},
            // {ir::Operator::mov, &Generator::generate_mov},
            {ir::Operator::__unuse__, &Generator::generate___unuse__},
            // 浮点数
            {ir::Operator::fadd, &Generator::generate_alu},
            {ir::Operator::fsub, &Generator::generate_alu},
            {ir::Operator::fmul, &Generator::generate_alu},
            {ir::Operator::fdiv, &Generator::generate_alu},
            {ir::Operator::flss, &Generator::generateComparison},
            {ir::Operator::fleq, &Generator::generateComparison},
            {ir::Operator::fgtr, &Generator::generateComparison},
            {ir::Operator::fgeq, &Generator::generateComparison},
            {ir::Operator::feq, &Generator::generateComparison},
            {ir::Operator::fneq, &Generator::generateComparison},
            {ir::Operator::cvt_f2i, &Generator::generate___cvt_f2i},
            {ir::Operator::cvt_i2f, &Generator::generate___cvt_i2f},
        };
    for (auto it = func.InstVec.begin(); it != func.InstVec.end(); ++it)
    {
        IRPC++;
        auto jumpLabelIt = jump_label_map.find(IRPC);
        // 判断函数是否结束
        if (jumpLabelIt != jump_label_map.end())
        {
            setLabel(fout, jumpLabelIt->second);
        }
        // 没有结束，生成指令
        // 通过*it获取指向Instruction的指针，然后通过**it来解引用获取Instruction的对象
        const ir::Instruction &instr = **it;
        ir::Operator instr_op = instr.op;
        auto it_func = function_map.find(instr_op);

        if (toString(instr_op) != "cvt_f2i" && previousInstrIsNeq == true)
        {
            previousInstrIsNeq = false;
        }
        if (toString(instr_op) == "neq")
        {
            previousInstrIsNeq = true;
            // fout<<"aaaaaaa";
        }
        // fout<<toString(instr_op)<<"    ";
        if (it_func != function_map.end())
        {
            AddFuncPtr funcPtr = it_func->second;
            (this->*funcPtr)(instr);
        }
        else
        {
            cout << "Not Implemented: " << ir::toString(instr_op) << endl;
        }
    }
}
// 函数初始化，调用上面几个函数完成工作
void backend::Generator::initFunc(const ir::Function *function)
{
    resetState();

    unordered_set<string> operands = collectOperands(function);
    // 统计所有operands
    for (const auto &param : function->ParameterList)
    {
        operands.insert(param.name);
    }
    auto collectOperands = [this, &operands](const ir::Operand &operand)
    {
        if (operand.type != ir::Type::null && operand.type != ir::Type::IntLiteral)
        {
            if (!is_global(operand.name))
            {
                operands.insert(operand.name);
            }
        }
    };

    for (const auto &instr : function->InstVec)
    {
        collectOperands(instr->des);
        collectOperands(instr->op1);
        collectOperands(instr->op2);
    }
    /*
    这13个额外的空间可能用于保存一些特殊的寄存器（如 S0 到 S11 以及 RA 寄存器）和其他必要的栈帧元素。
    这里去12 float的
    */
    // this->stack_size = (operands.size() + 13 + 12) * 4;
    this->stack_size = (operands.size() + 13 + 12) * 4 < 280 ? 280 : (operands.size() + 13 + 12) * 4;
    fout << "\taddi\tsp,sp,-" << this->stack_size << "\n";

    int currentOffset = 0;
    // 依次处理saveCalleeReg、函数参数、所有操作数
    saveCalleeRegisters(currentOffset, function);
    saveFunctionParameters(currentOffset, function);
    allocateStackSpaceForOperands(currentOffset, function);
    for (size_t i = 0; i < function->InstVec.size(); ++i)
    {
        const auto &instr = function->InstVec[i];
        if (instr->op == ir::Operator::_goto)
        {
            int jumpLabelOffset = stoi(instr->des.name);
            if (!jump_label_map.count(i + jumpLabelOffset))
            {
                jump_label_map[i + jumpLabelOffset] = "_ir_goto_label" + to_string(this->label_count++);
            }
        }
    }
}

// ===================寄存器分配函数==============================
// 为操作数分配寄存器
rv::rvREG backend::Generator::allocLoadintReg(ir::Operand operand)
{
    rv::rvREG result = allocIntReg();
    auto load_from_global = [&]()
    {
        ir::Type irType = operand.type;
        string opname = operand.name;
        string strResult = rv::toString(result);
        if (irType == ir::Type::Int)
        {
            auto curReg = allocIntReg();
            string strCurReg = rv::toString(curReg);
            fout << "\tla\t" << strCurReg << ", " << opname << "\n";
            fout << "\tlw\t" << strResult << ", " << 0 << "(" << strCurReg << ")\n";
            freeIntReg(curReg);
        }
        else
        {
            fout << "\tla\t" << strResult << ", " << opname << "\n";
        }
    };
    auto load_from_stack = [&]()
    {
        string strResult = rv::toString(result);
        string opname = operand.name;
        // fout << "operand.name:" << operand.name << '\n';
        // fout << "stackVarMap->_table[operand.name]:" << stackVarMap->_table[operand.name] << '\n';
        fout << "\tlw\t" << strResult << ", " << stackVarMap->_table[opname] << "(sp)\n";
    };
    if (is_global(operand.name))
    {
        load_from_global();
    }
    else
    {
        load_from_stack();
    }
    return result;
}

// 保存操作数
void backend::Generator::saveIntOperand(ir::Operand operand, rv::rvREG int_reg)
{
    auto load_from_global = [&]()
    {
        rv::rvREG curReg = allocIntReg();
        fout << "\tla\t" << toString(curReg) << ", " << operand.name << "\n";
        fout << "\tsw\t" << toString(int_reg) << "," << 0 << "(" << toString(curReg) << ")\n";
        freeIntReg(curReg);
    };
    auto load_from_stack = [&]()
    {
        // fout << "operand.name:" << operand.name << '\n';
        // fout << "stackVarMap->_table[operand.name]:" << stackVarMap->_table[operand.name] << '\n';
        fout << "\tsw\t" << toString(int_reg) << ", " << stackVarMap->_table[operand.name] << "(sp)\n";
    };
    if (is_global(operand.name))
    {
        load_from_global();
    }
    else
    {
        load_from_stack();
    }
}
// 分配一个REG
rv::rvREG backend::Generator::allocIntReg()
{
    rv::rvREG result = *(free_int_regs.begin());
    free_int_regs.erase(result);
    return result;
}
// 释放寄存器
void backend::Generator::freeIntReg(rv::rvREG int_reg)
{
    free_int_regs.insert(int_reg);
}

// float
rv::rvFREG backend::Generator::allocLoadfloatReg(ir::Operand operand)
{
    rv::rvFREG result = allocFloatReg();
    auto load_from_global = [&]()
    {
        ir::Type irType = operand.type;
        string opname = operand.name;
        string strResult = rv::toString(result);
        if (irType == ir::Type::Float)
        {
            auto curReg = allocFloatReg();
            string strCurReg = rv::toString(curReg);
            fout << "\tla\t" << strCurReg << ", " << opname << "\n";
            fout << "\tflw\t" << strResult << ", " << 0 << "(" << strCurReg << ")\n";
            freeFloatReg(curReg);
        }
        else
        {
            fout << "\tla\t" << strResult << ", " << opname << "\n";
        }
    };
    auto load_from_stack = [&]()
    {
        string strResult = rv::toString(result);
        string opname = operand.name;

        fout << "\tflw\t" << strResult << ", " << stackVarMap->_table[opname] << "(sp)\n";
    };
    if (is_global(operand.name))
    {
        load_from_global();
    }
    else
    {
        load_from_stack();
    }
    return result;
}
// 保存操作数
void backend::Generator::saveFloatOperand(ir::Operand operand, rv::rvFREG float_reg)
{
    auto load_from_global = [&]()
    {
        rv::rvFREG curReg = allocFloatReg();
        fout << "\tla\t" << toString(curReg) << ", " << operand.name << "\n";
        fout << "\tfsw\t" << toString(float_reg) << "," << 0 << "(" << toString(curReg) << ")\n";
        freeFloatReg(curReg);
    };
    auto load_from_stack = [&]()
    {
        fout << "\tfsw\t" << toString(float_reg) << ", " << stackVarMap->_table[operand.name] << "(sp)\n";
    };
    if (is_global(operand.name))
    {
        load_from_global();
    }
    else
    {
        load_from_stack();
    }
}
// 分配一个REG
rv::rvFREG backend::Generator::allocFloatReg()
{
    rv::rvFREG result = *(free_float_regs.begin());
    free_float_regs.erase(result);
    return result;
}
// 释放寄存器
void backend::Generator::freeFloatReg(rv::rvFREG float_reg)
{
    free_float_regs.insert(float_reg);
}
// =====================生成指令  函数 ============================

void backend::Generator::generate_alu(const ir::Instruction &instr)
{
    if (instr.des.type == ir::Type::Int || instr.des.type == ir::Type::IntLiteral)
    {
        rv::rvREG op1 = allocLoadintReg(instr.op1);
        rv::rvREG op2 = allocLoadintReg(instr.op2);
        rv::rvREG des = allocIntReg();

        if (instr.op == ir::Operator::mod)
        {
            fout << "\trem\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
        }
        else
        {
            fout << "\t" << toString(instr.op) << "\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
        }
        saveIntOperand(instr.des, des);
        freeIntReg(op1);
        freeIntReg(op2);
        freeIntReg(des);
    }
    else if (instr.des.type == ir::Type::Float || instr.des.type == ir::Type::FloatLiteral)
    {
        rv::rvFREG op1 = allocLoadfloatReg(instr.op1);
        rv::rvFREG op2 = allocLoadfloatReg(instr.op2);
        rv::rvFREG des = allocFloatReg();
        fout << "\t" << toString(instr.op) << ".s" << "\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";

        saveFloatOperand(instr.des, des);
        freeFloatReg(op1);
        freeFloatReg(op2);
        freeFloatReg(des);
    }
}
void backend::Generator::generateComparison(const ir::Instruction &instr)
{
    if (instr.op1.type == ir::Type::Int || instr.op1.type == ir::Type::IntLiteral)
    {
        rv::rvREG op1, op2;
        if (instr.op1.type == ir::Type::IntLiteral)
        {
            op1 = allocIntReg();
            string a = deletefloat(instr.op1.name);
            fout << "\tli\t" << toString(op1) << "," << a << "\n";
        }
        else
        {
            op1 = allocLoadintReg(instr.op1);
        }

        if (instr.op2.type == ir::Type::IntLiteral)
        {
            op2 = allocIntReg();
            string a = deletefloat(instr.op2.name);
            fout << "\tli\t" << toString(op2) << "," << a << "\n";
        }
        else
        {
            op2 = allocLoadintReg(instr.op2);
        }
        rv::rvREG des = allocIntReg();

        string comparisonOp = toString(instr.op);
        if (comparisonOp == "lss")
        {
            fout << "\tslt\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
        }
        else if (comparisonOp == "leq")
        {
            fout << "\tslt\t" << toString(des) << "," << toString(op2) << "," << toString(op1) << "\n";
            fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
        }
        else if (comparisonOp == "gtr")
        {
            fout << "\tslt\t" << toString(des) << "," << toString(op2) << "," << toString(op1) << "\n";
        }
        else if (comparisonOp == "geq")
        {
            fout << "\tslt\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
            fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
        }
        else if (comparisonOp == "eq")
        {
            fout << "\txor\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
            fout << "\tsltiu\t" << toString(des) << "," << toString(des) << ",1\n";
        }
        else if (comparisonOp == "neq")
        {
            fout << "\txor\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
            fout << "\tsltiu\t" << toString(des) << "," << toString(des) << ",1\n";
            fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
        }
        else if (comparisonOp == "not")
        {
            fout << "\tsltiu\t" << toString(des) << "," << toString(op1) << ",1\n";
        }
        else
        {
            fout << "\tsltiu\t" << toString(des) << "," << toString(op1) << ",1\n";
            fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
            fout << "\tsltiu\t" << toString(op2) << "," << toString(op2) << ",1\n";
            fout << "\txori\t" << toString(op2) << "," << toString(op2) << ",1\n";
            fout << "\t" << toString(instr.op) << "\t" << toString(des) << "," << toString(des) << "," << toString(op2) << "\n";
        }

        saveIntOperand(instr.des, des);
        freeIntReg(op1);
        freeIntReg(op2);
        freeIntReg(des);
    }
    else if (instr.op1.type == ir::Type::Float || instr.op1.type == ir::Type::FloatLiteral)
    {
        if (toString(instr.op) == "flss" || toString(instr.op) == "fleq" || toString(instr.op) == "fgtr" ||
            toString(instr.op) == "fgeq" || toString(instr.op) == "feq" || toString(instr.op) == "fneq")
        {
            // if(toString(instr.op)=="neq"){
            //     return;
            // }
            rv::rvFREG op1, op2;
            if (instr.op1.type == ir::Type::FloatLiteral)
            {
                op1 = allocFloatReg();
                rv::rvREG tmp = allocIntReg();
                string floatLiteralValue = bit_extend(stof(instr.op1.name));
                fout << "\tli\t" << toString(tmp) << "," << floatLiteralValue << "\n";
                fout << "\tfmv.w.x\t" << toString(op1) << "," << toString(tmp) << "\n";
                freeIntReg(tmp);
            }
            else
            {
                op1 = allocLoadfloatReg(instr.op1);
            }

            if (instr.op2.type == ir::Type::FloatLiteral)
            {
                op2 = allocFloatReg();
                rv::rvREG tmp = allocIntReg();
                string floatLiteralValue = bit_extend(stof(instr.op2.name));
                fout << "\tli\t" << toString(tmp) << "," << floatLiteralValue << "\n";
                fout << "\tfmv.w.x\t" << toString(op2) << "," << toString(tmp) << "\n";
                freeIntReg(tmp);
            }
            else
            {
                op2 = allocLoadfloatReg(instr.op2);
            }
            rv::rvREG des = allocIntReg();

            string comparisonOp = toString(instr.op);
            if (comparisonOp == "flss")
            {
                fout << "\tflt.s\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
            }
            else if (comparisonOp == "fleq")
            {
                fout << "\fle.s\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
            }
            else if (comparisonOp == "fgtr")
            {
                fout << "\tflt\t" << toString(des) << "," << toString(op2) << "," << toString(op1) << "\n";
            }
            else if (comparisonOp == "fgeq")
            {
                fout << "\fle.s\t" << toString(des) << "," << toString(op2) << "," << toString(op1) << "\n";
            }
            else if (comparisonOp == "feq")
            {
                fout << "\tfeq.s\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
            }
            else if (comparisonOp == "fneq")
            {
                fout << "\tfeq.s\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
                fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
            }
            saveIntOperand(instr.des, des);
            freeFloatReg(op1);
            freeFloatReg(op2);
            freeIntReg(des);
        }
        else
        { // neq 有bug ，改成eq TODO
            rv::rvREG op1, op2;
            if (instr.op1.type == ir::Type::IntLiteral)
            {
                op1 = allocIntReg();
                string a = deletefloat(instr.op1.name);
                fout << "\tli\t" << toString(op1) << "," << a << "\n";
            }
            else
            {
                op1 = allocLoadintReg(instr.op1);
            }

            if (instr.op2.type == ir::Type::IntLiteral)
            {
                op2 = allocIntReg();
                string a = deletefloat(instr.op2.name);
                fout << "\tli\t" << toString(op2) << "," << a << "\n";
            }
            else
            {
                op2 = allocLoadintReg(instr.op2);
            }
            rv::rvREG des = allocIntReg();

            string comparisonOp = toString(instr.op);
            if (comparisonOp == "lss")
            {
                fout << "\tslt\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
            }
            else if (comparisonOp == "leq")
            {
                fout << "\tslt\t" << toString(des) << "," << toString(op2) << "," << toString(op1) << "\n";
                fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
            }
            else if (comparisonOp == "gtr")
            {
                fout << "\tslt\t" << toString(des) << "," << toString(op2) << "," << toString(op1) << "\n";
            }
            else if (comparisonOp == "geq")
            {
                fout << "\tslt\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
                fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
            }
            else if (comparisonOp == "eq")
            {
                fout << "\txor\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
                fout << "\tsltiu\t" << toString(des) << "," << toString(des) << ",1\n";
            }
            else if (comparisonOp == "neq")
            {
                // fout << "\txor\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n";
                // fout << "\tsltiu\t" << toString(des) << "," << toString(des) << ",1\n";
                // fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
                fout << "\tmv\t" << toString(des) << "," << toString(op1) << "\n";
            }
            else if (comparisonOp == "not")
            {
                fout << "\tsltiu\t" << toString(des) << "," << toString(op1) << ",1\n";
            }
            else
            {
                fout << "\tsltiu\t" << toString(des) << "," << toString(op1) << ",1\n";
                fout << "\txori\t" << toString(des) << "," << toString(des) << ",1\n";
                fout << "\tsltiu\t" << toString(op2) << "," << toString(op2) << ",1\n";
                fout << "\txori\t" << toString(op2) << "," << toString(op2) << ",1\n";
                fout << "\t" << toString(instr.op) << "\t" << toString(des) << "," << toString(des) << "," << toString(op2) << "\n";
            }

            saveIntOperand(instr.des, des);
            freeIntReg(op1);
            freeIntReg(op2);
            freeIntReg(des);
        }
    }
}
// 分配堆栈空间并更新目标寄存器
void backend::Generator::generate_alloc(const ir::Instruction &instr)
{
    // 更新堆栈指针偏移量
    stackDistrustedBytes += stoi(instr.op1.name) * 4;
    // 分配寄存器并生成指令
    auto des = allocIntReg();
    fout << "\taddi\t" << toString(des) << ", sp, " << -stackDistrustedBytes << "\n";
    // 保存结果并释放寄存器
    saveIntOperand(instr.des, des);
    freeIntReg(des);
}

// 最后生成地址计算的add指令
void backend::Generator::generate_address_calculation(const ir::Instruction &instr, rv::rvREG &op1, rv::rvREG &op2, int &arr_offset)
{
    auto loadIntLiteral = [&](const string &name)
    {
        op2 = allocIntReg();
        arr_offset = stoi(name) * 4;
        fout << "\tli\t" << toString(op2) << "," << arr_offset << "\n";
    };
    auto loadIntVar = [&]()
    {
        op2 = allocLoadintReg(instr.op2);
        fout << "\tslli\t" << toString(op2) << "," << toString(op2) << ",2\n";
    };
    // op1寄存器加载instr.op1
    op1 = allocLoadintReg(instr.op1);
    // 使用if语句判断instr.op2的类型
    if (instr.op2.type == ir::Type::IntLiteral)
        loadIntLiteral(instr.op2.name);
    else
        loadIntVar();
    fout << "\tadd\t" << toString(op1) << "," << toString(op1) << "," << toString(op2) << "\n";
}

rv::rvREG backend::Generator::generate_destination_register(const ir::Operand &operand)
{
    rv::rvREG des;
    if (operand.type == ir::Type::IntLiteral)
    {
        des = allocIntReg();
        string value = deletefloat(operand.name);
        fout << "\tli\t" << toString(des) << "," << value << "\n";
    }
    else
    {
        des = allocLoadintReg(operand);
    }
    return des;
}

void backend::Generator::generate_ld_sw(const ir::Instruction &instr)
{
    rv::rvREG op1, op2, des;
    int arr_offset = 0;
    generate_address_calculation(instr, op1, op2, arr_offset);
    if (instr.op == ir::Operator::load)
    {
        des = allocIntReg();
        fout << "\tlw\t" << toString(des) << "," << 0 << "(" + toString(op1) + ")" << "\n";
        saveIntOperand(instr.des, des);
    }
    else
    {
        des = generate_destination_register(instr.des);
        fout << "\tsw\t" << toString(des) << "," << 0 << "(" + toString(op1) + ")" << "\n";
    }
    freeIntReg(op1);
    freeIntReg(op2);
    freeIntReg(des);
}

void backend::Generator::generate_getptr(const ir::Instruction &instr)
{
    // Lambda 表达式：加载基地址
    auto loadBaseAddress = [&](rv::rvREG &op1)
    {
        if (!is_global(instr.op1.name))
        {
            op1 = allocLoadintReg(instr.op1);
        }
        else
        {
            op1 = allocIntReg();
            fout << "\tla\t" << toString(op1) << "," << instr.op1.name << "\n";
        }
    };
    // Lambda 表达式：计算偏移量
    auto calculateOffset = [&](rv::rvREG &op2, int offset)
    {
        op2 = allocIntReg();
        fout << "\tli\t" << toString(op2) << "," << offset << "\n";
    };

    rv::rvREG op1, op2, des;
    des = allocIntReg();
    // 加载基地址到 op1
    loadBaseAddress(op1);
    // 根据 op2 的类型计算偏移量
    if (instr.op2.type == ir::Type::IntLiteral)
    {
        int arr_offset = stoi(instr.op2.name) * 4;
        calculateOffset(op2, arr_offset);
    }
    else
    {
        op2 = allocLoadintReg(instr.op2);
        fout << "\tslli\t" << toString(op2) << "," << toString(op2) << ",2\n";
    }
    // 将基地址和偏移量相加，结果存储在 des 中
    fout << "\tadd\t" << toString(des) << "," << toString(op1) << "," << toString(op2) << "\n"; // 保存结果到目标操作数
    saveIntOperand(instr.des, des);
    // 释放使用的寄存器
    freeIntReg(op1);
    freeIntReg(op2);
    freeIntReg(des);
}

void backend::Generator::generate__return(const ir::Instruction &instr)
{
    // 根据函数返回值类型，将结果保存在a0或fa0中
    if (instr.op1.type == ir::Type::IntLiteral)
    {
        string intLiteralValue = deletefloat(instr.op1.name);
        fout << "\tli\ta0," << intLiteralValue << "\n";
    }
    else if (instr.op1.type == ir::Type::Int)
    {
        rv::rvREG tempRegister = allocLoadintReg(instr.op1);
        fout << "\tmv\ta0," << toString(tempRegister) << "\n";
        freeIntReg(tempRegister);
    }
    else if (instr.op1.type == ir::Type::FloatLiteral)
    {
        string floatLiteralValue = instr.op1.name;
        fout << "\tfli\tfa0," << floatLiteralValue << "\n";
    }
    else if (instr.op1.type == ir::Type::Float)
    {
        rv::rvFREG tempFloatRegister = allocLoadfloatReg(instr.op1);
        fout << "\tfmv.s\tfa0," << toString(tempFloatRegister) << "\n";
        freeFloatReg(tempFloatRegister);
    }
    // 恢复所有被调用者保存的寄存器
    for (auto it = this->calleeSavedReg->_calleeSavedReg.begin(); it != this->calleeSavedReg->_calleeSavedReg.end(); ++it)
    {
        string savedRegister = "calleeSavedReg" + toString(*it);
        fout << "\tlw\t" << toString(*it) << "," << stackVarMap->_table[savedRegister] << "(sp)\n";
    }
    for (auto it = this->calleeSavedFloatReg->_calleeSavedFloatReg.begin(); it != this->calleeSavedFloatReg->_calleeSavedFloatReg.end(); ++it)
    {
        string savedRegister = "calleeSavedFloatReg" + toString(*it);
        fout << "\tflw\t" << toString(*it) << "," << stackVarMap->_table[savedRegister] << "(sp)\n";
    }
    fout << "\taddi\tsp,sp," << this->stack_size << "\n";
    fout << "\tret\n";
}

void backend::Generator::storeOperand(const ir::Operand &op, int index)
{
    if (op.type == ir::Type::IntLiteral)
    {
        string intLiteralValue = deletefloat(op.name);
        fout << "\tli\t" << toString(this->intPremRegs._intPremRegs[index]) << "," << intLiteralValue << "\n";
    }
    else if (op.type == ir::Type::Int || op.type == ir::Type::IntPtr || op.type == ir::Type::FloatPtr)
    {
        rv::rvREG reg = allocLoadintReg(op);
        fout << "\tmv\t" << toString(this->intPremRegs._intPremRegs[index]) << "," << toString(reg) << "\n";
        freeIntReg(reg);
    }
    else if (op.type == ir::Type::FloatLiteral)
    {
        rv::rvREG tmp = allocIntReg();
        string floatLiteralValue = bit_extend(stof(op.name));
        fout << "\tli\t" << toString(tmp) << "," << floatLiteralValue << "\n";
        fout << "\tfmv.w.x\t" << toString(this->floatPremRegs._floatPremRegs[index]) << "," << toString(tmp) << "\n";
    }
    else if (op.type == ir::Type::Float)
    {
        rv::rvFREG reg = allocLoadfloatReg(op);
        fout << "\tfmv.s\t" << toString(this->floatPremRegs._floatPremRegs[index]) << "," << toString(reg) << "\n";
        freeFloatReg(reg);
    }
}
void backend::Generator::storeOperandOnStack(const ir::Operand &op, int index)
{
    switch (op.type)
    {
    case ir::Type::IntLiteral:
    {
        string intLiteralValue = deletefloat(op.name);
        rv::rvREG allocReg1 = allocIntReg();
        rv::rvREG allocReg2 = allocIntReg();
        fout << "\tli\t" << toString(allocReg1) << "," << intLiteralValue << "\n";
        fout << "\taddi\t" << toString(allocReg2) << "," << toString(rv::rvREG::SP) << "," << -(stackDistrustedBytes) << "\n";
        fout << "\tsw\t" << toString(allocReg1) << "," << (index - 8) * 4 << "(" << toString(allocReg2) << ")\n";
        freeIntReg(allocReg1);
        freeIntReg(allocReg2);
        break;
    }
    case ir::Type::Int:
    {
        rv::rvREG allocReg1 = allocLoadintReg(op);
        rv::rvREG allocReg2 = allocIntReg();
        fout << "\taddi\t" << toString(allocReg2) << "," << toString(rv::rvREG::SP) << "," << -(stackDistrustedBytes) << "\n";
        fout << "\tsw\t" << toString(allocReg1) << "," << (index - 8) * 4 << "(" << toString(allocReg2) << ")\n";
        freeIntReg(allocReg1);
        freeIntReg(allocReg2);
        break;
    }
    case ir::Type::FloatLiteral:
    {
        string floatLiteralValue = op.name;
        rv::rvFREG allocReg1 = allocFloatReg();
        rv::rvREG allocReg2 = allocIntReg();
        fout << "\tli.s\t" << toString(allocReg1) << "," << floatLiteralValue << "\n"; // Load float literal
        fout << "\taddi\t" << toString(allocReg2) << "," << toString(rv::rvREG::SP) << "," << -(stackDistrustedBytes) << "\n";
        fout << "\tswc1\t" << toString(allocReg1) << "," << (index - 8) * 4 << "(" << toString(allocReg2) << ")\n"; // Store float to memory
        freeFloatReg(allocReg1);
        freeIntReg(allocReg2);
        break;
    }
    case ir::Type::Float:
    {
        rv::rvFREG allocReg1 = allocLoadfloatReg(op); // Assuming allocLoadFloatReg to load float variables
        rv::rvREG allocReg2 = allocIntReg();
        fout << "\taddi\t" << toString(allocReg2) << "," << toString(rv::rvREG::SP) << "," << -(stackDistrustedBytes) << "\n";
        fout << "\tswc1\t" << toString(allocReg1) << "," << (index - 8) * 4 << "(" << toString(allocReg2) << ")\n"; // Store float to memory
        freeFloatReg(allocReg1);
        freeIntReg(allocReg2);
        break;
    }
    default:
        // Handle any other types if necessary
        break;
    }
}

void backend::Generator::generate_call(const ir::Instruction &instr)
{
    // op1.name为函数名，des为函数返回值
    const auto *instr_ptr = &instr;
    auto callinst_ptr = dynamic_cast<const ir::CallInst *>(instr_ptr);
    if (instr.op1.name == "main" || instr.op1.name == "global")
    {
        return;
    }
    int extraStackSpace = max(0, static_cast<int>(callinst_ptr->argumentList.size()) - 8) * 4;
    stackDistrustedBytes += extraStackSpace;
    // 首先将所有参数存入对应的寄存器
    for (size_t i = 0; i < callinst_ptr->argumentList.size(); i++)
    {
        if (i <= 7)
        {
            storeOperand(callinst_ptr->argumentList[i], i);
        }
        else
        {
            storeOperandOnStack(callinst_ptr->argumentList[i], i);
        }
    }
    // Set the stack pointer to reserve space for function call parameters
    fout << "\taddi\t" << toString(rv::rvREG::SP) << "," << toString(rv::rvREG::SP) << "," << -(stackDistrustedBytes) << "\n";

    // Call the function specified by callinst_ptr
    fout << "\tcall\t" << callinst_ptr->op1.name << "\n";

    // Restore the stack pointer to its original position after the function call
    fout << "\taddi\t" << toString(rv::rvREG::SP) << "," << toString(rv::rvREG::SP) << "," << stackDistrustedBytes << "\n";

    // If there is a destination operand in the instruction, save the result into register A0
    if (instr.des.type == ir::Type::Int || instr.des.type == ir::Type::IntPtr)
        saveIntOperand(instr.des, rv::rvREG::A0);
    else if (instr.des.type != ir::Type::null)
    {
        saveFloatOperand(instr.des, rv::rvFREG::FA0);
    }
}

void backend::Generator::generate__goto(const ir::Instruction &instr)
{
    int jumpOffset = stoi(instr.des.name);
    auto handleConditionalJump = [this, &instr, &jumpOffset]()
    {
        rv::rvREG reg;
        if (instr.op1.type == ir::Type::Int)
        {
            reg = allocLoadintReg(instr.op1);
        }
        else if (instr.op1.type == ir::Type::IntLiteral)
        {
            reg = allocIntReg();
            string intLiteralValue = deletefloat(instr.op1.name);
            fout << "\tli\t" << toString(reg) << "," << intLiteralValue << "\n";
        }
        string a = jump_label_map[IRPC + jumpOffset];
        fout << "\tbnez\t" << toString(reg) << "," << a << "\n";
        freeIntReg(reg);
    };
    auto handleUnconditionalJump = [this, &jumpOffset]()
    {
        fout << "\tj\t" << jump_label_map[IRPC + jumpOffset] << "\n";
    };

    if (instr.op1.type == ir::Type::IntLiteral || instr.op1.type == ir::Type::Int)
        handleConditionalJump();
    else
        handleUnconditionalJump();
}

void backend::Generator::generate___unuse__(const ir::Instruction &instr)
{
    fout << "\tnop\n";
}

void backend::Generator::generate_mov(const ir::Instruction &instr)
{
    ir::Type irtype = instr.op1.type;
    if (irtype == ir::Type::IntLiteral)
    {
        rv::rvREG des = allocIntReg();

        string intLiteralValue = deletefloat(instr.op1.name);
        fout << "\tli\t" << toString(des) << "," << intLiteralValue << "\n";
        saveIntOperand(instr.des, des);
        freeIntReg(des);
    }
    else if (irtype == ir::Type::Int)
    {
        rv::rvREG des = allocIntReg();
        rv::rvREG op1 = allocLoadintReg(instr.op1);
        fout << "\tmv\t" << toString(des) << "," << toString(op1) << "\n";
        freeIntReg(op1);
        saveIntOperand(instr.des, des);
        freeIntReg(des);
    }
    else if (irtype == ir::Type::FloatLiteral)
    {
        rv::rvREG des = allocIntReg();
        string floatLiteralValue = instr.op1.name;
        string a = bit_extend(stof(floatLiteralValue));
        rv::rvFREG floatReg = allocFloatReg();
        fout << "\tli\t" << toString(des) << "," << a << "\n";
        fout << "\tfmv.w.x\t" << toString(floatReg) << "," << toString(des) << "\n";
        saveFloatOperand(instr.des, floatReg);
        freeFloatReg(floatReg);
        freeIntReg(des);
    }
    else if (irtype == ir::Type::Float)
    {
        rv::rvFREG des = allocFloatReg();
        rv::rvFREG floatReg = allocLoadfloatReg(instr.op1);
        fout << "\tfmv.s\t" << toString(des) << "," << toString(floatReg) << "\n";
        freeFloatReg(floatReg);
        saveFloatOperand(instr.des, des);
        freeFloatReg(des);
    }
}

void backend::Generator::generate___cvt_f2i(const ir::Instruction &instr)
{
    // fout << "cvt_f2i  " << previousInstrIsNeq;
    // fout << toString(instr.op1.type);
    // fout << toString(instr.des.type);
    if (previousInstrIsNeq)
    {
        rv::rvREG des = allocIntReg();
        rv::rvREG floatReg = allocLoadintReg(instr.op1);
        fout << "\tmv\t" << toString(des) << "," << toString(floatReg) << "\n";
        freeIntReg(floatReg);
        saveIntOperand(instr.des, des);
        freeIntReg(des);
    }
    else
    {
        rv::rvREG des = allocIntReg();
        rv::rvFREG floatReg = allocLoadfloatReg(instr.op1);  
        fout << "\tfcvt.w.s\t" << toString(des) << "," << toString(floatReg) <<",rtz" << "\n";
        freeFloatReg(floatReg);
        saveIntOperand(instr.des, des);
        freeIntReg(des);
    }
}

void backend::Generator::generate___cvt_i2f(const ir::Instruction &instr)
{
    rv::rvFREG des = allocFloatReg();
    rv::rvREG intReg = allocLoadintReg(instr.op1);
    fout << "\tfcvt.s.w\t" << toString(des) << "," << toString(intReg) << "\n";
    freeIntReg(intReg);
    saveFloatOperand(instr.des, des);
    freeFloatReg(des);
}
