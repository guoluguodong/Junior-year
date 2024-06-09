#include "front/semantic.h"

#include <cassert>
#include <iostream>
using ir::Function;
using ir::Instruction;
using ir::Operand;
using ir::Operator;
using namespace std;
#define TODO assert(0 && "TODO");
#define DEBUG_PARSER 1
#define ANALYSIS_NODE(child, node, type, index)              \
    if (child = dynamic_cast<type *>(node->children[index])) \
        analyze##type(child);

#define ANALYSIS_NODE_WITH_RETURN(node, type, index)              \
    if (auto child = dynamic_cast<type *>(node->children[index])) \
        return analyze##type(child);

#define ANALYSIS_NODE_WITH_LeftVal(child, node, type, index, res) \
    if (node->children.size() > index)                            \
        if (child = dynamic_cast<type *>(node->children[index]))  \
            res = analyze##type(child);

#define COPY_EXP_NODE(from, to) \
    to->v = from->v;            \
    to->t = from->t;
#define CHANGE_NODE(root, new_v, new_t) \
    root->v = new_v;                    \
    root->t = new_t;
map<string, ir::Function *> *frontend::get_lib_funcs()
{
    static map<string, ir::Function *> lib_funcs = {
        {"getint", new Function("getint", Type::Int)},
        {"getch", new Function("getch", Type::Int)},
        {"getfloat", new Function("getfloat", Type::Float)},
        {"getarray", new Function("getarray", {Operand("arr", Type::IntPtr)}, Type::Int)},
        {"getfarray", new Function("getfarray", {Operand("arr", Type::FloatPtr)}, Type::Int)},
        {"putint", new Function("putint", {Operand("i", Type::Int)}, Type::null)},
        {"putch", new Function("putch", {Operand("i", Type::Int)}, Type::null)},
        {"putfloat", new Function("putfloat", {Operand("f", Type::Float)}, Type::null)},
        {"putarray", new Function("putarray", {Operand("n", Type::Int), Operand("arr", Type::IntPtr)}, Type::null)},
        {"putfarray", new Function("putfarray", {Operand("n", Type::Int), Operand("arr", Type::FloatPtr)}, Type::null)},
    };
    return &lib_funcs;
}
/*
符号表用来存储程序中相关变量信息，包括类型，作用域，访问控制信息
     * @brief enter a new scope, record the infomation in scope stacks
     * 输入一个新作用域，将信息记录在scope_stack中
     * @param node: a Block node, entering a new Block means a new name scope
     * 输入新的Block表示新的名称范围
     * struct Block: AstNode{
        Block(AstNode* p = nullptr);
        };
        Block -> '{' { BlockItem } '}'
        代码块项
        表示代码块中的一项的语法树节点
    存到    vector<ScopeInfo> scope_stack;
    // 作用域，作用范围
struct ScopeInfo {
    int cnt;    // 编号,需要计数
    string name;// 作用域的名称
    Block* block;// 作用域入口根节点,block为一个作用域整体
    map_str_ste table;// table将作用域映射到符号表入口
};

using map_str_ste = map<string, STE>;
// definition of symbol table entry符号表入口
struct STE {
    struct Operand {
    string name;变量名
    Type type;变量类型};
    ir::Operand operand;
    vector<int> dimension;//数组操作数
};
*/
void frontend::SymbolTable::add_scope()
{
    // TODO;
    ScopeInfo scopeInfo;
    scopeInfo.cnt = blockId;                       // 当前作用域编号
    scopeInfo.name = "Block" + to_string(blockId); // 作用域编号
    blockId++;
    scope_stack.push_back(scopeInfo);
}

/**
 * @brief exit a scope, pop out infomations
 * 退出作用域，弹出信息
 */
void frontend::SymbolTable::exit_scope()
{
    scope_stack.pop_back();
}
/**
 * @brief Get the scoped name, to deal the same name in different scopes, we change origin id to a new one with scope infomation,
 * for example, we have these code:
 * "
 * int a;
 * {
 *      int a; ....
 * }
 * "
 * in this case, we have two variable both name 'a', after change they will be 'a' and 'a_block'
 * 获取作用域名称，为了在不同的作用域中处理相同的名称，
 * 我们将origin id更改为具有作用域信息的新id，
 * 在这种情况下，我们有两个变量，名称都为“a”，更改后它们将是“a”和“a_block”
 * @param id: origin id
 * @return string: new name with scope infomations
 */
string frontend::SymbolTable::get_scoped_name(string id) const
{
    for (int i = scope_stack.size() - 1; i >= 0; --i)
    {
        // scope_stack[i].table :map<string,STE>
        if (scope_stack[i].table.find(id) != scope_stack[i].table.end())
            return id + "_" + scope_stack[i].name; // id+作用域名称
    }
}
/**
 * @brief get the right operand with the input name：
 * 使用输入名称获取正确的operand
 * @param id identifier name
 * @return Operand
 */
Operand frontend::SymbolTable::get_operand(string id) const
{
    for (int i = scope_stack.size() - 1; i >= 0; --i)
    {
        if (scope_stack[i].table.find(id) != scope_stack[i].table.end())
            // scope_stack[i].table :map<string,STE>
            // ->second:STE
            return scope_stack[i].table.find(id)->second.operand;
    }
}
/**
 * @brief get the right ste with the input name：
 * 使用输入的名称获得正确的ste
 * @param id identifier name
 * @return STE
 */
frontend::STE frontend::SymbolTable::get_ste(string id) const
{
    // TODO;
    for (int i = scope_stack.size() - 1; i >= 0; --i)
    {
        if (scope_stack[i].table.find(id) != scope_stack[i].table.end())
            // scope_stack[i].table :map<string,STE>
            // ->second:STE
            return scope_stack[i].table.find(id)->second;
    }
}
void merge(vector<ir::Instruction *> &instrVec, vector<ir::Instruction *> &instructions)
{
    instrVec.insert(instrVec.end(), instructions.begin(), instructions.end());
}

/*实验核心工作，在本次实验中，你们需要实现 Analyzer 类，
CompUnit -> (Decl | FuncDef) [CompUnit]
完成 ir::Program get_ir_program(CompUnit*);接口，
该接口接受一个源程序语法树的根节点 Comp*，对其进行分析，
返回分析结果 ir::Program
*/
/*
struct Analyzer {
    int tmp_cnt;
    vector<ir::Instruction*> g_init_inst; // 全局变量初始化指令队列
    SymbolTable symbol_table;           // 全局符号表
    ir::Program irProgram;                 // 程序
    ir::Function* curFunctionPtr = nullptr;  // 当前函数指针
    }
*/
frontend::Analyzer::Analyzer() : tmp_cnt(0), symbol_table()
{
    // TODO;
    // 构造函数，不用动
}
/*
struct SymbolTable{
    vector<ScopeInfo> scope_stack;
    map<string,ir::Function*> functions;
    int blockId=0;
*/
ir::Program frontend::Analyzer::get_ir_program(CompUnit *root)
{
    // TODO;
    // 符号表添加全局作用域
    symbol_table.add_scope();
    ir::Function *globalFunction = new ir::Function("global", ir::Type::null);
    /*
    struct Function {
    string name;//函数块名称，可以直接将源程序中函数名作为name
    ir::Type returnType;//函数返回类型
    Function();
    Function(const string&, const ir::Type&);
};
    */
    symbol_table.functions["global"] = globalFunction;
    // frontend::get_lib_funcs() IO库函数名称到对应库函数的映射,装载库函数
    map<string, ir::Function *> libFunctions = *get_lib_funcs();
    for (auto iterator = libFunctions.begin(); iterator != libFunctions.end(); ++iterator)
        symbol_table.functions[iterator->first] = iterator->second;
    // DFS遍历AST,从compunit开始
    analyzeCompUnit(root);
    // 构造irProgram
    /*
        struct Program {
           vector<Function> functions;
           vector<GlobalVal> globalVal;
           Program();
           void addFunction(const Function& proc);
           string draw();
       };
    */
    // 修改全局函数的return
    ir::Instruction *globalreturn = new ir::Instruction(ir::Operand(), ir::Operand(), ir::Operand(), ir::Operator::_return);
    globalFunction->addInst(globalreturn);
    irProgram.addFunction(*globalFunction);
    // scope_stack[0].table: 全局域的map<string, STE>
    for (auto it = symbol_table.scope_stack[0].table.begin(); it != symbol_table.scope_stack[0].table.end(); it++)
    {
        /*
            struct GlobalVal
            {
                ir::Operand val;
                int maxlen = 0;     //为数组长度设计
                GlobalVal(ir::Operand va);
                GlobalVal(ir::Operand va, int len);
            };
        */
        // int,float,immediate
        if (it->second.dimension.size() == 0)
            irProgram.globalVal.push_back({{symbol_table.get_scoped_name(it->second.operand.name), it->second.operand.type}});
        // array
        else
        {
            int maxlen = 1;
            // 将多维数组展平成1维，计算长度
            for (unsigned int i = 0; i < it->second.dimension.size(); i++)
                maxlen = maxlen * it->second.dimension[i];
            // ir::GlobalVal::GlobalVal(ir::Operand va, int len)
            irProgram.globalVal.push_back({{symbol_table.get_scoped_name(it->second.operand.name), it->second.operand.type}, maxlen});
        }
    }
    return irProgram;
}

/*
              分析表达式的结构
*/
// CompUnit -> (Decl | FuncDef) [CompUnit]
void frontend::Analyzer::analyzeCompUnit(CompUnit *root)
{
    if (root->children.size() == 0)
    {
        return;
    }
    // 分析 Decl ,还要声明全局变量
    if (Decl *decl = dynamic_cast<Decl *>(root->children[0]))
    {
        vector<ir::Instruction *> decl_insts = analyzeDecl(decl);
        for (auto inst : decl_insts)
            symbol_table.functions["global"]->addInst(inst);
    }
    // 分析 FuncDef
    FuncDef *child;
    ANALYSIS_NODE(child, root, FuncDef, 0);
    // 分析可选部分
    if (root->children.size() > 1)
    {
        CompUnit *child;
        ANALYSIS_NODE(child, root, CompUnit, 1);
    }
}

// Decl -> ConstDecl | VarDecl
vector<ir::Instruction *> frontend::Analyzer::analyzeDecl(Decl *root)
{
    assert(root->children.size() != 0);
    // 分析 ConstDecl
    ANALYSIS_NODE_WITH_RETURN(root, ConstDecl, 0);
    // 分析 VarDecl
    ANALYSIS_NODE_WITH_RETURN(root, VarDecl, 0);
}

// ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'
vector<ir::Instruction *> frontend::Analyzer::analyzeConstDecl(ConstDecl *root)
{
    vector<ir::Instruction *> instrVec;
    // BType
    ir::Type res;
    BType *child;
    ANALYSIS_NODE_WITH_LeftVal(child, root, BType, 1, res);
    ir::Type type = res;
    for (int i = 2; i < root->children.size(); i += 2)
    {
        // ConstDef,循环重复
        ConstDef *constdef = dynamic_cast<ConstDef *>(root->children[i]);
        vector<Instruction *> instructions = analyzeConstDef(constdef, type);
        merge(instrVec, instructions);
    }
    return instrVec;
}

// ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
vector<ir::Instruction *> frontend::Analyzer::analyzeConstDef(ConstDef *root, ir::Type type)
{
    vector<ir::Instruction *> results;
    /*
    "name" : "ConstDef",
    "subtree" : [
        {
            "name" : "Terminal",
            "type" : "IDENFR",
            "value" : "Cy92k8jOyGwpymrp_aZ
        },
        {
            "name" : "Terminal",
            "type" : "ASSIGN",
            "value" : "="
        },....
    */
    // 获取变量名
    string variableName = dynamic_cast<Term *>(root->children[0])->token.value;
    // 最简单的情形 a = 2, root.children.size==3
    if (root->children.size() == 3)
    {
        // 符号表入口,构建ste变量
        STE variableSTE;
        if (type == ir::Type::Float)
            variableSTE.operand = ir::Operand(variableName, ir::Type::FloatLiteral);
        else if (type == ir::Type::Int)
            variableSTE.operand = ir::Operand(variableName, ir::Type::IntLiteral);
        // ste存到符号表
        symbol_table.scope_stack.back().table[variableName] = variableSTE;

        // ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
        // 接着处理 ConstInitVal
        ConstInitVal *constinit = dynamic_cast<ConstInitVal *>(root->children[2]);
        // 在analyzeConstExp后，ConstExp *constexp 中以存入表达式的计算结果，值也已经计算出来，constexp.v
        // int a=5+6 分析时，直接计算5+6，汇编 def a 11 即可，不写计算过程的汇编
        ConstExp *constexp;
        ANALYSIS_NODE(constexp, constinit, ConstExp, 0);
        symbol_table.scope_stack.back().table[variableName].literalVal = constexp->v;

        // 存入符号表之后，还要编写汇编指令
        if (type == ir::Type::Int)
        {
            // constexp.v 是 string
            int value;
            if (constexp->t == ir::Type::FloatLiteral)
                value = std::stof(constexp->v);
            else if (constexp->t == ir::Type::IntLiteral)
                value = std::stoi(constexp->v);
            ir::Instruction *defInstruction = new ir::Instruction(ir::Operand(to_string(value), ir::Type::IntLiteral), ir::Operand(),
                                                                  ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::Int), ir::Operator::def);
            results.push_back(defInstruction);
        }
        else if (type == ir::Type::Float)
        {
            float value = std::stof(constexp->v);
            ir::Instruction *fdefInstruction = new ir::Instruction(ir::Operand(to_string(value), ir::Type::FloatLiteral), ir::Operand(),
                                                                   ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::Float), ir::Operator::fdef);
            results.push_back(fdefInstruction);
        }
    }
    // n维数组 Ident { '[' ConstExp ']' } '=' ConstInitVal Ident '[' ConstExp ']' '=' ConstInitVal
    else
    {
        int arraySize = 1;
        STE arrSTE;
        for (unsigned int i = 0; i < root->children.size(); i++)
        {
            if (Term *term = dynamic_cast<Term *>(root->children[i]))
                if (term->token.type == TokenType::ASSIGN)
                    break;
            ConstExp *constexp = nullptr;
            ANALYSIS_NODE(constexp, root, ConstExp, i);
            if (constexp != nullptr)
            {
                arrSTE.dimension.push_back(std::stoi(constexp->v));
                arraySize *= std::stoi(constexp->v);
            }
        }
        /*
        ir::Operand operand;
        vector<int> dimension;//数组操作数
        */
        ir::Type arrayType = type;
        // ir::Operand
        if (arrayType == ir::Type::Int)
            arrayType = ir::Type::IntPtr;
        else if (arrayType == ir::Type::Float)
            arrayType = ir::Type::FloatPtr;
        arrSTE.operand = ir::Operand(variableName, arrayType);
        // 加入ste
        symbol_table.scope_stack.back().table[variableName] = arrSTE;
        // 生成汇编指令
        if (symbol_table.scope_stack.size() > 1)
        {
            ir::Instruction *allocInst = new ir::Instruction(ir::Operand(to_string(arraySize), ir::Type::IntLiteral), ir::Operand(),
                                                             ir::Operand(symbol_table.get_scoped_name(variableName), arrayType), ir::Operator::alloc);
            results.push_back(allocInst);
        }
        ConstInitVal *constinit = dynamic_cast<ConstInitVal *>(root->children.back());
        vector<Instruction *> instructions = analyzeConstInitVal(constinit, arrayType, arraySize, variableName);
        merge(results, instructions);
    }
    return results;
}
// ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
/*
ConstInitVal
    Terminal '{'
    ConstInitVal
        ConstExp
    Terminal ','
    ConstInitVal
        ConstExp
    Terminal ','
    ...
    Terminal '}'
*/
vector<ir::Instruction *> frontend::Analyzer::analyzeConstInitVal(ConstInitVal *root, ir::Type arrayType, int arraySize, string variableName)
{
    vector<ir::Instruction *> instrVec;
    // 如果数组没有初始值 如 int a[5];不会调用analyzeConstInitVal，不用初始化

    // 测试用例中数组均初始化了
    // 单独的ConstExp已经处理过
    // 只处理'{' [ ConstInitVal { ',' ConstInitVal } ] '}'
    for (unsigned int i = 1, cnt = 0; i < root->children.size() - 1; i += 2, cnt += 1)
    {
        ConstInitVal *Subconstinit = dynamic_cast<ConstInitVal *>(root->children[i]);
        ConstExp *constexp;
        ANALYSIS_NODE(constexp, Subconstinit, ConstExp, 0);
        ir::Instruction *storeInstruction;
        if (arrayType == ir::Type::IntPtr)
        {
            int value;
            if (constexp->t == ir::Type::Float)
                value = std::stof(constexp->v);
            else
                value = std::stoi(constexp->v);
            // arr[2] = 3; => store 3, arr, 2
            storeInstruction = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::IntPtr),
                                                   ir::Operand(to_string(cnt), ir::Type::IntLiteral),
                                                   ir::Operand(to_string(value), ir::Type::IntLiteral), ir::Operator::store);
        }
        else if (arrayType == ir::Type::FloatPtr)
        {
            float value = std::stof(constexp->v);
            // arr[2] = 3; => store 3, arr, 2
            storeInstruction = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::FloatPtr),
                                                   ir::Operand(to_string(cnt), ir::Type::IntLiteral),
                                                   ir::Operand(to_string(value), ir::Type::FloatLiteral), ir::Operator::store);
        }
        instrVec.push_back(storeInstruction);
    }
    return instrVec;
}

// VarDecl -> BType VarDef { ',' VarDef } ';'
vector<ir::Instruction *> frontend::Analyzer::analyzeVarDecl(VarDecl *root)
{
    vector<ir::Instruction *> instrVec;
    // BType
    auto btype = dynamic_cast<BType *>(root->children[0]);
    ir::Type type = analyzeBType(btype);
    for (int i = 1; i < root->children.size(); i += 2)
    {
        VarDef *vardef = dynamic_cast<VarDef *>(root->children[i]);
        vector<Instruction *> instructions = analyzeVarDef(vardef, type);
        merge(instrVec, instructions);
    }
    return instrVec;
}

// VarDef -> Ident { '[' ConstExp ']' } [ '=' InitVal ]
// 类似于constdef ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
vector<ir::Instruction *> frontend::Analyzer::analyzeVarDef(VarDef *root, ir::Type type)
{
    vector<ir::Instruction *> instrVec;
    string variableName = dynamic_cast<Term *>(root->children[0])->token.value;
    // 如果是 a 或者 a = 2
    if (root->children.size() == 1 || root->children.size() == 3)
    {
        // 记录符号表条目
        STE variableSTE;
        variableSTE.operand = ir::Operand(variableName, type);
        symbol_table.scope_stack.back().table[variableName] = variableSTE;

        string val;
        ir::Type irType = ir::Type::Int;
        // 判断是否初始化,检查尾部是不是 InitVal
        if (InitVal *initVal = dynamic_cast<InitVal *>(root->children.back()))
        {
            // 计算表达式值
            Exp *exp;
            vector<Instruction *> instructions;
            ANALYSIS_NODE_WITH_LeftVal(exp, initVal, Exp, 0, instructions);
            merge(instrVec, instructions);
            // 变量声明时，变量类型由声明决定，与表达式的结果无关

            if (type == ir::Type::Int)
            {
                // int val;
                if (exp->t == ir::Type::Int || exp->t == ir::Type::IntLiteral)
                {
                    val = exp->v;
                    irType = exp->t;
                }
                else if (exp->t == ir::Type::Float)
                {
                    /* cvt_f2i
                    浮点型变量转为整型变量，第一个操作数为待转换变量，结果为类型转换后变量，第二个操作数不使用。示例如下：
                    float a = 2;int b = a; => cvt_f2i b, a
                    */
                    string tmpIntcvtName = "a" + to_string(tmp_cnt++);
                    ir::Instruction *cvtInst = new ir::Instruction(ir::Operand(exp->v, ir::Type::Float), ir::Operand(),
                                                                   ir::Operand(tmpIntcvtName, ir::Type::Int), ir::Operator::cvt_f2i);
                    instrVec.push_back(cvtInst);
                    val = tmpIntcvtName;
                    irType = ir::Type::Int;
                }
                else if (exp->t == ir::Type::FloatLiteral)
                {
                    int value = std::stof(exp->v);
                    val = to_string(value);
                    irType = ir::Type::IntLiteral;
                }
                ir::Instruction *defInstruction = new ir::Instruction(ir::Operand(val, irType), ir::Operand(),
                                                                      ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::Int), ir::Operator::def);
                instrVec.push_back(defInstruction);
            }
            else if (type == ir::Type::Float)
            {
                if (exp->t == ir::Type::Float || exp->t == ir::Type::FloatLiteral)
                {
                    val = exp->v;
                    irType = exp->t;
                }
                else if (exp->t == ir::Type::Int)
                {
                    string tmpFloatcvtName = "a" + to_string(tmp_cnt++);
                    ir::Instruction *cvtInst = new ir::Instruction(ir::Operand(exp->v, ir::Type::Int), ir::Operand(),
                                                                   ir::Operand(tmpFloatcvtName, ir::Type::Float), ir::Operator::cvt_i2f);
                    instrVec.push_back(cvtInst);
                    val = tmpFloatcvtName;
                    irType = ir::Type::Float;
                }
                else if (exp->t == ir::Type::IntLiteral)
                {
                    float value = std::stof(exp->v);
                    val = to_string(value);
                    irType = ir::Type::FloatLiteral;
                }
                ir::Instruction *defInstruction = new ir::Instruction(ir::Operand(val, irType), ir::Operand(),
                                                                      ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::Float), ir::Operator::fdef);
                instrVec.push_back(defInstruction);
            }
        }
        // 没有初始化值，初始化为0
        else
        {
            if (type == ir::Type::Int)
            {
                val = "0";
                irType = ir::Type::IntLiteral;
                ir::Instruction *defInstruction = new ir::Instruction(ir::Operand(val, irType), ir::Operand(),
                                                                      ir::Operand(symbol_table.get_scoped_name(variableName), type), ir::Operator::def);
                instrVec.push_back(defInstruction);
            }
            else if (type == ir::Type::Float)
            {
                val = "0.0";
                irType = ir::Type::FloatLiteral;
                ir::Instruction *defInstruction = new ir::Instruction(ir::Operand(val, irType), ir::Operand(),
                                                                      ir::Operand(symbol_table.get_scoped_name(variableName), type), ir::Operator::fdef);
                instrVec.push_back(defInstruction);
            }
        }
    }
    // n维数组 Ident '[' ConstExp ']' 或 Ident '[' ConstExp ']' '=' InitVal
    else
    {
        int arraySize = 1;
        STE arrSTE;
        for (unsigned int i = 0; i < root->children.size(); i++)
        {
            if (Term *term = dynamic_cast<Term *>(root->children[i]))
                if (term->token.type == TokenType::ASSIGN)
                    break;
            ConstExp *constexp = nullptr;
            ANALYSIS_NODE(constexp, root, ConstExp, i);
            if (constexp != nullptr)
            {
                arrSTE.dimension.push_back(std::stoi(constexp->v));
                arraySize *= std::stoi(constexp->v);
            }
        }
        /*
        ir::Operand operand;
        vector<int> dimension;//数组操作数
        */
        ir::Type arrayType = type;
        // ir::Operand
        if (arrayType == ir::Type::Int)
            arrayType = ir::Type::IntPtr;
        else if (arrayType == ir::Type::Float)
            arrayType = ir::Type::FloatPtr;
        arrSTE.operand = ir::Operand(variableName, arrayType);
        // 加入ste
        symbol_table.scope_stack.back().table[variableName] = arrSTE;
        // 生成汇编指令
        if (symbol_table.scope_stack.size() > 1)
        {
            ir::Instruction *allocInst = new ir::Instruction(ir::Operand(to_string(arraySize), ir::Type::IntLiteral), ir::Operand(),
                                                             ir::Operand(symbol_table.get_scoped_name(variableName), arrayType), ir::Operator::alloc);
            instrVec.push_back(allocInst);
        }
        if (InitVal *initval = dynamic_cast<InitVal *>(root->children.back()))
        {
            vector<Instruction *> instructions = analyzeInitVal(initval, arrayType, arraySize, variableName);
            merge(instrVec, instructions);
        }
    }
    return instrVec;
}

// InitVal -> Exp | '{' [ InitVal { ',' InitVal } ] '}'
// 类似于 ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
vector<ir::Instruction *> frontend::Analyzer::analyzeInitVal(InitVal *root, ir::Type arrayType, int arraySize, string var_name)
{
    vector<ir::Instruction *> instrVec;
    int count = 0;
    for (unsigned int i = 1; i < root->children.size() - 1; i = i + 2, count++)
    {
        InitVal *Subinit = dynamic_cast<InitVal *>(root->children[i]);
        Exp *exp;
        vector<Instruction *> expInstruction;
        ANALYSIS_NODE_WITH_LeftVal(exp, Subinit, Exp, 0, expInstruction);
        merge(instrVec, expInstruction);
        ir::Type targettype;
        string value;
        ir::Type ptrType;
        if (arrayType == ir::Type::IntPtr)
        {
            ptrType = ir::Type::IntPtr;
            if (exp->t == Type::IntLiteral || exp->t == Type::Int)
            {
                targettype = exp->t;
                value = exp->v;
            }
            else if (exp->t == Type::FloatLiteral)
            {
                int val = std::stof(exp->v);
                value = to_string(val);
                targettype = Type::IntLiteral;
            }
            else if (exp->t == Type::Float)
            {
                string tmp_floatcvt_name = "t";
                tmp_floatcvt_name += std::to_string(tmp_cnt++);
                ir::Instruction *cvtInst = new ir::Instruction(ir::Operand(exp->v, ir::Type::Float), ir::Operand(),
                                                               ir::Operand(tmp_floatcvt_name, ir::Type::Int), ir::Operator::cvt_f2i);
                instrVec.push_back(cvtInst);
                value = tmp_floatcvt_name;
                targettype = Type::Int;
            }
        }
        else if (arrayType == ir::Type::FloatPtr)
        {
            ptrType = ir::Type::FloatPtr;
            if (exp->t == Type::Float || exp->t == Type::FloatLiteral)
            {
                targettype = exp->t;
                value = exp->v;
            }
            else if (exp->t == Type::IntLiteral)
            {
                int val = std::stoi(exp->v);
                value = to_string(val);
                targettype = Type::FloatLiteral;
            }
            else if (exp->t == Type::Int)
            {
                string tmp_intcvt_name = "t";
                tmp_intcvt_name += std::to_string(tmp_cnt++);
                ir::Instruction *cvtInst = new ir::Instruction(ir::Operand(exp->v, ir::Type::Int),
                                                               ir::Operand(),
                                                               ir::Operand(tmp_intcvt_name, ir::Type::Float), ir::Operator::cvt_i2f);
                instrVec.push_back(cvtInst);
                instrVec.push_back(cvtInst);
                value = tmp_intcvt_name;
                targettype = Type::Float;
            }
        }
        ir::Instruction *storeInst = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(var_name), ptrType),
                                                         ir::Operand(std::to_string(count), ir::Type::IntLiteral),
                                                         ir::Operand(value, targettype), ir::Operator::store);
        instrVec.push_back(storeInst);
    }
    // 数组初始化
    if (arrayType == ir::Type::FloatPtr || arrayType == ir::Type::IntPtr)
    {
        for (unsigned int i = count; i < arraySize; i++)
        {
            ir::Instruction *storeInst;
            ir::Type targetType = arrayType == ir::Type::FloatPtr ? ir::Type::FloatPtr : ir::Type::IntPtr;
            ir::Type irType = arrayType == ir::Type::FloatPtr ? ir::Type::FloatLiteral : ir::Type::IntLiteral;
            string val = arrayType == ir::Type::FloatPtr ? "0.0" : "0";
            storeInst = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(var_name), targetType),
                                            ir::Operand(std::to_string(i), ir::Type::IntLiteral),
                                            ir::Operand(val, irType), ir::Operator::store);
            instrVec.push_back(storeInst);
        }
    }
    return instrVec;
}

