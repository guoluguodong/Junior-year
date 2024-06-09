#ifndef SEMANTIC_H
#define SEMANTIC_H

#include"ir/ir.h"
#include"front/abstract_syntax_tree.h"

#include<map>
#include<string>
#include<vector>
using std::map;
using std::string;
using std::vector;
using namespace std;
namespace frontend
{

// definition of symbol table entry符号表入口
struct STE {
    /*
    struct Operand {
    std::string name;变量名
    Type type;变量类型};
    */
    ir::Operand operand;
    vector<int> dimension;//数组操作数
    string literalVal;  // 把立即数值存下来，变量名直接查
};

using map_str_ste = map<string, STE>;
// definition of scope infomation
// 作用域，作用范围
struct ScopeInfo {
    int cnt;    // 编号
    string name;// 作用域的名称
    Block* block;// 作用域入口根节点,block为一个作用域整体
    map_str_ste table;// table将作用域映射到符号表入口
    
};

// surpport lib functions
map<std::string,ir::Function*>* get_lib_funcs();

// definition of symbol table符号表，存在Analyzer中
struct SymbolTable{
    // 全局一张符号表，管理所有作用域
    vector<ScopeInfo> scope_stack;
    // 函数，全局变量
    map<std::string,ir::Function*> functions;
    // 记录递增的作用域序号,每个作用域ScopeInfo的name需要编号，从这里获取
    //ScopeInfo.cnt;    // 编号
    int blockId=0;
    /**
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
        全局作用域是CompUnit *root
     */
    void add_scope();

    /**
     * @brief exit a scope, pop out infomations
     * 退出范围，弹出信息
     */
    void exit_scope();

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
    string get_scoped_name(string id) const;

    /**
     * @brief get the right operand with the input name：
     * 使用输入名称获取正确的operand
     * @param id identifier name
     * @return Operand 
     */
    ir::Operand get_operand(string id) const;

    /**
     * @brief get the right ste with the input name：
     * 使用输入的名称获得正确的ste
     * @param id identifier name
     * @return STE 符号表入口
     */
    
    STE get_ste(string id) const;
};


// singleton class 单例类
struct Analyzer {
    int tmp_cnt;
    vector<ir::Instruction*> g_init_inst; // 全局变量初始化指令队列
    SymbolTable symbol_table;           // 全局符号表

    /**
     * @brief constructor
     */
    Analyzer();

    // analysis functions，
    /**
     * @brief 该接口接受一个源程序语法树的根节点 Comp*，对其进行分析，
            返回分析结果 ir::Program
     */
    ir::Program get_ir_program(CompUnit*);

    // reject copy & assignment，拒绝复制构造和赋值
    Analyzer(const Analyzer&) = delete;
    Analyzer& operator=(const Analyzer&) = delete;

    ir::Program irProgram;                 // 程序
    ir::Function* curFunctionPtr = nullptr;  // 当前函数指针

// ====================================================================
 // 辅助函数，分析表达式的结构，返回中间生成的IR指令
    void analyzeCompUnit(CompUnit*);
    vector<ir::Instruction *> analyzeDecl(Decl*);
    void analyzeFuncDef(FuncDef*);
    ir::Type analyzeFuncType(FuncType*);
    ir::Operand analyzeFuncFParam(FuncFParam *root);
    vector<ir::Operand> analyzeFuncFParams(FuncFParams*);
    vector<ir::Instruction *> analyzeFuncRParams(FuncRParams*, vector<ir::Operand> &, vector<ir::Operand> &);  // 分析函数返回参数列表还需要返回IR指令，也需要函数传入参数列表
    ir::Type analyzeBType(BType*);
    vector<ir::Instruction *> analyzeBlock(Block*);
    vector<ir::Instruction *> analyzeBlockItem(BlockItem*);
    vector<ir::Instruction *> analyzeStmt(Stmt*);
    vector<ir::Instruction *> analyzeConstDecl(ConstDecl*);
    vector<ir::Instruction *> analyzeConstDef(ConstDef*, ir::Type);
    vector<ir::Instruction *> analyzeConstInitVal(ConstInitVal*, ir::Type, int, string);
    vector<ir::Instruction *> analyzeVarDecl(VarDecl*);
    vector<ir::Instruction *> analyzeVarDef(VarDef*, ir::Type);
    vector<ir::Instruction *> analyzeInitVal(InitVal*, ir::Type, int, string);
    
    // 辅助函数，计算表达式的值，结果写入v，类型写入t，返回中间生成的IR指令
    vector<ir::Instruction *> analyzeExp(Exp*);
    vector<ir::Instruction *> analyzeConstExp(ConstExp*);    // 常数计算不生成指令，因此返回空数组
    vector<ir::Instruction *> analyzeAddExp(AddExp*);
    template <typename T>
    vector<ir::Instruction *> ChangeType(Type promaxType, T *root);
    vector<ir::Instruction *> analyzeMulExp(MulExp*);
    vector<ir::Instruction *> analyzeUnaryExp(UnaryExp*);
    vector<ir::Instruction *> analyzePrimaryExp(PrimaryExp*);
    vector<ir::Instruction *> analyzeLval(LVal *root,bool isStore,Exp *exp);
    void analyzeNumber(Number*);        // 常数计算不生成指令，因此返回空数组
    vector<ir::Instruction *> analyzeCond(Cond*);
    template <typename T>
    void FloatCompare(T* root,vector<ir::Instruction *> & instrVec);
    vector<ir::Instruction *> analyzeLOrExp(LOrExp*);
    vector<ir::Instruction *> analyzeLAndExp(LAndExp*);
    vector<ir::Instruction *> analyzeEqExp(EqExp*);
    vector<ir::Instruction *> analyzeRelExp(RelExp*);

    // 辅助函数，在计算表达式的值的时候进行类型转换
    void IntLiteral2Int(AstNode*, AstNode*, frontend::NodeType, vector<ir::Instruction *> &);
    void IntLiteral2FloatLiteral(AstNode*, AstNode*, frontend::NodeType);
    void IntLiteral2Float(AstNode*, AstNode*, frontend::NodeType, vector<ir::Instruction *> &);
    void Int2Float(AstNode*, AstNode*, frontend::NodeType, vector<ir::Instruction *> &);
    void FloatLiteral2Float(AstNode*, AstNode*, frontend::NodeType, vector<ir::Instruction *> &);
};

} // namespace frontend

#endif