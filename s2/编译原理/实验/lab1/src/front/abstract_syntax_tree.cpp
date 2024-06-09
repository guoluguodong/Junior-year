#include "front/abstract_syntax_tree.h"
#include "ir/ir.h"

#include <cassert>

using frontend::AddExp;
using frontend::AstNode;
using frontend::Block;
using frontend::BlockItem;
using frontend::BType;
using frontend::CompUnit;
using frontend::Cond;
using frontend::ConstDecl;
using frontend::ConstDef;
using frontend::ConstExp;
using frontend::ConstInitVal;
using frontend::Decl;
using frontend::EqExp;
using frontend::Exp;
using frontend::FuncDef;
using frontend::FuncFParam;
using frontend::FuncFParams;
using frontend::FuncRParams;
using frontend::FuncType;
using frontend::InitVal;
using frontend::LAndExp;
using frontend::LOrExp;
using frontend::LVal;
using frontend::MulExp;
using frontend::Number;
using frontend::PrimaryExp;
using frontend::RelExp;
using frontend::Stmt;
using frontend::Term;
using frontend::UnaryExp;
using frontend::UnaryOp;
using frontend::VarDecl;
using frontend::VarDef;

AstNode::AstNode(NodeType t, AstNode *p) 
{
    // 提供的函数没有将 p->children.push_back(this)
    this->parent = p;
    this->type = t;
    // 抽象树节点AstNode
    if (p != nullptr)
        p->children.push_back(this);
}

AstNode::~AstNode()
{
    for (auto it : children)
        delete it;
}

void AstNode::get_json_output(Json::Value &root) const
{
    root["name"] = toString(type);
    if (type == NodeType::TERMINAL)
    {
        auto termP = dynamic_cast<Term *>(const_cast<AstNode *>(this));
        assert(termP);
        root["type"] = toString(termP->token.type);
        root["value"] = termP->token.value;
    }
    else
    {
        root["subtree"] = Json::Value();
        for (const auto &node : children)
        {
            Json::Value tmp;
            node->get_json_output(tmp);
            root["subtree"].append(tmp);
        }
    }
}

std::string frontend::toString(NodeType nodeType)
{
    switch (nodeType)
    {
    case NodeType::TERMINAL:
        return "Terminal";
    case NodeType::COMPUINT:
        return "CompUnit";
    case NodeType::DECL:
        return "Decl";
    case NodeType::FUNCDEF:
        return "FuncDef";
    case NodeType::CONSTDECL:
        return "ConstDecl";
    case NodeType::BTYPE:
        return "BType";
    case NodeType::CONSTDEF:
        return "ConstDef";
    case NodeType::CONSTINITVAL:
        return "ConstInitVal";
    case NodeType::VARDECL:
        return "VarDecl";
    case NodeType::VARDEF:
        return "VarDef";
    case NodeType::INITVAL:
        return "InitVal";
    case NodeType::FUNCTYPE:
        return "FuncType";
    case NodeType::FUNCFPARAM:
        return "FuncFParam";
    case NodeType::FUNCFPARAMS:
        return "FuncFParams";
    case NodeType::BLOCK:
        return "Block";
    case NodeType::BLOCKITEM:
        return "BlockItem";
    case NodeType::STMT:
        return "Stmt";
    case NodeType::EXP:
        return "Exp";
    case NodeType::COND:
        return "Cond";
    case NodeType::LVAL:
        return "LVal";
    case NodeType::NUMBER:
        return "Number";
    case NodeType::PRIMARYEXP:
        return "PrimaryExp";
    case NodeType::UNARYEXP:
        return "UnaryExp";
    case NodeType::UNARYOP:
        return "UnaryOp";
    case NodeType::FUNCRPARAMS:
        return "FuncRParams";
    case NodeType::MULEXP:
        return "MulExp";
    case NodeType::ADDEXP:
        return "AddExp";
    case NodeType::RELEXP:
        return "RelExp";
    case NodeType::EQEXP:
        return "EqExp";
    case NodeType::LANDEXP:
        return "LAndExp";
    case NodeType::LOREXP:
        return "LOrExp";
    case NodeType::CONSTEXP:
        return "ConstExp";
    default:
        assert(0 && "invalid node type");
        break;
    }
    return "";
}
/*消除了左递归的文法规则：
CompUnit -> (Decl | FuncDef) [CompUnit]
Decl -> ConstDecl | VarDecl
ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'
BType -> 'int' | 'float'
ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
VarDecl -> BType VarDef { ',' VarDef } ';'
VarDef -> Ident { '[' ConstExp ']' } [ '=' InitVal ]
InitVal -> Exp | '{' [ InitVal { ',' InitVal } ] '}'
FuncDef -> FuncType Ident '(' [FuncFParams] ')' Block
FuncType -> 'void' | 'int' | 'float'
FuncFParam -> BType Ident ['[' ']' { '[' Exp ']' }]
FuncFParams -> FuncFParam { ',' FuncFParam }
Block -> '{' { BlockItem } '}'
BlockItem -> Decl | Stmt
Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | [Exp] ';'
Exp -> AddExp
Cond -> LOrExp
LVal -> Ident {'[' Exp ']'}
Number -> IntConst | floatConst
PrimaryExp -> '(' Exp ')' | LVal | Number
UnaryExp -> PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp
UnaryOp -> '+' | '-' | '!'
FuncRParams -> Exp { ',' Exp }
MulExp -> UnaryExp { ('*' | '/' | '%') UnaryExp }
AddExp -> MulExp { ('+' | '-') MulExp }
RelExp -> AddExp { ('<' | '>' | '<=' | '>=') AddExp }
EqExp -> RelExp { ('==' | '!=') RelExp }
LAndExp -> EqExp [ '&&' LAndExp ]
LOrExp -> LAndExp [ '||' LOrExp ]
ConstExp -> AddExp
*/