// FuncType -> 'void' | 'int' | 'float' 函数类型
ir::Type frontend::Analyzer::analyzeFuncType(FuncType *root)
{
    Term *term = dynamic_cast<Term *>(root->children[0]);
    if (term->token.type == TokenType::VOIDTK)
        return ir::Type::null;
    else if (term->token.type == TokenType::INTTK)
        return ir::Type::Int;
    else if (term->token.type == TokenType::FLOATTK)
        return ir::Type::Float;
}

// FuncFParam -> BType Ident ['[' ']' { '[' Exp ']' }]
//                  int a   ||  int b[]  ||   int c[][exp]
ir::Operand frontend::Analyzer::analyzeFuncFParam(FuncFParam *root)
{
    // 获取变量类型
    ir::Type type;
    BType *btype;
    ANALYSIS_NODE_WITH_LeftVal(btype, root, BType, 0, type);
    // 获取变量名
    string name = dynamic_cast<Term *>(root->children[1])->token.value;
    vector<int> dimension = {-1};
    // 数组类型参数
    if (root->children.size() > 2)
    {
        // int数组，类型是IntPtr
        if (type == ir::Type::Int)
            type = ir::Type::IntPtr;
        // Float数组，类型是FloatPtr
        else if (type == ir::Type::Float)
            type = ir::Type::FloatPtr;
        // 二维数组 int a [][exp]
        if (root->children.size() == 7)
        {
            Exp *exp;
            vector<Instruction *> instructions;
            ANALYSIS_NODE_WITH_LeftVal(exp, root, Exp, 6, instructions);
            int value = std::stoi(exp->v);
            dimension.push_back(value);
        }
    }
    ir::Operand param(name, type);
    symbol_table.scope_stack.back().table[name] = {param, dimension};
    ir::Operand funcfParamOperand(symbol_table.get_scoped_name(param.name), param.type);
    return funcfParamOperand;
}

// FuncFParams -> FuncFParam { ',' FuncFParam }
vector<ir::Operand> frontend::Analyzer::analyzeFuncFParams(FuncFParams *root)
{
    vector<ir::Operand> funcFParams;
    for (unsigned int i = 0; i < root->children.size(); i++)
    {
        FuncFParam *funcfparam = nullptr;
        ir::Operand funcfParamOperand;
        ANALYSIS_NODE_WITH_LeftVal(funcfparam, root, FuncFParam, i, funcfParamOperand);
        if (funcfparam != nullptr)
            funcFParams.push_back(funcfParamOperand);
    }
    return funcFParams;
}

// BType -> 'int' | 'float'
ir::Type frontend::Analyzer::analyzeBType(BType *root)
{
    Term *term = dynamic_cast<Term *>(root->children[0]);
    if (term->token.type == TokenType::INTTK)
        return ir::Type::Int;
    else if (term->token.type == TokenType::FLOATTK)
        return ir::Type::Float;
}

// FuncDef -> FuncType Ident '(' [FuncFParams] ')' Block
void frontend::Analyzer::analyzeFuncDef(FuncDef *root)
{
    // 函数类型
    ir::Type irType;
    FuncType *func_type;
    ANALYSIS_NODE_WITH_LeftVal(func_type, root, FuncType, 0, irType);
    // 函数名
    string name = dynamic_cast<Term *>(root->children[1])->token.value;
    // 为函数创建作用域
    symbol_table.add_scope();
    // 参数列表 '(' [FuncFParams] ')'
    vector<ir::Operand> funcParamsVec;
    FuncFParams *funcFParams;
    ANALYSIS_NODE_WITH_LeftVal(funcFParams, root, FuncFParams, 3, funcParamsVec)
        // 构造Function
        ir::Function *function = new ir::Function(name, funcParamsVec, irType);
    // 如果函数为main函数，则在其中增加全局函数调用
    if (name == "main")
    {
        ir::CallInst *callGlobal = new ir::CallInst(ir::Operand("global", ir::Type::null), ir::Operand("t" + to_string(tmp_cnt++), ir::Type::null));
        function->addInst(callGlobal);
    }
    // 函数加入符号表
    symbol_table.functions[name] = function;
    // Block
    curFunctionPtr = function;
    vector<ir::Instruction *> funcInstrVec = analyzeBlock(dynamic_cast<Block *>(root->children.back()));
    // 退出作用域
    symbol_table.exit_scope();
    // 将函数内的指令添加到函数
    for (auto inst : funcInstrVec)
        function->addInst(inst);
    // 还需要补充return
    // main函数加一条return 0
    if (name == "main")
    {
        Instruction *returnInstuction = new ir::Instruction(ir::Operand("0", ir::Type::IntLiteral), ir::Operand(), ir::Operand(), ir::Operator::_return);
        function->addInst(returnInstuction);
    }
    // VOID函数添加return null指令
    if (irType == ir::Type::null)
    {
        Instruction *returnInstuction = new ir::Instruction(ir::Operand(), ir::Operand(), ir::Operand(), ir::Operator::_return);
        function->addInst(returnInstuction);
    }
    irProgram.addFunction(*function);
}

// Block -> '{' { BlockItem } '}'
vector<ir::Instruction *> frontend::Analyzer::analyzeBlock(Block *root)
{
    vector<ir::Instruction *> instrVec;
    for (int i = 1; i < int(root->children.size()) - 1; ++i)
    {
        BlockItem *blockitem;
        vector<ir::Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(blockitem, root, BlockItem, i, instructions);
        merge(instrVec, instructions);
    }
    return instrVec;
}

// BlockItem -> Decl | Stmt
vector<ir::Instruction *> frontend::Analyzer::analyzeBlockItem(BlockItem *root)
{
    ANALYSIS_NODE_WITH_RETURN(root, Decl, 0);
    ANALYSIS_NODE_WITH_RETURN(root, Stmt, 0);
}
//  Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | [Exp] ';'
vector<ir::Instruction *> frontend::Analyzer::analyzeStmt(Stmt *root)
{
    vector<ir::Instruction *> instrVec;
    // LVal '=' Exp ';'
    if (LVal *lval = dynamic_cast<LVal *>(root->children[0]))
    {
        Exp *exp; // 这里的exp是LVal '=' Exp '
        vector<ir::Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(exp, root, Exp, 2, instructions);
        merge(instrVec, instructions);
        string variableName = dynamic_cast<Term *>(lval->children[0])->token.value;
        STE variableSTE = symbol_table.get_ste(variableName);
        string val;
        ir::Type irType = ir::Type::Int;
        ir::Operator irOperator = ir::Operator::mov;
        instructions = analyzeLval(lval, true, exp);
        merge(instrVec, instructions);
        return instrVec;
    }
    // Block
    ANALYSIS_NODE_WITH_RETURN(root, Block, 0);
    // [Exp] ';'
    // 符号[...]表示方括号内包含的为可选项, 有或者无
    ANALYSIS_NODE_WITH_RETURN(root, Exp, 0);
    // if while break continue return
    Term *term = dynamic_cast<Term *>(root->children[0]);
    // ';'
    if (term->token.type == TokenType::SEMICN)
        return instrVec;
    // return
    if (term->token.type == TokenType::RETURNTK)
    {
        Exp *exp = nullptr;
        vector<ir::Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(exp, root, Exp, 1, instructions);
        // 'return' [Exp] ';'
        if (root->children.size() == 3)
        {
            merge(instrVec, instructions);
            string val;
            ir::Type irType = ir::Type::Int;
            ir::Operator irOperator = ir::Operator::_return;
            ir::Instruction *returnInstion;
            if (curFunctionPtr->returnType == Type::Int)
            {
                if (exp->t == ir::Type::Int || exp->t == ir::Type::IntLiteral)
                {
                    val = exp->v;
                    irType = exp->t;
                    irOperator = ir::Operator::_return;
                }
                else if (exp->t == ir::Type::Float)
                {
                    /* cvt_f2i
                    浮点型变量转为整型变量，第一个操作数为待转换变量，结果为类型转换后变量，第二个操作数不使用。示例如下：
                    float a = 2;int b = a; => cvt_f2i b, a
                    */
                    string tmpFloatcvtName = "a" + to_string(tmp_cnt++);
                    val = exp->v;
                    irType = ir::Type::Float;
                    irOperator = ir::Operator::cvt_f2i;
                    ir::Instruction *cvtInst = new ir::Instruction(ir::Operand(val, irType), ir::Operand(),
                                                                   ir::Operand(tmpFloatcvtName, Type::Int), irOperator);
                    instrVec.push_back(cvtInst);
                    val = tmpFloatcvtName;
                    irType = ir::Type::Int;
                    irOperator = ir::Operator::_return;
                }
                else if (exp->t == ir::Type::FloatLiteral)
                {
                    int value = std::stof(exp->v);
                    val = to_string(value);
                    irType = ir::Type::IntLiteral;
                    irOperator = ir::Operator::_return;
                }
                returnInstion = new ir::Instruction(ir::Operand(val, irType), ir::Operand(), ir::Operand(), irOperator);
                instrVec.push_back(returnInstion);
            }
            else if (curFunctionPtr->returnType == Type::Float)
            {
                if (exp->t == ir::Type::Float || exp->t == ir::Type::FloatLiteral)
                {
                    val = exp->v;
                    irType = exp->t;
                    irOperator = ir::Operator::_return;
                }
                else if (exp->t == ir::Type::Int)
                {
                    /* cvt_i2f
                    浮点型变量转为整型变量，第一个操作数为待转换变量，结果为类型转换后变量，第二个操作数不使用。示例如下：
                    float a = 2;int b = a; => cvt_f2i b, a
                    */
                    string tmpFloatcvtName = "a" + to_string(tmp_cnt++);
                    val = exp->v;
                    irType = ir::Type::Int;
                    irOperator = ir::Operator::cvt_i2f;
                    ir::Instruction *cvtInst = new ir::Instruction(ir::Operand(val, irType), ir::Operand(),
                                                                   ir::Operand(tmpFloatcvtName, Type::Float), irOperator);
                    instrVec.push_back(cvtInst);
                    val = tmpFloatcvtName;
                    irType = ir::Type::Float;
                    irOperator = ir::Operator::_return;
                }
                else if (exp->t == ir::Type::IntLiteral)
                {
                    int value = std::stoi(exp->v);
                    val = to_string(value);
                    irType = ir::Type::FloatLiteral;
                    irOperator = ir::Operator::_return;
                }
                returnInstion = new ir::Instruction(ir::Operand(val, irType), ir::Operand(), ir::Operand(), irOperator);
                instrVec.push_back(returnInstion);
            }
        }
        else
        { // return;
            ir::Instruction *returnInstion = new ir::Instruction(ir::Operand(), ir::Operand(),
                                                                 ir::Operand(), ir::Operator::_return);
            instrVec.push_back(returnInstion);
        }
        return instrVec;
    }
    // if
    if (term->token.type == TokenType::IFTK)
    {
        // 'if' '(' Cond ')' Stmt [ 'else' Stmt ]
        Cond *cond;
        vector<ir::Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(cond, root, Cond, 2, instructions);
        merge(instrVec, instructions);
        /*
        0       if instruction -if goto elsestmt
        1       goto else
        size1   stmt instrctions
        1       goto nextinstion
        size2   elsestmt instions
        1       next insttion
            */
        // // goto     [op1=cond_var/null],    des = offset
        string offset = "2";
        // 判断if,为true跳到stmt instrctions ，偏移2
        // 为假则顺序执行 goto else
        ir::Instruction *gotoInstruction = new ir::Instruction(ir::Operand(cond->v, cond->t), ir::Operand(),
                                                               ir::Operand(offset, Type::IntLiteral), ir::Operator::_goto);
        instrVec.push_back(gotoInstruction);
        symbol_table.add_scope();
        vector<ir::Instruction *> StmtInstruction;
        Stmt *stmt;
        ANALYSIS_NODE_WITH_LeftVal(stmt, root, Stmt, 4, StmtInstruction);
        symbol_table.exit_scope();
        // 如果有'else' Stmt
        symbol_table.add_scope();
        vector<ir::Instruction *> ElseStmtInstruction;
        ANALYSIS_NODE_WITH_LeftVal(stmt, root, Stmt, 6, ElseStmtInstruction);
        symbol_table.exit_scope();
        /*
        goto
        跳转指令。每条IR生成都会对应一标签， goto IR 跳转到某个标签的 IR 处。
        第一个操作数为跳转条件，其为整形变量或 type = Type::null 的变量，当为整形变量时表示条件跳转（值不等于0发生跳转），否则为无条件跳转。
        第二个操作数不使用，
        目的操作数应为整形，其值为跳转相对目前pc的偏移量
        */
        // 在执行了if后，应该跳转到if块的后一条指令:NextInstruction，偏移量=ElseStmtInstruction.size() + 1
        offset = to_string(ElseStmtInstruction.size() + 1);
        ir::Instruction *gotoNextInstruction = new ir::Instruction(ir::Operand(), ir::Operand(),
                                                                   ir::Operand(offset, Type::IntLiteral),
                                                                   ir::Operator::_goto);
        // 有else，在StmtInstruction加入这条指令gotoNextInstruction
        if (root->children.size() == 7)
            StmtInstruction.push_back(gotoNextInstruction);
        // 按照顺序，if后面跟else,如果if为false，进入else分支，偏移量=更新后的StmtInstruction.size() + 1
        offset = to_string(StmtInstruction.size() + 1);
        ir::Instruction *gotoElseStmtInstruction = new ir::Instruction(ir::Operand(), ir::Operand(),
                                                                       ir::Operand(offset, Type::IntLiteral),
                                                                       ir::Operator::_goto);
        instrVec.push_back(gotoElseStmtInstruction);
        merge(instrVec, StmtInstruction);
        // 有else，才需要加入ElseStmtInstruction
        if (root->children.size() == 7)
            merge(instrVec, ElseStmtInstruction);
        /*        unuse
        生成一条带有标签但无实际含义的IR，第一个操作数、第二个操作数与结果均不使用。可用于避免某
         些分支跳转情况假出口跳转到未知标签。示例如下：
        */
        Instruction *unuseInstruction = new ir::Instruction(ir::Operand(), ir::Operand(),
                                                            ir::Operand(), ir::Operator::__unuse__);
        instrVec.push_back(unuseInstruction);
        return instrVec;
    }
    // 'while' '(' Cond ')' Stmt
    if (term->token.type == TokenType::WHILETK)
    {
        // Cond
        Cond *cond;
        vector<ir::Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(cond, root, Cond, 2, instructions);
        merge(instrVec, instructions);

        symbol_table.add_scope();
        vector<Instruction *> StmtInstruction;
        Stmt *stmt;
        ANALYSIS_NODE_WITH_LeftVal(stmt, root, Stmt, 4, StmtInstruction);
        symbol_table.exit_scope();
        /*
        condInstructions
        while 判断 goto +2
        goto next
        stmt instructions
        continueInstruction
        next instruction
        */
        string offset = "2";
        // 进入while
        Instruction *gotoInstruction = new ir::Instruction(ir::Operand(cond->v, cond->t), ir::Operand(),
                                                           ir::Operand(offset, Type::IntLiteral), ir::Operator::_goto);
        // stmt结尾要跳回while，前的cond,用自定义的continue
        Instruction *continueInstruction = new ir::Instruction(ir::Operand("continue", Type::null), ir::Operand(),
                                                               ir::Operand(), ir::Operator::__unuse__);
        StmtInstruction.push_back(continueInstruction);

        Instruction *gotoNextInstruction = new ir::Instruction(ir::Operand(), ir::Operand(),
                                                               ir::Operand(to_string(StmtInstruction.size() + 1), Type::IntLiteral), ir::Operator::_goto);
        for (unsigned int i = 0; i < StmtInstruction.size(); i++)
        {
            string op1 = StmtInstruction[i]->op1.name;
            if (StmtInstruction[i]->op == Operator::__unuse__)
            {
                if (op1 == "continue")
                {
                    offset = to_string(-(2 + int(i) + int(instructions.size())));
                    StmtInstruction[i] = new ir::Instruction(ir::Operand(), ir::Operand(),
                                                             ir::Operand(offset, Type::IntLiteral), ir::Operator::_goto);
                }
                else if (op1 == "break")
                {
                    offset = to_string(int(StmtInstruction.size()) - i);
                    StmtInstruction[i] = new ir::Instruction(ir::Operand(), ir::Operand(),
                                                             ir::Operand(offset, Type::IntLiteral), ir::Operator::_goto);
                }
            }
        }
        instrVec.push_back(gotoInstruction);
        instrVec.push_back(gotoNextInstruction);
        merge(instrVec, StmtInstruction);
        /*        unuse
        生成一条带有标签但无实际含义的IR，第一个操作数、第二个操作数与结果均不使用。可用于避免某
         些分支跳转情况假出口跳转到未知标签。示例如下：
        */
        Instruction *unuseInstruction = new ir::Instruction(ir::Operand(), ir::Operand(),
                                                            ir::Operand(), ir::Operator::__unuse__);
        instrVec.push_back(unuseInstruction);
        return instrVec;
    }
    // 'break' ';'
    if (term->token.type == TokenType::BREAKTK)
    { // add costume break instruction,这里不知道stmt的大小和i的位置, 在WHILE中处理, 并替换为goto指令
        Instruction *breakInstruction = new Instruction(Operand("break", Type::null), Operand(),
                                                        Operand(), Operator::__unuse__);
        instrVec.push_back(breakInstruction);
        return instrVec;
    }
    // 'continue' ';'
    if (term->token.type == TokenType::CONTINUETK)
    { // add costume continue instruction,这里不知道stmt的大小和i的位置, 在WHILE中处理, 并替换为goto指令
        Instruction *continueInstruction = new Instruction(Operand("continue", Type::null), Operand(),
                                                           Operand(), Operator::__unuse__);
        instrVec.push_back(continueInstruction);
        return instrVec;
    }
}

