#ifndef AST_H
#define AST_H

#include"front/token.h"
#include"json/json.h"
#include"ir/ir.h"
using ir::Type;

#include<set>
#include<vector>
#include<string>
using std::vector;
using std::string;

namespace frontend {

// enumerate for node type
enum class NodeType {
    TERMINAL,       // terminal lexical unit
    COMPUINT,
    DECL,
    FUNCDEF,
    CONSTDECL,
    BTYPE,
    CONSTDEF,
    CONSTINITVAL,
    VARDECL,
    VARDEF,
    INITVAL,
    FUNCTYPE,
    FUNCFPARAM,
    FUNCFPARAMS,
    BLOCK,
    BLOCKITEM,
    STMT,
    EXP,
    COND,
    LVAL,
    NUMBER,
    PRIMARYEXP,
    UNARYEXP,
    UNARYOP,
    FUNCRPARAMS,
    MULEXP,
    ADDEXP,
    RELEXP,
    EQEXP,
    LANDEXP,
    LOREXP,
    CONSTEXP,
};
std::string toString(NodeType);

// tree node basic class
struct AstNode{
    NodeType type;  // the node type
    AstNode* parent;    // the parent node
    vector<AstNode*> children;     // children of node

    AstNode(NodeType t, AstNode* p = nullptr);

    /**
     * @brief destructor
     */
    virtual ~AstNode();

    /**
     * @brief Get the json output object
     * @param root: a Json::Value buffer, should be initialized before calling this function
     */
    void get_json_output(Json::Value& root) const;

    // rejcet copy and assignment
    AstNode(const AstNode&) = delete;
    AstNode& operator=(const AstNode&) = delete;
};

struct Term: AstNode {
    Token token;
    Term(Token t, AstNode* p = nullptr);
};


struct CompUnit: AstNode {

    CompUnit(AstNode* p = nullptr);
};

struct Decl: AstNode{

    Decl(AstNode* p = nullptr);
};

struct FuncDef: AstNode{
    string n;
    Type t;
    

    FuncDef(AstNode* p = nullptr);
};

struct ConstDecl: AstNode {
    Type t;


    ConstDecl(AstNode* p = nullptr);        
};

struct BType: AstNode {
    Type t;


    BType(AstNode* p = nullptr);
};

struct ConstDef: AstNode{
    std::string arr_name;


    ConstDef(AstNode* p = nullptr);
};

struct ConstInitVal: AstNode{
    string v;
    Type t;


    ConstInitVal(AstNode* p = nullptr);
};

struct VarDecl: AstNode{
    Type t;


    VarDecl(AstNode* p = nullptr);
};

struct VarDef: AstNode{
    std::string arr_name;


    VarDef(AstNode* p = nullptr);
};

struct InitVal: AstNode{
    bool is_computable = false;
    string v;
    Type t;


    InitVal(AstNode* p = nullptr);
};

struct FuncType: AstNode{

    FuncType(AstNode* p = nullptr);
};

struct FuncFParam: AstNode{

    FuncFParam(AstNode* p = nullptr);
};

struct FuncFParams: AstNode{

    FuncFParams(AstNode* p = nullptr);
};

struct Block: AstNode{

    Block(AstNode* p = nullptr);
};

struct BlockItem: AstNode{

    BlockItem(AstNode* p = nullptr);
};

struct Stmt: AstNode{
    // for while & break & continue, we need a vector to remember break & continue instruction
    std::set<ir::Instruction*> jump_eow;  // jump to end of while
    std::set<ir::Instruction*> jump_bow;  // jump to begin of while


    Stmt(AstNode* p = nullptr);
};

struct Exp: AstNode{
    bool is_computable = false;
    string v;
    Type t;


    Exp(AstNode* p = nullptr);
};

struct Cond: AstNode{
    bool is_computable = false;
    string v;
    Type t;


    Cond(AstNode* p = nullptr);
};

struct LVal: AstNode{
    bool is_computable = false;
    string v;
    Type t;
    int i;  // array index, legal if t is IntPtr or FloatPtr


    LVal(AstNode* p = nullptr);
};

struct Number: AstNode{
    bool is_computable = true;
    string v;
    Type t;


    Number(AstNode* p = nullptr);
};

struct PrimaryExp: AstNode{
    bool is_computable = false;
    string v;
    Type t;
    

    PrimaryExp(AstNode* p = nullptr);
};

struct UnaryExp: AstNode{
    bool is_computable = false;
    string v;
    Type t;


    UnaryExp(AstNode* p = nullptr);
};

struct UnaryOp: AstNode{
    TokenType op;
    

    UnaryOp(AstNode* p = nullptr);
};

struct FuncRParams: AstNode{

    FuncRParams(AstNode* p = nullptr);
};

struct MulExp: AstNode{
    bool is_computable = false;
    string v;
    Type t;


    MulExp(AstNode* p = nullptr);
};

struct AddExp: AstNode{
    bool is_computable = false;
    string v;
    Type t;


    AddExp(AstNode* p = nullptr);
};

struct RelExp: AstNode{
    bool is_computable = false;
    string v;
    Type t = Type::Int;


    RelExp(AstNode* p = nullptr);
};

struct EqExp: AstNode{
    bool is_computable = false;
    string v;
    Type t = Type::Int;


    EqExp(AstNode* p = nullptr);
};

struct LAndExp: AstNode{
    bool is_computable = false;
    string v;
    Type t = Type::Int;


    LAndExp(AstNode* p = nullptr);
};

struct LOrExp: AstNode{
    bool is_computable = false;
    string v;
    Type t = Type::Int;


    LOrExp(AstNode* p = nullptr);
};

struct ConstExp: AstNode{
    bool is_computable = true;
    string v;
    Type t ;


    ConstExp(AstNode* p = nullptr);
};
    
} // namespace frontend

#endif