// 依次实现abstract_syntax_tree.h的struct
// 终结符
// 表示语法树中的终结符节点，例如标识符、数字等
Term::Term(Token t, AstNode *p) : AstNode(NodeType::TERMINAL, p), token(t) {}

// 翻译单位
// 表示整个翻译单元的语法树节点
CompUnit::CompUnit(AstNode *p) : AstNode(NodeType::COMPUINT, p) {}

// 声明
// 表示变量或函数声明的语法树节点
Decl::Decl(AstNode *p) : AstNode(NodeType::DECL, p) {}

// 函数定义
// 表示函数定义的语法树节点
FuncDef::FuncDef(AstNode *p) : AstNode(NodeType::FUNCDEF, p) {}

// 常量声明
// 表示常量声明的语法树节点
ConstDecl::ConstDecl(AstNode *p) : AstNode(NodeType::CONSTDECL, p) {}

// 基本类型
// 表示变量或常量的基本类型的语法树节点
BType::BType(AstNode *p) : AstNode(NodeType::BTYPE, p) {}

// 常量定义
// 表示常量定义的语法树节点
ConstDef::ConstDef(AstNode *p) : AstNode(NodeType::CONSTDEF, p) {}

// 常量初始化值
// 表示常量初始化值的语法树节点
ConstInitVal::ConstInitVal(AstNode *p) : AstNode(NodeType::CONSTINITVAL, p) {}

// 变量声明
// 表示变量声明的语法树节点
VarDecl::VarDecl(AstNode *p) : AstNode(NodeType::VARDECL, p) {}

// 变量定义
// 表示变量定义的语法树节点
VarDef::VarDef(AstNode *p) : AstNode(NodeType::VARDEF, p) {}

// 初始化值
// 表示变量或常量的初始化值的语法树节点
InitVal::InitVal(AstNode *p) : AstNode(NodeType::INITVAL, p) {}

// 函数类型
// 表示函数的返回类型的语法树节点
FuncType::FuncType(AstNode *p) : AstNode(NodeType::FUNCTYPE, p) {}

// 函数形参
// 表示函数形参的语法树节点
FuncFParam::FuncFParam(AstNode *p) : AstNode(NodeType::FUNCFPARAM, p) {}

// 函数形参列表
// 表示函数形参列表的语法树节点
FuncFParams::FuncFParams(AstNode *p) : AstNode(NodeType::FUNCFPARAMS, p) {}

// 代码块
// 表示代码块的语法树节点
Block::Block(AstNode *p) : AstNode(NodeType::BLOCK, p) {}

// 代码块项
// 表示代码块中的一项的语法树节点
BlockItem::BlockItem(AstNode *p) : AstNode(NodeType::BLOCKITEM, p) {}

// 表达式
// 表示表达式的语法树节点
Exp::Exp(AstNode *p) : AstNode(NodeType::EXP, p) {}

// 条件
// 表示条件表达式的语法树节点
Cond::Cond(AstNode *p) : AstNode(NodeType::COND, p) {}
// 语句
// 表示语句的语法树节点
Stmt::Stmt(AstNode *p) : AstNode(NodeType::STMT, p) {}

// 左值
// 表示左值的语法树节点
LVal::LVal(AstNode *p) : AstNode(NodeType::LVAL, p) {}

// 函数实参列表
// 表示函数实参列表的语法树节点
FuncRParams::FuncRParams(AstNode *p) : AstNode(NodeType::FUNCRPARAMS, p) {}
// 加法表达式
// 表示加法运算表达式的语法树节点
AddExp::AddExp(AstNode *p) : AstNode(NodeType::ADDEXP, p) {}
// 乘法表达式
// 表示乘法运算表达式的语法树节点
MulExp::MulExp(AstNode *p) : AstNode(NodeType::MULEXP, p) {}

// 数字
// 表示数字的语法树节点
Number::Number(AstNode *p) : AstNode(NodeType::NUMBER, p) {}

// 基础表达式
// 表示基础表达式的语法树节点
PrimaryExp::PrimaryExp(AstNode *p) : AstNode(NodeType::PRIMARYEXP, p) {}
// 关系表达式
// 表示关系运算表达式的语法树节点
RelExp::RelExp(AstNode *p) : AstNode(NodeType::RELEXP, p) {}

// 相等表达式
// 表示相等运算表达式的语法树节点
EqExp::EqExp(AstNode *p) : AstNode(NodeType::EQEXP, p) {}

// 逻辑与表达式
// 表示逻辑与运算表达式的语法树节点
LAndExp::LAndExp(AstNode *p) : AstNode(NodeType::LANDEXP, p) {}

// 逻辑或表达式
// 表示逻辑或运算表达式的语法树节点
LOrExp::LOrExp(AstNode *p) : AstNode(NodeType::LOREXP, p) {}

// 常量表达式
// 表示常量表达式的语法树节点
ConstExp::ConstExp(AstNode *p) : AstNode(NodeType::CONSTEXP, p) {}
// 一元表达式
// 表示一元运算表达式的语法树节点
UnaryExp::UnaryExp(AstNode *p) : AstNode(NodeType::UNARYEXP, p) {}

// 一元运算符
// 表示一元运算符的语法树节点
UnaryOp::UnaryOp(AstNode *p) : AstNode(NodeType::UNARYOP, p) {}