// Exp -> AddExp
vector<ir::Instruction *> frontend::Analyzer::analyzeExp(Exp *root)
{
    AddExp *child;
    vector<ir::Instruction *> instructions;
    ANALYSIS_NODE_WITH_LeftVal(child, root, AddExp, 0, instructions);
    root->v = child->v;
    root->t = child->t;
    return instructions;
}

// ConstExp -> AddExp is same as Exp -> AddExp
vector<ir::Instruction *> frontend::Analyzer::analyzeConstExp(ConstExp *root)
{
    AddExp *child;
    vector<ir::Instruction *> instructions;
    ANALYSIS_NODE_WITH_LeftVal(child, root, AddExp, 0, instructions);
    root->v = child->v;
    root->t = child->t;
    return instructions;
}
int getPriority(Type t)
{
    switch (t)
    {
    case Type::Float:
        return 4; // 优先级最高
    case Type::Int:
        return 3;
    case Type::FloatLiteral:
        return 2;
    case Type::IntLiteral:
        return 1; // 优先级最低
    default:
        return 0; // 未知类型
    }
}

//  AddExp -> MulExp { ('+' | '-') MulExp }
template <typename T>
vector<ir::Instruction *> frontend::Analyzer::ChangeType(Type promaxType, T *root)
{
    vector<ir::Instruction *> instrVec;
    string val;
    ir::Type irType1;
    ir::Type irType2;
    ir::Operator iroperator;
    string tmpIntcvtName;
    if (promaxType != root->t)
    {
        if (promaxType == Type::Int)
        {
            val = root->v;
            irType1 = ir::Type::IntLiteral;
            irType2 = ir::Type::Int;
            tmpIntcvtName = "a" + to_string(tmp_cnt++);
            iroperator = ir::Operator::def;
            Instruction *cvtInst = new Instruction(ir::Operand(val, irType1), ir::Operand(), ir::Operand(tmpIntcvtName, irType2), iroperator);
            instrVec.push_back(cvtInst);
            root->v = tmpIntcvtName;
            root->t = irType2;
        }

        else if (promaxType == Type::Float)
        {
            if (root->t == Type::IntLiteral)
            {
                float value = std::stof(root->v);
                val = to_string(value);
                irType1 = ir::Type::FloatLiteral;
                irType2 = ir::Type::Float;
                tmpIntcvtName = "a" + to_string(tmp_cnt++);
                ir::Operator iroperator = ir::Operator::fdef;
                Instruction *cvtInst = new Instruction(ir::Operand(val, irType1), ir::Operand(), ir::Operand(tmpIntcvtName, irType2), iroperator);
                instrVec.push_back(cvtInst);
                root->v = tmpIntcvtName;
                root->t = irType2;
            }
            else if (root->t == Type::Int)
            {
                val = root->v;
                irType1 = ir::Type::Int;
                irType2 = ir::Type::Float;
                tmpIntcvtName = "a" + to_string(tmp_cnt++);
                iroperator = ir::Operator::cvt_i2f;
                Instruction *cvtInst = new Instruction(ir::Operand(val, irType1), ir::Operand(), ir::Operand(tmpIntcvtName, irType2), iroperator);
                instrVec.push_back(cvtInst);
                root->v = tmpIntcvtName;
                root->t = irType2;
            }
            else if (root->t == Type::FloatLiteral)
            {
                val = root->v;
                irType1 = ir::Type::FloatLiteral;
                irType2 = ir::Type::Float;
                tmpIntcvtName = "a" + to_string(tmp_cnt++);
                iroperator = ir::Operator::fdef;
                Instruction *cvtInst = new Instruction(ir::Operand(val, irType1), ir::Operand(), ir::Operand(tmpIntcvtName, irType2), iroperator);
                instrVec.push_back(cvtInst);
                root->v = tmpIntcvtName;
                root->t = irType2;
            }
        }

        if (promaxType == Type::FloatLiteral)
        {
            float val = std::stoi(root->v);
            root->v = to_string(val);
            root->t = Type::FloatLiteral;
        }
    }

    return instrVec;
}

vector<ir::Instruction *> frontend::Analyzer::analyzeAddExp(AddExp *root)
{
    vector<Instruction *> instrVec;
    Type promaxType = Type::IntLiteral;
    for (unsigned int i = 0; i < root->children.size(); i += 2)
    {
        MulExp *mulexp;
        vector<Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(mulexp, root, MulExp, i, instructions);
        merge(instrVec, instructions);
        // 记录最高优先级的变量类型
        if ((promaxType == Type::FloatLiteral && mulexp->t == Type::Int) || (promaxType == Type::Int && mulexp->t == Type::FloatLiteral))
        {
            promaxType = Type::Float;
        }
        else if (getPriority(mulexp->t) > getPriority(promaxType))
        {
            promaxType = mulexp->t;
        }
    }
    MulExp *MulExp1 = dynamic_cast<MulExp *>(root->children[0]);
    COPY_EXP_NODE(MulExp1, root);
    if (root->children.size() == 1)
        return instrVec;
    // 进行类型转换
    vector<Instruction *> changeTypeInstruction = ChangeType(promaxType, root);
    merge(instrVec, changeTypeInstruction);
    for (unsigned int i = 2; i < root->children.size(); i += 2)
    {
        MulExp *mulexp = dynamic_cast<MulExp *>(root->children[i]);
        Term *curOperator = dynamic_cast<Term *>(root->children[i - 1]);
        // 进行类型转换
        vector<Instruction *> changeTypeInstruction = ChangeType(promaxType, mulexp);
        merge(instrVec, changeTypeInstruction);
        // 已经转化
        // 立即数直接算就可以了
        if (promaxType == Type::IntLiteral)
        {
            // 如果是IntLiteral,+-不会影响计算结果
            int value1 = std::stoi(root->v);
            int value2 = std::stoi(mulexp->v);
            root->v = to_string(curOperator->token.type == TokenType::PLUS ? value1 + value2 : value1 - value2);
        }
        else if (promaxType == Type::FloatLiteral)
        {
            // 如果是IntLiteral,+-不会影响计算结果
            float value1 = std::stof(root->v);
            float value2 = std::stof(mulexp->v);
            root->v = to_string(curOperator->token.type == TokenType::PLUS ? value1 + value2 : value1 - value2);
        }
        else
        {
            ir::Type irType = ir::Type::Int;
            ir::Operator irOperator = ir::Operator::add;
            string instructionName = "a" + to_string(tmp_cnt++);
            Instruction *computeInstruction;
            string coumputeName = "a" + std::to_string(tmp_cnt++);
            if (promaxType == Type::Int)
            {
                if (promaxType == Type::Int)
                {
                    if (curOperator->token.type == TokenType::PLUS)
                    {
                        irType = ir::Type::Int;
                        irOperator = ir::Operator::add;
                    }
                    else if (curOperator->token.type == TokenType::MINU)
                    {
                        irType = ir::Type::Int;
                        irOperator = ir::Operator::sub;
                    }
                }
                computeInstruction = new Instruction(ir::Operand(root->v, irType),
                                                     ir::Operand(mulexp->v, irType),
                                                     ir::Operand(instructionName, irType), irOperator);
                instrVec.push_back(computeInstruction);
                root->v = instructionName;
            }
            else if (promaxType == Type::Float)
            {
                if (curOperator->token.type == TokenType::PLUS)
                {
                    irType = ir::Type::Float;
                    irOperator = ir::Operator::fadd;
                }
                else if (curOperator->token.type == TokenType::MINU)
                {
                    irType = ir::Type::Float;
                    irOperator = ir::Operator::fsub;
                }
                computeInstruction = new Instruction(ir::Operand(root->v, irType),
                                                     ir::Operand(mulexp->v, irType),
                                                     ir::Operand(instructionName, irType), irOperator);
                instrVec.push_back(computeInstruction);
                root->v = instructionName;
            }
        }
    }
    return instrVec;
}

// MulExp -> UnaryExp { ('*' | '/' | '%') UnaryExp }
vector<ir::Instruction *> frontend::Analyzer::analyzeMulExp(MulExp *root)
{
    vector<Instruction *> instrVec;
    Type promaxType = Type::IntLiteral;
    for (unsigned int i = 0; i < root->children.size(); i += 2)
    {
        UnaryExp *unaryexp;
        vector<Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(unaryexp, root, UnaryExp, i, instructions);
        merge(instrVec, instructions);

        if ((unaryexp->t == ir::Type::FloatLiteral && promaxType == ir::Type::Int) || (promaxType == ir::Type::FloatLiteral && unaryexp->t == ir::Type::Int))
            promaxType = ir::Type::Float;
        else if (getPriority(unaryexp->t) > getPriority(promaxType))
        {
            promaxType = unaryexp->t;
        }
    }

    UnaryExp *UnaryExp1 = dynamic_cast<UnaryExp *>(root->children[0]);
    COPY_EXP_NODE(UnaryExp1, root);
    vector<Instruction *> changeTypeInstruction = ChangeType(promaxType, root);
    merge(instrVec, changeTypeInstruction);

    for (unsigned int i = 2; i < root->children.size(); i += 2)
    {
        UnaryExp *unaryexp = dynamic_cast<UnaryExp *>(root->children[i]);
        Term *curOperator = dynamic_cast<Term *>(root->children[i - 1]);
        // 类型转换
        vector<Instruction *> changeTypeInstruction = ChangeType(promaxType, unaryexp);
        merge(instrVec, changeTypeInstruction);
        if (promaxType == Type::IntLiteral)
        {
            int value1 = std::stoi(root->v);
            int value2 = std::stoi(unaryexp->v);
            int ans;
            if (curOperator->token.type == TokenType::MULT)
                ans = value1 * value2;
            else if (curOperator->token.type == TokenType::DIV)
                ans = value1 / value2;
            else if (curOperator->token.type == TokenType::MOD)
                ans = value1 % value2;
            root->v = to_string(ans);
        }
        else if (promaxType == Type::FloatLiteral)
        {
            float value1 = std::stof(root->v);
            float value2 = std::stof(unaryexp->v);
            float ans;
            if (curOperator->token.type == TokenType::MULT)
                ans = value1 * value2;
            else if (curOperator->token.type == TokenType::DIV)
                ans = value1 / value2;
            root->v = to_string(ans);
        }
        else
        {
            ir::Type irType = ir::Type::Int;
            ir::Operator irOperator = ir::Operator::add;
            string instructionName;
            Instruction *computeInstruction;
            if (promaxType == Type::Int)
            {
                instructionName = "a" + to_string(tmp_cnt++);
                irType = ir::Type::Int;
                if (curOperator->token.type == TokenType::MULT)
                    irOperator = ir::Operator::mul;
                else if (curOperator->token.type == TokenType::DIV)
                    irOperator = ir::Operator::div;
                else if (curOperator->token.type == TokenType::MOD)
                    irOperator = ir::Operator::mod;
            }
            else if (promaxType == Type::Float)
            {
                instructionName = "a" + to_string(tmp_cnt++);
                irType = ir::Type::Float;
                if (curOperator->token.type == TokenType::MULT)
                    irOperator = ir::Operator::fmul;
                else if (curOperator->token.type == TokenType::DIV)
                    irOperator = ir::Operator::fdiv;
            }
            computeInstruction = new Instruction(ir::Operand(root->v, irType),
                                                 ir::Operand(unaryexp->v, irType),
                                                 ir::Operand(instructionName, irType), irOperator);
            instrVec.push_back(computeInstruction);
            root->v = instructionName;
        }
    }
    return instrVec;
}

// UnaryExp -> PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp
vector<ir::Instruction *> frontend::Analyzer::analyzeUnaryExp(UnaryExp *root)
{
    vector<ir::Instruction *> instrVec;
    // PrimaryExp
    if (PrimaryExp *primaryexp = dynamic_cast<PrimaryExp *>(root->children[0]))
    {
        vector<Instruction *> instructions = analyzePrimaryExp(primaryexp);
        ANALYSIS_NODE_WITH_LeftVal(primaryexp, root, PrimaryExp, 0, instructions);
        merge(instrVec, instructions);
        COPY_EXP_NODE(primaryexp, root);
        return instrVec;
    }
    //  Ident '(' [FuncRParams] ')'
    // 获取函数名
    if (Term *term = dynamic_cast<Term *>(root->children[0]))
    {
        if (term->token.type == TokenType::IDENFR)
        {
            // 提取函数名
            string functionName = term->token.value;
            Function *curFunction = symbol_table.functions[functionName];
            vector<Operand> paraVec;
            // [FuncRParams]
            // 分析函数参数
            if (auto *funcrparams = dynamic_cast<FuncRParams *>(root->children[2]))
            {
                // 获取当前函数的参数类型
                vector<Operand> ParameterTypeList = curFunction->ParameterList;
                // 分析函数参数并生成相应的指令
                vector<ir::Instruction *> paraInstructionVec = analyzeFuncRParams(funcrparams, ParameterTypeList, paraVec);
                // 合并指令
                merge(instrVec, paraInstructionVec);
            }
            // 分析函数返回值
            string returnVaribleName = "a" + std::to_string(tmp_cnt++);
            /*"函数调用" instruction
            struct CallInst: public Instruction{
                std::vector<Operand> argumentList;//用于存入函数调用实参
                CallInst(const Operand& op1, std::vector<Operand> paraList, const Operand& des);
                CallInst(const Operand& op1, const Operand& des);   //无参数情况
                std::string draw() const;
            };
            */
            ir::CallInst *callInst = new ir::CallInst(ir::Operand(curFunction->name, curFunction->returnType), paraVec,
                                                      ir::Operand(returnVaribleName, curFunction->returnType));
            instrVec.push_back(callInst);
            CHANGE_NODE(root, returnVaribleName, curFunction->returnType);
            return instrVec;
        }
    }

    // UnaryOp UnaryExp + - !
    if (auto *unaryop = dynamic_cast<UnaryOp *>(root->children[0]))
    {
        // cal UnaryExp
        UnaryExp *unaryexp;
        vector<ir::Instruction *> cal_insts;
        ANALYSIS_NODE_WITH_LeftVal(unaryexp, root, UnaryExp, 1, cal_insts);
        merge(instrVec, cal_insts);

        Term *unaryOperator = dynamic_cast<Term *>(unaryop->children[0]);
        // 正数
        if (unaryOperator->token.type == TokenType::PLUS)
        {
            COPY_EXP_NODE(unaryexp, root);
        }
        // 负数
        else if (unaryOperator->token.type == TokenType::MINU)
        {

            if (unaryexp->t == Type::IntLiteral || unaryexp->t == Type::FloatLiteral)
            { // 立即数
                CHANGE_NODE(root, std::to_string(-std::stof(unaryexp->v)), unaryexp->t);
            }
            else
            { // 非立即数
                string zeroOperand;
                string zeroVal;
                ir::Type zeroOperandType;
                ir::Operator irOperator1;
                ir::Operator irOperator2;
                ir::Instruction *defInstruction;
                string minusOperand;
                if (unaryexp->t == Type::Int)
                {
                    // 用0减实现，添加指令
                    zeroOperand = "a" + std::to_string(tmp_cnt++);
                    zeroVal = "0";
                    zeroOperandType = ir::Type::IntLiteral;
                    irOperator1 = ir::Operator::def;
                    // 创建零时变量0
                    defInstruction = new ir::Instruction(ir::Operand(zeroVal, zeroOperandType), ir::Operand(),
                                                         ir::Operand(zeroOperand, unaryexp->t), irOperator1);
                    instrVec.push_back(defInstruction);
                    minusOperand = "a" + std::to_string(tmp_cnt++);
                    irOperator2 = ir::Operator::sub;
                    // 0 - 指令
                    ir::Instruction *minusIstruction = new ir::Instruction(ir::Operand(zeroOperand, unaryexp->t),
                                                                           ir::Operand(unaryexp->v, unaryexp->t),
                                                                           ir::Operand(minusOperand, unaryexp->t), irOperator2);

                    instrVec.push_back(minusIstruction);
                    CHANGE_NODE(root, minusOperand, unaryexp->t);
                }
                else if (unaryexp->t == Type::Float)
                {
                    string zeroOperand = "a" + std::to_string(tmp_cnt++);
                    string zeroVal = "0.0";
                    ir::Type zeroOperandType = (ir::Type::FloatLiteral);
                    ir::Operator irOperator1 = (ir::Operator::fdef);
                    // 创建零时变量0.0
                    ir::Instruction *defInstruction = new ir::Instruction(ir::Operand(zeroVal, zeroOperandType), ir::Operand(),
                                                                          ir::Operand(zeroOperand, unaryexp->t), irOperator1);
                    instrVec.push_back(defInstruction);
                    string minusOperand = "a" + std::to_string(tmp_cnt++);
                    ir::Operator irOperator2 = (ir::Operator::fsub);
                    // 0.0 - 指令
                    ir::Instruction *minusIstruction = new ir::Instruction(ir::Operand(zeroOperand, unaryexp->t),
                                                                           ir::Operand(unaryexp->v, unaryexp->t),
                                                                           ir::Operand(minusOperand, unaryexp->t), irOperator2);
                    instrVec.push_back(minusIstruction);
                    CHANGE_NODE(root, minusOperand, unaryexp->t);
                }
            }
        }
        // 取反
        else if (unaryOperator->token.type == TokenType::NOT)
        {
            if (unaryexp->t == Type::IntLiteral)
            {
                int tmp_result = !std::stoi(unaryexp->v);
                CHANGE_NODE(root, std::to_string(tmp_result), Type::IntLiteral)
            }
            else if (unaryexp->t == Type::FloatLiteral)
            {
                int tmp_result = !std::stof(unaryexp->v);
                CHANGE_NODE(root, std::to_string(tmp_result), Type::FloatLiteral)
            }
            else if (unaryexp->t == Type::Int || unaryexp->t == Type::Float)
            {
                string tmpNotName = "a" + std::to_string(tmp_cnt++);
                // 变量取非运算 ! ，第一个操作数为取非变量，第二个操作数不使用，结果为取非结果变量。
                ir::Instruction *tmpNotInstruction = new ir::Instruction(ir::Operand(unaryexp->v, unaryexp->t), ir::Operand(),
                                                                         ir::Operand(tmpNotName, ir::Type::Int), ir::Operator::_not);
                instrVec.push_back(tmpNotInstruction);
                CHANGE_NODE(root, tmpNotName, unaryexp->t);
            }
        }
        return instrVec;
    }
}

// FuncRParams -> Exp { ',' Exp }
/* 引用传参
    @brief  root:               FuncRParams 节点
            ParameterTypeList:  函数参数类型的列表
            paraVec:            参数列表
*/
vector<Instruction *> frontend::Analyzer::analyzeFuncRParams(FuncRParams *root, vector<Operand> &ParameterTypeList, vector<Operand> &paraVec)
{
    vector<ir::Instruction *> instrVec;
    // Exp { ',' Exp }
    for (unsigned int i = 0; i < root->children.size(); i = i + 2)
    {
        Exp *exp;
        vector<ir::Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(exp, root, Exp, i, instructions);
        merge(instrVec, instructions);
        // 统一变量类型，根据传入的参数类型列表，对exp的类型进行修正
        ir::Type curType = ParameterTypeList[int(i / 2)].type;
        ir::Type expType = exp->t;
        /*
        Int,          // 整型
        Float,        // 浮点型变量
        IntLiteral,   // 为立即数整型
        FloatLiteral, // 立即数浮点型
        IntPtr,//为整型指针
        FloatPtr,//浮点型指针
        null//当函数的返回值为 void 时，提供了特殊的 null 类型
        */
        string operandN;
        ir::Type operandT;
        if (curType == ir::Type::Int)
        {
            if (expType == Type::Float)
            {
                operandN = "a" + to_string(tmp_cnt++);
                operandT = ir::Type::Int;
                ir::Instruction *cvtInst = new Instruction(ir::Operand(exp->v, ir::Type::Float), ir::Operand(),
                                                           ir::Operand(operandN, operandT), ir::Operator::cvt_f2i);
                instrVec.push_back(cvtInst);
            }
            else if (expType == Type::FloatLiteral)
            {
                int value = std::stoi(exp->v);
                operandN = to_string(value);
                operandT = ir::Type::IntLiteral;
            }
            else if (expType == ir::Type::Int || expType == ir::Type::IntLiteral)
            {
                operandN = exp->v;
                operandT = expType;
            }
        }
        else if (curType == ir::Type::Float)
        {
            if (expType == Type::Int)
            {
                operandN = "a" + to_string(tmp_cnt++);
                operandT = ir::Type::Float;
                ir::Instruction *cvtInst = new Instruction(ir::Operand(exp->v, ir::Type::Int), ir::Operand(),
                                                           ir::Operand(operandN, operandT), ir::Operator::cvt_i2f);
                instrVec.push_back(cvtInst);
            }
            else if (expType == Type::IntLiteral)
            {
                int value = std::stof(exp->v);
                operandN = to_string(value);
                operandT = ir::Type::IntLiteral;
            }
            else if (expType == ir::Type::Float || expType == ir::Type::FloatLiteral)
            {
                operandN = exp->v;
                operandT = expType;
            }
        }
        else
        {
            operandN = exp->v;
            operandT = expType;
        }
        paraVec.push_back(Operand(operandN, operandT));
    }
    return instrVec;
}

// PrimaryExp -> '(' Exp ')' | LVal | Number
vector<Instruction *> frontend::Analyzer::analyzePrimaryExp(PrimaryExp *root)
{
    vector<Instruction *> instrVec;
    // Exp
    if (auto *term = dynamic_cast<Term *>(root->children[0]))
    {
        Exp *exp;
        ANALYSIS_NODE_WITH_LeftVal(exp, root, Exp, 1, instrVec);
        COPY_EXP_NODE(exp, root);
        return instrVec;
    }
    // LVal
    if (auto *lval = dynamic_cast<LVal *>(root->children[0]))
    {
        instrVec = analyzeLval(lval, false, nullptr);
        COPY_EXP_NODE(lval, root);
        return instrVec;
    }
    // Number
    if (auto *number = dynamic_cast<Number *>(root->children[0]))
    {
        ANALYSIS_NODE(number, root, Number, 0);
        COPY_EXP_NODE(number, root);
        return instrVec;
    }
}

// todo
// LVal -> Ident {'[' Exp ']'}
vector<Instruction *> frontend::Analyzer::analyzeLval(LVal *root, bool isStore = false, Exp *exp = nullptr)
{
    if (!isStore)
    {
        vector<Instruction *> instrVec;
        Term *term = dynamic_cast<Term *>(root->children[0]);
        string variableName = term->token.value;
        STE variableSTE = symbol_table.get_ste(variableName);
        string scopeVarName = symbol_table.get_scoped_name(variableName);
        Type promaxType;
        // LVal -> Ident
        if (root->children.size() == 1)
        {

            root->t = variableSTE.operand.type;
            if (root->t != Type::IntLiteral && root->t != Type::FloatLiteral)
                root->v = scopeVarName;
            else
                root->v = variableSTE.literalVal;
            return {};
        }
        // LVal -> Ident {'[' Exp ']'}
        else if (root->children.size() == 4)
        { // 形如，a[1]
            Exp *exp;
            vector<Instruction *> exp_result;
            ANALYSIS_NODE_WITH_LeftVal(exp, root, Exp, 2, exp_result);
            merge(instrVec, exp_result);
            string loadVariableName = "a" + to_string(tmp_cnt++);

            if (variableSTE.dimension.size() == 1)
            { // 形如，a[1]， a[1] 是数字

                if (variableSTE.operand.type == Type::IntPtr)
                {
                    promaxType = Type::Int;
                }
                else if (variableSTE.operand.type == Type::FloatPtr)
                {
                    promaxType = Type::Float;
                }
                Instruction *loadInst = new ir::Instruction(ir::Operand(scopeVarName, variableSTE.operand.type), // 基址
                                                            ir::Operand(exp->v, exp->t),                         // 偏移量
                                                            ir::Operand(loadVariableName, promaxType), ir::Operator::load);
                instrVec.push_back(loadInst);
            }
            else
            { //  a[1] 是数组
                promaxType = variableSTE.operand.type;
                // exp->t 有两种情形，IntLiteral和Int
                if (exp->t == Type::IntLiteral)
                {
                    /*
                    getptr获取指针指令，这实际上是一个指针运算指令，第一个操作数为数组名，第二个操作数为数组下标，
                    运算结果仍为指针，其值是数组名(基址)+数组下标(偏移量)之后的地址，目的操作数为存入的指针操作数。
                    */
                    int val = std::stoi(exp->v);
                    string offset = to_string(val * variableSTE.dimension[1]);
                    Instruction *getptrInstruction = new ir::Instruction(ir::Operand(scopeVarName, variableSTE.operand.type),
                                                                         ir::Operand(offset, Type::IntLiteral),
                                                                         ir::Operand(loadVariableName, promaxType), ir::Operator::getptr);
                    instrVec.push_back(getptrInstruction);
                }
                // else
                // {    // 实际上测试用例没有这种情形
                //     // 由于exp为int型，计算时需要列数*exp时，需要用乘法指令
                //     // mul 整型变量乘法指令，用于两操作数均为整型变量情况
                //     // 所以要 variableSTE.dimension[1] def 成int形变量
                //     string mulResultName = "a" + to_string(tmp_cnt++);
                //     string operand1Name = "a" + to_string(tmp_cnt++);
                //     Instruction *defInstruction = new ir::Instruction(ir::Operand(to_string(variableSTE.dimension[1]), Type::IntLiteral),
                //                                                       ir::Operand(),
                //                                                       ir::Operand(operand1Name, Type::Int), ir::Operator::def);
                //     Instruction *mulInstruction = new ir::Instruction(ir::Operand(exp->v, Type::Int),
                //                                                       ir::Operand(operand1Name, Type::Int),
                //                                                       ir::Operand(mulResultName, Type::Int), ir::Operator::mul);
                //     Instruction *getptrInstruction = new ir::Instruction(ir::Operand(scopeVarName, variableSTE.operand.type),
                //                                                          ir::Operand(mulResultName, Type::Int),
                //                                                          ir::Operand(loadVariableName, promaxType), ir::Operator::getptr);
                //     instrVec.push_back(defInstruction);
                //     instrVec.push_back(mulInstruction);
                //     instrVec.push_back(getptrInstruction);
                // }
            }
            CHANGE_NODE(root, loadVariableName, promaxType);
            return instrVec;
        }
        // n维数组a[2][1]
        else if (root->children.size() == 7)
        {
            ir::Operand operand1, operand2;
            for (int i = 2; i < root->children.size(); i = i + 3)
            {
                Exp *exp;
                vector<Instruction *> exp_result;
                ANALYSIS_NODE_WITH_LeftVal(exp, root, Exp, i, exp_result);
                merge(instrVec, exp_result);
                if (i == 2)
                    operand1 = ir::Operand(exp->v, exp->t);
                if (i == 5)
                    operand2 = ir::Operand(exp->v, exp->t);
            }
            if (variableSTE.operand.type == Type::IntPtr)
            {
                promaxType = Type::Int;
            }
            else if (variableSTE.operand.type == Type::FloatPtr)
            {
                promaxType = Type::Float;
            }
            string operand1Name = "a" + to_string(tmp_cnt++), operand2Name = "a" + to_string(tmp_cnt++);
            Instruction *def1Instruction = new ir::Instruction(operand1, ir::Operand(),
                                                               ir::Operand(operand1Name, Type::Int), ir::Operator::def);
            Instruction *def2Instruction = new ir::Instruction(operand2, ir::Operand(),
                                                               ir::Operand(operand2Name, Type::Int), ir::Operator::def);
            string tmp_col_len_name = "a" + to_string(tmp_cnt++);
            instrVec.push_back(def1Instruction);
            instrVec.push_back(def2Instruction);
            // a[m][n] 计算m*dim + n
            Instruction *defDimInstruction = new ir::Instruction(ir::Operand(to_string(variableSTE.dimension[1]), Type::IntLiteral),
                                                                 ir::Operand(), ir::Operand(tmp_col_len_name, Type::Int), ir::Operator::def);

            instrVec.push_back(defDimInstruction);
            string mMulDimName = "a" + to_string(tmp_cnt++);
            Instruction *mMulDimInstruction = new ir::Instruction(ir::Operand(operand1Name, Type::Int),
                                                                  ir::Operand(tmp_col_len_name, Type::Int),
                                                                  ir::Operand(mMulDimName, Type::Int), ir::Operator::mul);
            instrVec.push_back(mMulDimInstruction);
            string mMulDimAddNName = "a" + to_string(tmp_cnt++);
            Instruction *mMulDimAddNInstruction = new ir::Instruction(ir::Operand(mMulDimName, Type::Int),
                                                                      ir::Operand(operand2Name, Type::Int),
                                                                      ir::Operand(mMulDimAddNName, Type::Int), ir::Operator::add);
            instrVec.push_back(mMulDimAddNInstruction);
            string loadVariableName = "a" + to_string(tmp_cnt++);
            Instruction *loadInst = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), variableSTE.operand.type),
                                                        ir::Operand(mMulDimAddNName, Type::Int),
                                                        ir::Operand(loadVariableName, promaxType), ir::Operator::load);

            instrVec.push_back(loadInst);
            CHANGE_NODE(root, loadVariableName, promaxType);
            return instrVec;
        }
    }
    else
    {
        vector<Instruction *> instrVec;
        Term *term = dynamic_cast<Term *>(root->children[0]);
        string variableName = term->token.value;
        STE variableSTE = symbol_table.get_ste(variableName);
        string scopeVarName = symbol_table.get_scoped_name(variableName);
        Type promaxType;
        string val;
        ir::Type irType = ir::Type::Int;
        ir::Operator irOperator = ir::Operator::mov;
        // lval赋值，LVal -> Ident {'[' Exp ']'}
        if (root->children.size() == 1)
        {
            // 转成int
            if (variableSTE.operand.type == Type::Int)
            {
                if (exp->t == ir::Type::Int || exp->t == ir::Type::IntLiteral)
                {
                    val = exp->v;
                    irType = exp->t;
                }
                else if (exp->t == ir::Type::Float)
                {
                    /* cvt_f2i
                    浮点型变量转为整型变量，第一个操作数为待转换变量，结果为类型转换后变量，第二个操作数不使用。示例如下：
                    float a = 2;int b = a; => cvt_f2i b, a
                    */
                    val = exp->v;
                    irType = ir::Type::Float;
                    irOperator = ir::Operator::cvt_f2i;
                }
                else if (exp->t == ir::Type::FloatLiteral)
                {
                    int value = std::stof(exp->v);
                    val = to_string(value);
                    irType = ir::Type::IntLiteral;
                }
            }
            // 转成float
            else if (variableSTE.operand.type == Type::Float)
            {
                if (exp->t == ir::Type::Float || exp->t == ir::Type::FloatLiteral)
                {
                    val = exp->v;
                    irType = exp->t;
                }
                else if (exp->t == ir::Type::Float)
                {
                    /* cvt_f2i
                    浮点型变量转为整型变量，第一个操作数为待转换变量，结果为类型转换后变量，第二个操作数不使用。示例如下：
                    float a = 2;int b = a; => cvt_f2i b, a
                    */
                    val = exp->v;
                    irType = ir::Type::Int;
                    irOperator = ir::Operator::cvt_i2f;
                }
                else if (exp->t == ir::Type::FloatLiteral)
                {
                    float value = std::stoi(exp->v);
                    val = to_string(value);
                    irType = ir::Type::FloatLiteral;
                }
            }
            ir::Instruction *Inst = new ir::Instruction(ir::Operand(val, irType), ir::Operand(),
                                                        ir::Operand(symbol_table.get_scoped_name(variableName), variableSTE.operand.type), irOperator);
            instrVec.push_back(Inst);
        }
        // 一维数组赋值
        else if (root->children.size() == 4)
        {
            // a[n]
            // 计算n
            Exp *nExp;
            vector<Instruction *> exp_result;
            ANALYSIS_NODE_WITH_LeftVal(nExp, root, Exp, 2, exp_result);
            merge(instrVec, exp_result);
            // Int or Float变量赋值
            if (variableSTE.operand.type == Type::IntPtr)
            {
                if (exp->t == Type::Int || exp->t == Type::IntLiteral)
                {
                    val = exp->v;
                    irType = exp->t;
                }
                else if (exp->t == Type::Float)
                {
                    string tmpFloatcvtName = "a" + to_string(tmp_cnt++);
                    ir::Instruction *f2iInst = new ir::Instruction(ir::Operand(exp->v, ir::Type::Float),
                                                                   ir::Operand(),
                                                                   ir::Operand(tmpFloatcvtName, ir::Type::Int), ir::Operator::cvt_f2i);
                    instrVec.push_back(f2iInst);
                    val = tmpFloatcvtName;
                    irType = Type::Int;
                }
                else if (exp->t == Type::FloatLiteral)
                {
                    int value = std::stof(exp->v);
                    val = to_string(value);
                    irType = Type::IntLiteral;
                }
            }
            else if (variableSTE.operand.type == Type::FloatPtr)
            {
                if (exp->t == Type::Float || exp->t == Type::FloatLiteral)
                {
                    val = exp->v;
                    irType = exp->t;
                }

                else if (exp->t == Type::IntLiteral)
                {
                    float value = std::stoi(exp->v);
                    val = to_string(value);
                    irType = Type::FloatLiteral;
                }
                else if (exp->t == Type::Int)
                {
                    string tmpIntcvtName = "a" + to_string(tmp_cnt++);
                    ir::Instruction *i2fInst = new ir::Instruction(ir::Operand(exp->v, ir::Type::Int),
                                                                   ir::Operand(),
                                                                   ir::Operand(tmpIntcvtName, ir::Type::Float), ir::Operator::cvt_i2f);
                    instrVec.push_back(i2fInst);
                    val = tmpIntcvtName;
                    irType = Type::Float;
                }
            }
            ir::Instruction *storeInstruction = new ir::Instruction(ir::Operand(scopeVarName, variableSTE.operand.type),
                                                                    ir::Operand(nExp->v, nExp->t),
                                                                    ir::Operand(val, irType), ir::Operator::store);
            instrVec.push_back(storeInstruction);
        }
        // 二维数组赋值
        else if (root->children.size() == 7)
        {
            ir::Operand operand1, operand2;
            for (int i = 2; i < root->children.size(); i = i + 3)
            {
                Exp *exp;
                vector<Instruction *> exp_result;
                ANALYSIS_NODE_WITH_LeftVal(exp, root, Exp, i, exp_result);
                merge(instrVec, exp_result);
                if (i == 2)
                    operand1 = ir::Operand(exp->v, exp->t);
                if (i == 5)
                    operand2 = ir::Operand(exp->v, exp->t);
            }
            if (variableSTE.operand.type == Type::IntPtr)
            {
                promaxType = Type::Int;
            }
            else if (variableSTE.operand.type == Type::FloatPtr)
            {
                promaxType = Type::Float;
            }
            string operand1Name = "a" + to_string(tmp_cnt++), operand2Name = "a" + to_string(tmp_cnt++);
            Instruction *def1Instruction = new ir::Instruction(operand1, ir::Operand(),
                                                               ir::Operand(operand1Name, Type::Int), ir::Operator::def);
            Instruction *def2Instruction = new ir::Instruction(operand2, ir::Operand(),
                                                               ir::Operand(operand2Name, Type::Int), ir::Operator::def);
            string tmp_col_len_name = "a" + to_string(tmp_cnt++);
            instrVec.push_back(def1Instruction);
            instrVec.push_back(def2Instruction);
            // a[m][n] 计算m*dim + n
            Instruction *defDimInstruction = new ir::Instruction(ir::Operand(to_string(variableSTE.dimension[1]), Type::IntLiteral),
                                                                 ir::Operand(), ir::Operand(tmp_col_len_name, Type::Int), ir::Operator::def);

            instrVec.push_back(defDimInstruction);
            string mMulDimName = "a" + to_string(tmp_cnt++);
            Instruction *mMulDimInstruction = new ir::Instruction(ir::Operand(operand1Name, Type::Int),
                                                                  ir::Operand(tmp_col_len_name, Type::Int),
                                                                  ir::Operand(mMulDimName, Type::Int), ir::Operator::mul);
            instrVec.push_back(mMulDimInstruction);
            string mMulDimAddNName = "a" + to_string(tmp_cnt++);
            Instruction *mMulDimAddNInstruction = new ir::Instruction(ir::Operand(mMulDimName, Type::Int),
                                                                      ir::Operand(operand2Name, Type::Int),
                                                                      ir::Operand(mMulDimAddNName, Type::Int), ir::Operator::add);
            instrVec.push_back(mMulDimAddNInstruction);
            string loadVariableName = "a" + to_string(tmp_cnt++);
            Instruction *loadInst = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), variableSTE.operand.type),
                                                        ir::Operand(mMulDimAddNName, Type::Int),
                                                        ir::Operand(loadVariableName, promaxType), ir::Operator::load);

            instrVec.push_back(loadInst);
            // Int or Float变量赋值
            if (variableSTE.operand.type == Type::IntPtr)
            {
                if (exp->t == Type::Int || exp->t == Type::IntLiteral)
                {
                    ir::Instruction *storeInstruction = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::IntPtr),
                                                                            ir::Operand(mMulDimAddNName, Type::Int),
                                                                            ir::Operand(exp->v, exp->t), ir::Operator::store);
                    instrVec.push_back(storeInstruction);
                }
                else if (exp->t == Type::Float)
                {
                    string tmpFloatcvtName = "a" + to_string(tmp_cnt++);
                    ir::Instruction *f2iInst = new ir::Instruction(ir::Operand(exp->v, ir::Type::Float),
                                                                   ir::Operand(),
                                                                   ir::Operand(tmpFloatcvtName, ir::Type::Int), ir::Operator::cvt_f2i);
                    ir::Instruction *storeInstruction = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::IntPtr),
                                                                            ir::Operand(mMulDimAddNName, Type::Int),
                                                                            ir::Operand(tmpFloatcvtName, Type::Int), ir::Operator::store);
                    instrVec.push_back(f2iInst);
                    instrVec.push_back(storeInstruction);
                }
                else if (exp->t == Type::FloatLiteral)
                {
                    int val = std::stof(exp->v);
                    ir::Instruction *storeInstruction = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::IntPtr),
                                                                            ir::Operand(mMulDimAddNName, Type::Int),
                                                                            ir::Operand(to_string(val), Type::IntLiteral), ir::Operator::store);
                    instrVec.push_back(storeInstruction);
                }
            }
            else if (variableSTE.operand.type == Type::FloatPtr)
            {
                if (exp->t == Type::Int)
                {
                    string tmpIntcvtName = "t";
                    tmpIntcvtName += to_string(tmp_cnt++);
                    ir::Instruction *i2fInst = new ir::Instruction(ir::Operand(exp->v, ir::Type::Int),
                                                                   ir::Operand(),
                                                                   ir::Operand(tmpIntcvtName, ir::Type::Float), ir::Operator::cvt_i2f);
                    ir::Instruction *storeInstruction = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::FloatPtr),
                                                                            ir::Operand(mMulDimAddNName, Type::Int),
                                                                            ir::Operand(tmpIntcvtName, Type::Float), ir::Operator::store);
                    instrVec.push_back(i2fInst);
                    instrVec.push_back(storeInstruction);
                }
                else if (exp->t == Type::IntLiteral)
                {
                    float val = std::stoi(exp->v);
                    ir::Instruction *storeInstruction = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::FloatPtr),
                                                                            ir::Operand(mMulDimAddNName, Type::Int),
                                                                            ir::Operand(to_string(val), Type::FloatLiteral), ir::Operator::store);
                    instrVec.push_back(storeInstruction);
                }
                else if (exp->t == Type::Float || exp->t == Type::FloatLiteral)
                {
                    ir::Instruction *storeInstruction = new ir::Instruction(ir::Operand(symbol_table.get_scoped_name(variableName), ir::Type::FloatPtr),
                                                                            ir::Operand(mMulDimAddNName, Type::Int),
                                                                            ir::Operand(exp->v, exp->t), ir::Operator::store);
                    instrVec.push_back(storeInstruction);
                }
            }
        }
        return instrVec;
    }
}
// Number -> IntConst | floatConst
// 进制转换
int changeToint(const string &input)
{
    int ans;
    // Check if the input starts with "0x" or "0X" and contains only hexadecimal digits
    if ((input.size() > 2 && input.substr(0, 2) == "0x" || input.substr(0, 2) == "0X"))
    {
        ans = std::stoi(input.substr(2), 0, 16);
    }

    // Check if the input starts with "0b" or "0B" and contains only binary digits
    else if ((input.size() > 2 && input.substr(0, 2) == "0b" || input.substr(0, 2) == "0B"))
    {
        ans = std::stoi(input.substr(2), 0, 2);
    }
    else if (input.size() > 1 && input.substr(0, 1) == "0")
    {
        ans = std::stoi(input.substr(1), 0, 8);
    }
    else
        ans = std::stoi(input);
    return ans;
}
void frontend::Analyzer::analyzeNumber(Number *root)
{
    Term *number = dynamic_cast<Term *>(root->children[0]);
    // IntConst
    if (number->token.type == TokenType::INTLTR)
    {
        string value = number->token.value;
        // 进制转换
        string intValue = to_string(changeToint(value));
        CHANGE_NODE(root, intValue, Type::IntLiteral);
    }
    // floatConst
    else if (number->token.type == TokenType::FLOATLTR)
    {
        CHANGE_NODE(root, number->token.value, Type::FloatLiteral);
    }
}

// Cond -> LOrExp
vector<ir::Instruction *> frontend::Analyzer::analyzeCond(Cond *root)
{
    LOrExp *lorexp;
    vector<ir::Instruction *> instructions;
    ANALYSIS_NODE_WITH_LeftVal(lorexp, root, LOrExp, 0, instructions);
    COPY_EXP_NODE(lorexp, root);
    if (root->t == Type::Float || root->t == Type::FloatLiteral)
    {
        if (root->t == Type::FloatLiteral)
        {
            int val = std::stof(root->v);
            root->v = to_string(val != 0);
            root->t = Type::IntLiteral;
        }
        else
        {
            string fneqName = "a" + to_string(tmp_cnt++);
            // 与0.0比较相等
            ir::Instruction *fneqInstuction = new ir::Instruction(ir::Operand(root->v, Type::Float),
                                                                  ir::Operand("0.0", Type::FloatLiteral),
                                                                  ir::Operand(fneqName, Type::Float), ir::Operator::fneq);
            string cvt2Name = "a" + to_string(tmp_cnt++);
            ir::Instruction *cvt2Instruction = new ir::Instruction(ir::Operand(fneqName, Type::Float),
                                                                   ir::Operand(), ir::Operand(cvt2Name, Type::Int), ir::Operator::cvt_f2i);
            instructions.push_back(fneqInstuction);
            instructions.push_back(cvt2Instruction);
            CHANGE_NODE(root, cvt2Name, Type::Int);
        }
    }
    return instructions;
}

// LOrExp -> LAndExp [ '||' LOrExp ]
/*
符号[...]表示方括号内包含的为可选项；
符号{...}表示花括号内包含的为可重复 0 次或多次的项；
*/
template <typename T>
// 如果数是float,就要fneq 0.0
// 如果数是FloatLiteral,就直接比较
void frontend::Analyzer::FloatCompare(T *root, vector<ir::Instruction *> &instrVec)
{
    if (root->t == Type::Float)
    {
        string fneqName = "a" + to_string(tmp_cnt++);
        // 与0.0比较相等
        ir::Instruction *fneqInstuction = new ir::Instruction(ir::Operand(root->v, Type::Float),
                                                              ir::Operand("0.0", Type::FloatLiteral),
                                                              ir::Operand(fneqName, Type::Float), ir::Operator::fneq);
        instrVec.push_back(fneqInstuction);
        CHANGE_NODE(root, "a" + to_string(tmp_cnt++), Type::Int);
    }
    else if (root->t == Type::FloatLiteral)
    {
        float val = std::stof(root->v);
        root->t = Type::IntLiteral;
        root->v = to_string(val != 0);
    }
}
vector<ir::Instruction *> frontend::Analyzer::analyzeLOrExp(LOrExp *root)
{
    vector<ir::Instruction *> instrVec;
    // LAndExp
    LAndExp *landexp;
    vector<ir::Instruction *> instructions;
    ANALYSIS_NODE_WITH_LeftVal(landexp, root, LAndExp, 0, instructions);
    merge(instrVec, instructions);
    COPY_EXP_NODE(landexp, root);
    // [ '||' LOrExp ]
    if (root->children.size() == 1) // 无
        return instrVec;
    else
    { // '||' LOrExp
        // LOrExp
        LOrExp *lorexp;
        vector<ir::Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(lorexp, root, LOrExp, 2, instructions);

        FloatCompare(root, instrVec);
        FloatCompare(lorexp, instrVec);
        // 计算LAndExp || LOrExp
        if (root->t == Type::IntLiteral && lorexp->t == Type::IntLiteral)
            root->v = to_string(std::stoi(root->v) || std::stoi(lorexp->v));
        else
        {
            /*
            or instruction
            true goto
            false goto
            true des mov 1
            ture end goto
            false des
            */
            // 初始为0，为真赋值1 ，这是由or的特性决定的
            string orName = "0";
            // 计算or
            Instruction *computeInstruction = new Instruction(ir::Operand(root->v, root->t),
                                                              ir::Operand(lorexp->v, lorexp->t),
                                                              ir::Operand(orName, ir::Type::Int), ir::Operator::_or);
            instructions.push_back(computeInstruction);
            // 真
            Instruction *trueGotoInstruction = new Instruction(ir::Operand(root->v, root->t), ir::Operand(),
                                                               ir::Operand("2", Type::IntLiteral), ir::Operator::_goto);
            // 假
            Instruction *falseGotoInstruction = new Instruction(ir::Operand(), ir::Operand(),
                                                                ir::Operand("3", Type::IntLiteral), ir::Operator::_goto);
            instrVec.push_back(trueGotoInstruction);
            instrVec.push_back(falseGotoInstruction);
            // true :orName赋为1,有一个真结果为真
            Instruction *root_true_assign = new Instruction(ir::Operand("1", Type::IntLiteral), ir::Operand(),
                                                            ir::Operand(orName, ir::Type::Int), ir::Operator::mov);
            instrVec.push_back(root_true_assign);
            // true :跳转至结束
            Instruction *true_logic_goto = new Instruction(ir::Operand(), ir::Operand(),
                                                           ir::Operand(to_string(instructions.size() + 1), Type::IntLiteral), ir::Operator::_goto);
            instrVec.push_back(true_logic_goto);

            merge(instrVec, instructions);
            CHANGE_NODE(root, orName, Type::Int);
        }
        return instrVec;
    }
}

// LAndExp -> EqExp [ '&&' LAndExp ]
vector<ir::Instruction *> frontend::Analyzer::analyzeLAndExp(LAndExp *root)
{
    vector<ir::Instruction *> instrVec;
    EqExp *eqexp;
    vector<ir::Instruction *> instructions;
    ANALYSIS_NODE_WITH_LeftVal(eqexp, root, EqExp, 0, instructions);
    merge(instrVec, instructions);
    COPY_EXP_NODE(eqexp, root);
    // [ '&&' LAndExp ]
    if (root->children.size() == 1) // 无'&&' LAndExp
        return instrVec;
    { // 有'&&' LAndExp
        LAndExp *landexp;
        vector<ir::Instruction *> instructions; // 并不立即加入到result中，在确定跳转后再加入
        ANALYSIS_NODE_WITH_LeftVal(landexp, root, LAndExp, 2, instructions);
        FloatCompare(root, instrVec);
        FloatCompare(landexp, instrVec);
        // 计算EqExp && LAndExp
        if (root->t == Type::IntLiteral && landexp->t == Type::IntLiteral)
            root->v = to_string(std::stoi(root->v) && std::stoi(landexp->v));
        else
        {
            /*
            and instruction
            true goto
            false goto
            false des mov 1
            false end goto
            true des
            */
            // 初始为1，为假赋值0 ，这是由and的特性决定的
            string andName = "1";
            // 计算or
            Instruction *computeInstruction = new Instruction(ir::Operand(root->v, root->t),
                                                              ir::Operand(landexp->v, landexp->t),
                                                              ir::Operand(andName, ir::Type::Int), ir::Operator::_and);
            instructions.push_back(computeInstruction);
            // 真
            Instruction *trueGotoInstruction = new Instruction(ir::Operand(root->v, root->t), ir::Operand(),
                                                               ir::Operand("4", Type::IntLiteral), ir::Operator::_goto);

            // 假
            Instruction *falseGotoInstruction = new Instruction(ir::Operand(), ir::Operand(),
                                                                ir::Operand("1", Type::IntLiteral), ir::Operator::_goto);
            instrVec.push_back(trueGotoInstruction);
            instrVec.push_back(falseGotoInstruction);
            // false :andName赋为0,有一个假结果为假
            Instruction *root_true_assign = new Instruction(ir::Operand("0", Type::IntLiteral), ir::Operand(),
                                                            ir::Operand(andName, ir::Type::Int), ir::Operator::mov);
            instrVec.push_back(root_true_assign);
            // false :跳转至结束
            Instruction *true_logic_goto = new Instruction(ir::Operand(), ir::Operand(),
                                                           ir::Operand(to_string(instructions.size() + 1), Type::IntLiteral), ir::Operator::_goto);
            instrVec.push_back(true_logic_goto);

            merge(instrVec, instructions);
            CHANGE_NODE(root, andName, Type::Int);
        }
        return instrVec;
    }
}

// EqExp -> RelExp { ('==' | '!=') RelExp }
vector<ir::Instruction *> frontend::Analyzer::analyzeEqExp(EqExp *root)
{
    vector<Instruction *> instrVec;
    for (int i = 0; i < root->children.size(); i += 2)
    {
        RelExp *relexp;
        vector<Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(relexp, root, RelExp, i, instructions);
        merge(instrVec, instructions);
    }
    RelExp *firstRelExp = dynamic_cast<RelExp *>(root->children[0]);
    COPY_EXP_NODE(firstRelExp, root);
    // RelExp
    if (root->children.size() == 1)
        return instrVec;
    Type promaxType = root->t;
    for (unsigned int i = 2; i < root->children.size(); i += 2)
    {
        RelExp *relexp = dynamic_cast<RelExp *>(root->children[i]);

        // 记录最高优先级的变量类型
        if ((promaxType == Type::FloatLiteral && relexp->t == Type::Int) || (promaxType == Type::Int && relexp->t == Type::FloatLiteral))
        {
            promaxType = Type::Float;
        }
        else if (getPriority(relexp->t) > getPriority(promaxType))
        {
            promaxType = relexp->t;
        }
    }
    // 执行类型转换 todo
    for (unsigned int i = 2; i < root->children.size(); i += 2)
    {
        RelExp *relexp = dynamic_cast<RelExp *>(root->children[i]);
        if (promaxType == Type::Int)
        { // IntLiteral -> Int
            // 可能需要修改的节点有两个，addexp,和root
            if (relexp->t == Type::IntLiteral)
            {
                string tmpIntcvtName = "a" + to_string(tmp_cnt++);
                Instruction *cvtInst = new Instruction(ir::Operand(relexp->v, ir::Type::IntLiteral), ir::Operand(),
                                                       ir::Operand(tmpIntcvtName, ir::Type::Int), ir::Operator::def);
                instrVec.push_back(cvtInst);
                CHANGE_NODE(relexp, tmpIntcvtName, Type::Int);
            }
            if (root->t == Type::IntLiteral)
            {
                string tmpIntcvtName = "a" + to_string(tmp_cnt++);
                Instruction *cvtInst = new Instruction(ir::Operand(root->v, ir::Type::IntLiteral), ir::Operand(),
                                                       ir::Operand(tmpIntcvtName, ir::Type::Int), ir::Operator::def);
                instrVec.push_back(cvtInst);
                CHANGE_NODE(root, tmpIntcvtName, Type::Int);
            }
        }
        else if (promaxType == Type::FloatLiteral)
        { // IntLiteral -> FloatLiteral
            if (relexp->t == Type::IntLiteral)
            {
                float value = std::stoi(relexp->v);
                string val = to_string(value);
                CHANGE_NODE(relexp, val, Type::FloatLiteral);
            }
            if (root->t == Type::IntLiteral)
            {
                float value = std::stoi(root->v);
                string val = to_string(value);
                CHANGE_NODE(root, val, Type::FloatLiteral);
            }
        }
        else if (promaxType == Type::Float)
        { // IntLiteral -> Float, Int -> Float, FloatLiteral -> Float
            string tmpIntcvtName;
            bool change = false;
            string val;
            ir::Type irType;
            ir::Operator irOperator;
            if (relexp->t == Type::IntLiteral)
            { // IntLiteral -> Float
                float value = std::stof(relexp->v);
                val = to_string(value);
                irType = ir::Type::FloatLiteral;
                irOperator = ir::Operator::fdef;
                change = true;
            }
            else if (relexp->t == Type::Int)
            {

                val = relexp->v;
                irType = ir::Type::Int;
                irOperator = ir::Operator::cvt_i2f;
                change = true;
            }
            else if (relexp->t == Type::FloatLiteral)
            {
                val = relexp->v;
                irType = ir::Type::FloatLiteral;
                irOperator = ir::Operator::fdef;
                change = true;
            }
            if (change)
            {
                tmpIntcvtName = "a" + to_string(tmp_cnt++);
                Instruction *cvtInst = new Instruction(ir::Operand(val, irType), ir::Operand(),
                                                       ir::Operand(tmpIntcvtName, ir::Type::Float), irOperator);
                instrVec.push_back(cvtInst);
                CHANGE_NODE(relexp, tmpIntcvtName, Type::Float);
            }
            change = false;
            if (root->t == Type::IntLiteral)
            { // IntLiteral -> Float
                float value = std::stof(root->v);
                val = to_string(value);
                irType = ir::Type::FloatLiteral;
                irOperator = ir::Operator::fdef;
                change = true;
            }
            else if (root->t == Type::Int)
            {
                val = root->v;
                irType = ir::Type::Int;
                irOperator = ir::Operator::cvt_i2f;
                change = true;
            }
            else if (root->t == Type::FloatLiteral)
            {
                val = root->v;
                irType = ir::Type::FloatLiteral;
                irOperator = ir::Operator::fdef;
                change = true;
            }
            if (change)
            {
                tmpIntcvtName = "a" + to_string(tmp_cnt++);
                Instruction *cvtInst = new Instruction(ir::Operand(val, irType), ir::Operand(),
                                                       ir::Operand(tmpIntcvtName, ir::Type::Float), irOperator);
                instrVec.push_back(cvtInst);
                CHANGE_NODE(root, tmpIntcvtName, Type::Float);
            }
        }
    }
    for (int i = 2; i < root->children.size(); i += 2)
    {
        RelExp *relexp = dynamic_cast<RelExp *>(root->children[i]);
        Term *curOperator = dynamic_cast<Term *>(root->children[i - 1]);
        // 已经化为相同类型，可以开始计算
        if (promaxType == Type::IntLiteral || promaxType == Type::FloatLiteral)
        {
            float value1 = std::stof(root->v);
            float value2 = std::stof(relexp->v);
            int val;
            if (curOperator->token.type == TokenType::EQL)
                val = value1 == value2;
            else if (curOperator->token.type == TokenType::NEQ)
                val = value1 != value2;
            CHANGE_NODE(root, to_string(val), promaxType);
        }
        else if (promaxType == Type::Int || promaxType == Type::Float)
        {
            string coumputeName = "a" + to_string(tmp_cnt++);
            Instruction *computeInstruction;
            ir::Operator irOperator;
            if (curOperator->token.type == TokenType::EQL)
            {
                irOperator = promaxType == Type::Int ? ir::Operator::eq : ir::Operator::feq;
                computeInstruction = new Instruction(ir::Operand(root->v, promaxType),
                                                     ir::Operand(relexp->v, promaxType),
                                                     ir::Operand(coumputeName, promaxType), irOperator);
                instrVec.push_back(computeInstruction);
                CHANGE_NODE(root, coumputeName, promaxType);
            }

            else if (curOperator->token.type == TokenType::NEQ)
            {
                irOperator = promaxType == Type::Int ? ir::Operator::neq : ir::Operator::fneq;
                computeInstruction = new Instruction(ir::Operand(root->v, promaxType),
                                                     ir::Operand(relexp->v, promaxType),
                                                     ir::Operand(coumputeName, promaxType), irOperator);
                instrVec.push_back(computeInstruction);
                CHANGE_NODE(root, coumputeName, promaxType);
            }
        }
    }
    return instrVec;
}

// RelExp -> AddExp { ('<' | '>' | '<=' | '>=') AddExp }
vector<ir::Instruction *> frontend::Analyzer::analyzeRelExp(RelExp *root)
{
    vector<Instruction *> instrVec;
    for (int i = 0; i < root->children.size(); i += 2)
    {
        AddExp *addexp;
        vector<Instruction *> instructions;
        ANALYSIS_NODE_WITH_LeftVal(addexp, root, AddExp, i, instructions);
        merge(instrVec, instructions);
    }
    AddExp *firstAddExp = dynamic_cast<AddExp *>(root->children[0]);
    COPY_EXP_NODE(firstAddExp, root);
    // AddExp
    if (root->children.size() == 1)
        return instrVec;
    Type promaxType = root->t;
    // { ('<' | '>' | '<=' | '>=') AddExp }
    for (unsigned int i = 2; i < root->children.size(); i += 2)
    {
        AddExp *addexp = dynamic_cast<AddExp *>(root->children[i]);

        // 记录最高优先级的变量类型
        if ((promaxType == Type::FloatLiteral && addexp->t == Type::Int) || (promaxType == Type::Int && addexp->t == Type::FloatLiteral))
        {
            promaxType = Type::Float;
        }
        else if (getPriority(addexp->t) > getPriority(promaxType))
        {
            promaxType = addexp->t;
        }
    }
    // 执行类型转换 todo
    for (unsigned int i = 2; i < root->children.size(); i += 2)
    {
        AddExp *addexp = dynamic_cast<AddExp *>(root->children[i]);
        if (promaxType == Type::Int)
        { // IntLiteral -> Int
            // 可能需要修改的节点有两个，addexp,和root
            if (addexp->t == Type::IntLiteral)
            {
                string tmpIntcvtName = "a" + to_string(tmp_cnt++);
                Instruction *cvtInst = new Instruction(ir::Operand(addexp->v, ir::Type::IntLiteral), ir::Operand(),
                                                       ir::Operand(tmpIntcvtName, ir::Type::Int), ir::Operator::def);
                instrVec.push_back(cvtInst);
                CHANGE_NODE(addexp, tmpIntcvtName, Type::Int);
            }
            if (root->t == Type::IntLiteral)
            {
                string tmpIntcvtName = "a" + to_string(tmp_cnt++);
                Instruction *cvtInst = new Instruction(ir::Operand(root->v, ir::Type::IntLiteral), ir::Operand(),
                                                       ir::Operand(tmpIntcvtName, ir::Type::Int), ir::Operator::def);
                instrVec.push_back(cvtInst);
                CHANGE_NODE(root, tmpIntcvtName, Type::Int);
            }
        }
        else if (promaxType == Type::FloatLiteral)
        { // IntLiteral -> FloatLiteral
            if (addexp->t == Type::IntLiteral)
            {
                float value = std::stoi(addexp->v);
                string val = to_string(value);
                CHANGE_NODE(addexp, val, Type::FloatLiteral);
            }
            if (root->t == Type::IntLiteral)
            {
                float value = std::stoi(root->v);
                string val = to_string(value);
                CHANGE_NODE(root, val, Type::FloatLiteral);
            }
        }
        else if (promaxType == Type::Float)
        { // IntLiteral -> Float, Int -> Float, FloatLiteral -> Float
            string tmpIntcvtName;
            bool change = false;
            string val;
            ir::Type irType;
            ir::Operator irOperator;
            if (addexp->t == Type::IntLiteral)
            { // IntLiteral -> Float
                float value = std::stof(addexp->v);
                val = to_string(value);
                irType = ir::Type::FloatLiteral;
                irOperator = ir::Operator::fdef;
                change = true;
            }
            else if (addexp->t == Type::Int)
            {

                val = addexp->v;
                irType = ir::Type::Int;
                irOperator = ir::Operator::cvt_i2f;
                change = true;
            }
            else if (addexp->t == Type::FloatLiteral)
            {
                val = addexp->v;
                irType = ir::Type::FloatLiteral;
                irOperator = ir::Operator::fdef;
                change = true;
            }
            if (change)
            {
                tmpIntcvtName = "a" + to_string(tmp_cnt++);
                Instruction *cvtInst = new Instruction(ir::Operand(val, irType), ir::Operand(),
                                                       ir::Operand(tmpIntcvtName, ir::Type::Float), irOperator);
                instrVec.push_back(cvtInst);
                CHANGE_NODE(addexp, tmpIntcvtName, Type::Float);
            }
            change = false;
            if (root->t == Type::IntLiteral)
            { // IntLiteral -> Float
                float value = std::stof(root->v);
                val = to_string(value);
                irType = ir::Type::FloatLiteral;
                irOperator = ir::Operator::fdef;
                change = true;
            }
            else if (root->t == Type::Int)
            {
                val = root->v;
                irType = ir::Type::Int;
                irOperator = ir::Operator::cvt_i2f;
                change = true;
            }
            else if (root->t == Type::FloatLiteral)
            {
                val = root->v;
                irType = ir::Type::FloatLiteral;
                irOperator = ir::Operator::fdef;
                change = true;
            }
            if (change)
            {
                tmpIntcvtName = "a" + to_string(tmp_cnt++);
                Instruction *cvtInst = new Instruction(ir::Operand(val, irType), ir::Operand(),
                                                       ir::Operand(tmpIntcvtName, ir::Type::Float), irOperator);
                instrVec.push_back(cvtInst);
                CHANGE_NODE(root, tmpIntcvtName, Type::Float);
            }
        }
    }
    for (unsigned int i = 2; i < root->children.size(); i += 2)
    {
        AddExp *addexp = dynamic_cast<AddExp *>(root->children[i]);
        Term *curOperator = dynamic_cast<Term *>(root->children[i - 1]);
        if (promaxType == Type::IntLiteral || promaxType == Type::FloatLiteral)
        {
            float value1 = std::stof(root->v);
            float value2 = std::stof(addexp->v);
            int val;
            if (curOperator->token.type == TokenType::LSS)
                val = value1 < value2;
            else if (curOperator->token.type == TokenType::GTR)
                val = value1 > value2;
            else if (curOperator->token.type == TokenType::LEQ)
                val = value1 <= value2;
            else if (curOperator->token.type == TokenType::GEQ)
                val = value1 >= value2;
            CHANGE_NODE(root, to_string(val), promaxType);
        }
        else if (promaxType == Type::Int || promaxType == Type::Float)
        {
            string coumputeName = "a" + to_string(tmp_cnt++);
            Instruction *computeInstruction;
            ir::Operator irOperator = ir::Operator::lss;
            if (curOperator->token.type == TokenType::LSS)
                irOperator = promaxType == Type::Int ? ir::Operator::lss : ir::Operator::flss;
            else if (curOperator->token.type == TokenType::GTR)
                irOperator = promaxType == Type::Int ? ir::Operator::gtr : ir::Operator::fgtr;
            else if (curOperator->token.type == TokenType::LEQ)
                irOperator = promaxType == Type::Int ? ir::Operator::leq : ir::Operator::fleq;
            else if (curOperator->token.type == TokenType::GEQ)
                irOperator = promaxType == Type::Int ? ir::Operator::geq : ir::Operator::fgeq;
            computeInstruction = new Instruction(ir::Operand(root->v, promaxType),
                                                 ir::Operand(addexp->v, promaxType),
                                                 ir::Operand(coumputeName, promaxType), irOperator);
            instrVec.push_back(computeInstruction);
            CHANGE_NODE(root, coumputeName, promaxType);
        }
    }
    return instrVec;
}
