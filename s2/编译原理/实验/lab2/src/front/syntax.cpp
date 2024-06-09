/*
代码参考了https://gitee.com/wangxingran222/Compiler_Theory_EXP1/） 重庆大学编译原理2022教改项目实验指导书
算法主要参考了编译原理_中科大(华保健)https://www.bilibili.com/video/BV16h411X7JY/
最朴素的自顶向下分析思想是：
tokens[]; // holding all tokens
i = 0; // 指向第i个token
stack = [S] // S是开始符号
while (stack != [])
    if (stack[top] is a terminal t)
        if (t == tokens[i++]) // 如果匹配成功
            pop();
        else
            backtrack();
    else if (stack[top] is a nonterminal T)
        pop();
        push(the next right hand side of T) // 不符合，尝试下一个右部式
定义辅助函数First_XX ，求取FIRST集合方式返回。
使用递归下降法生成抽象语法树，在构造产生式时从左向右依次判断当前token 是否符合文法规则，符合则匹配成功，失败则回退。
*/
#include "front/syntax.h"

#include <iostream>
#include <cassert>
#include <unordered_set>

using frontend::Parser;

// #define DEBUG_PARSER
// #define TODO assert(0 && "todo")
#define CUR_TOKEN_IS(tk_type) (token_stream[index].type == TokenType::tk_type)
#define NEW_TERM(result) (new Term(token_stream[index++], result))
Parser::Parser(const std::vector<frontend::Token> &tokens) : index(0), token_stream(tokens) {}

Parser::~Parser() {}

frontend::CompUnit *Parser::get_abstract_syntax_tree()
{
    // 从根节点CompUnit开始构造抽象语法树
    return parseCompUnit(nullptr);
}

void Parser::log(AstNode *node)
{
}

// 辅助函数，定义辅助函数 First_XX 为每种产生式的构造提供可选的终结符/非终结符，以集合方式返回；用于判断某文法内的分支.
std::unordered_set<frontend::TokenType> Parser::First_Compunit()
{
    /*  1.CompUnit -> (Decl | FuncDef) [CompUnit]
        可能的分支有俩个
        1.1.ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'
        BType -> 'int' | 'float'
        ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal

        auto first_decl = First_Decl();   First_Decl(),  有 常量 declaration 和 变量 declartion, 前者返回'const'， 后者返回 'int'、'float'
        返回两个集合的并集

        1.2.FuncType -> 'void' | 'int' | 'float'
        auto first_funcdef = First_FuncDef(); 函数声明的开头为函数类型， 返回'void'、'int'、'float'
    */
    auto first_decl = First_Decl();
    auto first_funcdef = First_FuncDef();
    first_funcdef.insert(first_decl.begin(), first_decl.end());
    return first_funcdef;
}

std::unordered_set<frontend::TokenType> Parser::First_Decl()
{
    /*
        First_Decl(),  有 常量 declaration 和 变量 declartion, 前者返回'const'， 后者返回 'int'、'float'
    */
    auto first_constdecl = First_ConstDecl();
    auto first_vardecl = First_VarDecl();
    first_vardecl.insert(first_constdecl.begin(), first_constdecl.end());
    return first_vardecl;
}

std::unordered_set<frontend::TokenType> Parser::First_ConstDecl()
{
    // ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'，开头是终结符const
    return {frontend::TokenType::CONSTTK};
}

std::unordered_set<frontend::TokenType> Parser::First_BType()
{
    // BType -> 'int' | 'float'
    return {frontend::TokenType::INTTK, frontend::TokenType::FLOATTK};
}

std::unordered_set<frontend::TokenType> Parser::First_ConstDef()
{
    return {frontend::TokenType::IDENFR};
}

std::unordered_set<frontend::TokenType> Parser::First_ConstInitVal()
{
    /*  ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
        开头ConstExp 或者 '{'
    */
    auto first_constexp = First_ConstExp();
    // 合并插入'{'
    first_constexp.insert(frontend::TokenType::LBRACE);
    return first_constexp;
}

std::unordered_set<frontend::TokenType> Parser::First_VarDecl()
{
    return First_BType();
}

std::unordered_set<frontend::TokenType> Parser::First_VarDef()
{
    return {frontend::TokenType::IDENFR};
}

std::unordered_set<frontend::TokenType> Parser::First_InitVal()
{
    auto first_exp = First_Exp();
    first_exp.insert(frontend::TokenType::LBRACE);
    return first_exp;
}

std::unordered_set<frontend::TokenType> Parser::First_FuncDef()
{
    return First_FuncType();
}

std::unordered_set<frontend::TokenType> Parser::First_FuncType()
{
    // FuncType -> 'void' | 'int' | 'float'
    return {frontend::TokenType::VOIDTK, frontend::TokenType::INTTK, frontend::TokenType::FLOATTK}; // 返回'void'、'int'、'float'
}

std::unordered_set<frontend::TokenType> Parser::First_FuncFParam()
{
    return First_BType();
}

std::unordered_set<frontend::TokenType> Parser::First_FuncFParams()
{
    /*   FuncFParams -> FuncFParam { ',' FuncFParam }
         FuncFParam -> BType Ident ['[' ']' { '[' Exp ']' }]
         函数参数的后面的是单个参数，单个参数又以基本数据类型做开头
     */
    return First_FuncFParam();
}

std::unordered_set<frontend::TokenType> Parser::First_Block()
{
    /* Block -> '{' { BlockItem } '}'
        代码块项
        表示代码块中的一项的语法树节点
    */
    return {frontend::TokenType::LBRACE};
}

std::unordered_set<frontend::TokenType> Parser::First_BlockItem()
{
    auto first_decl = First_Decl();
    auto first_stmt = First_Stmt();
    first_stmt.insert(first_decl.begin(), first_decl.end());
    return first_stmt;
}

std::unordered_set<frontend::TokenType> Parser::First_Stmt()
{
    /*
    语句
    表示语句的语法树节点
    Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | [Exp] ';'
    */
    auto first_lval = First_LVal();
    auto first_block = First_Block();
    first_lval.insert(first_block.begin(), first_block.end());
    first_lval.insert(frontend::TokenType::IFTK);
    first_lval.insert(frontend::TokenType::WHILETK);
    first_lval.insert(frontend::TokenType::BREAKTK);
    first_lval.insert(frontend::TokenType::CONTINUETK);
    first_lval.insert(frontend::TokenType::RETURNTK);
    auto first_exp = First_Exp();
    first_lval.insert(first_exp.begin(), first_exp.end());
    first_lval.insert(frontend::TokenType::SEMICN);
    return first_lval;
}

std::unordered_set<frontend::TokenType> Parser::First_Exp()
{
    return First_AddExp();
}

std::unordered_set<frontend::TokenType> Parser::First_Cond()
{
    return First_LOrExp();
}

std::unordered_set<frontend::TokenType> Parser::First_LVal()
{
    // 左值 LVal -> Ident {'[' Exp ']'}
    return {frontend::TokenType::IDENFR};
}

std::unordered_set<frontend::TokenType> Parser::First_Number()
{
    return {frontend::TokenType::INTLTR, frontend::TokenType::FLOATLTR};
}

std::unordered_set<frontend::TokenType> Parser::First_PrimaryExp()
{
    auto first_lval = First_LVal();
    auto first_number = First_Number();
    first_lval.insert(first_number.begin(), first_number.end());
    first_lval.insert(frontend::TokenType::LPARENT);
    return first_lval; // 返回'('、LVal、Number
}

std::unordered_set<frontend::TokenType> Parser::First_UnaryExp()
{
    /* 一元表达式
    UnaryExp -> PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp
    // 返回PrimaryExp、Ident、UnaryOp
    */
    auto first_primaryexp = First_PrimaryExp();
    first_primaryexp.insert(frontend::TokenType::IDENFR);
    auto first_unaryop = First_UnaryOp();
    first_primaryexp.insert(first_unaryop.begin(), first_unaryop.end());
    return first_primaryexp;
}

std::unordered_set<frontend::TokenType> Parser::First_UnaryOp()
{
    return {frontend::TokenType::PLUS, frontend::TokenType::MINU, frontend::TokenType::NOT};
}

std::unordered_set<frontend::TokenType> Parser::First_FuncRParams()
{
    return First_Exp();
}

std::unordered_set<frontend::TokenType> Parser::First_MulExp()
{
    // 多元表达->一元表达式
    return First_UnaryExp();
}

std::unordered_set<frontend::TokenType> Parser::First_AddExp()
{
    return First_MulExp();
}

std::unordered_set<frontend::TokenType> Parser::First_RelExp()
{
    return First_AddExp();
}

std::unordered_set<frontend::TokenType> Parser::First_EqExp()
{
    return First_RelExp();
}

std::unordered_set<frontend::TokenType> Parser::First_LAndExp()
{
    // EqExp -> RelExp { ('==' | '!=') RelExp }
    return First_EqExp();
}

std::unordered_set<frontend::TokenType> Parser::First_LOrExp()
{
    return First_LAndExp();
}

std::unordered_set<frontend::TokenType> Parser::First_ConstExp()
{
    // ConstExp -> AddExp
    return First_AddExp();
}

/*
是将匹配状态回退到之前的状态，主要是通过还原 index 和删除多余的子节点来实现。
在解析嵌套结构时，发现后续部分无法匹配时需要回退到之前的状态重新尝试匹配。
*/
void Parser::backtrack(int lastIndex, AstNode *curResult, int childNums)
{
    index = lastIndex;
    while (int(curResult->children.size()) > childNums)
    {
        curResult->children.pop_back();
    }
    return;
}

/*
parse 函数中，我们根据下一个需要处理的 token 类型和该节点不同产生式的 first 集来选择处理哪一
个产生式；在处理产生式时，应该按顺序从左到右依次处理，对非终结符调用其相应的 parse 函数，并
将得到的语法树节点加入该节点的子节点中；对终结符，我们使用一个特殊的 parse 函数 Term*
判断当前 index 所指的 Token 是否为产生式所要求的 Token 类型，如果不是则发生了错误，程序运
行结果则不可预计
2. 如果是符合预期的 Token 类型
i. a. 则 new 一个 Term 节点并将 Token 内容拷贝到节点中
ii. 将该节点加入 parent 的子节点
iii. Parser 的 index++
*/

frontend::CompUnit *Parser::parseCompUnit(AstNode *parent)
{ // CompUnit -> (Decl | FuncDef) [CompUnit]
    // 根节点 CompUnit,起始符
    CompUnit *result = new CompUnit(parent);
    bool decl_flag = false;
    bool funcdef_flag = false;
    int lastIndex = index;
    int childNums = result->children.size();
    if (First_Decl().count(token_stream[index].type))
    {
        decl_flag = parseDecl(result);
        if (!decl_flag)
        {
            backtrack(lastIndex, result, childNums);
        }
    }
    if (!decl_flag && First_FuncDef().count(token_stream[index].type))
    {
        funcdef_flag = parseFuncDef(result);
    }
    if (!decl_flag && !funcdef_flag)
    {
        assert(0 && (decl_flag?"true":"flase"));
    }

    // [CompUnit]
    if (First_Compunit().count(token_stream[index].type))
    {
        parseCompUnit(result);
    }

    return result;
}

// Decl -> ConstDecl | VarDecl
bool Parser::parseDecl(AstNode *parent)
{

    Decl *result = new Decl(parent);
    bool constdecl_flag = false;
    bool vardecl_flag = false;
    int lastIndex = index;
    int childNums = result->children.size();

    if (First_ConstDecl().count(token_stream[index].type))
    {
        constdecl_flag = parseConstDecl(result);
        if (!constdecl_flag)
        {
            backtrack(lastIndex, result, childNums);
        }
    }
    if (!constdecl_flag && First_VarDecl().count(token_stream[index].type))
    {
        vardecl_flag = parseVarDecl(result);
    }
    if (!constdecl_flag && !vardecl_flag)
    {
        return false;
    }

    return true;
}

// ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'
bool Parser::parseConstDecl(AstNode *parent)
{
    ConstDecl *result = new ConstDecl(parent);

    if (!CUR_TOKEN_IS(CONSTTK))
    {
        return false;
    }
    NEW_TERM(result);
    if (!parseBType(result))
    {
        return false;
    }
    if (!parseConstDef(result))
    {
        return false;
    }

    while (CUR_TOKEN_IS(COMMA))
    {
        NEW_TERM(result);
        if (!parseConstDef(result))
        {
            return false;
        }
    }

    if (!CUR_TOKEN_IS(SEMICN))
    {
        return false;
    }
    NEW_TERM(result);

    return true;
}

// BType -> 'int' | 'float'
bool Parser::parseBType(AstNode *parent)
{
    BType *result = new BType(parent);

    if (CUR_TOKEN_IS(INTTK))
    {
        NEW_TERM(result);
    }
    else if (CUR_TOKEN_IS(FLOATTK))
    {
        NEW_TERM(result);
    }
    else
    {
        return false;
    }

    return true;
}

// ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
bool Parser::parseConstDef(AstNode *parent)
{
    ConstDef *result = new ConstDef(parent);

    if (!CUR_TOKEN_IS(IDENFR))
    {
        return false;
    }
    NEW_TERM(result);

    while (CUR_TOKEN_IS(LBRACK))
    {
        NEW_TERM(result);
        if (!parseConstExp(result))
        {
            return false;
        }
        if (!CUR_TOKEN_IS(RBRACK))
        {
            return false;
        }
        NEW_TERM(result);
    }

    if (!CUR_TOKEN_IS(ASSIGN))
    {
        return false;
    }
    NEW_TERM(result);

    if (!parseConstInitVal(result))
    {
        return false;
    }

    return true;
}

// ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
bool Parser::parseConstInitVal(AstNode *parent)
{
    ConstInitVal *result = new ConstInitVal(parent);
    int lastIndex = index;
    int childNums = result->children.size();
    bool constExpFlag = false;
    bool lbraceFlag = false;
    if (First_ConstExp().count(token_stream[index].type))
    {
        constExpFlag = parseConstExp(result);
        if (!constExpFlag)
        {
            backtrack(lastIndex, result, childNums);
        }
    }
    if (!constExpFlag && CUR_TOKEN_IS(LBRACE))
    {
        NEW_TERM(result);
        if (First_ConstInitVal().count(token_stream[index].type))
        {
            if (!parseConstInitVal(result))
            {
                return false;
            }
            while (CUR_TOKEN_IS(COMMA))
            {
                NEW_TERM(result);
                if (!parseConstInitVal(result))
                {
                    return false;
                }
            }
        }
        if (!CUR_TOKEN_IS(RBRACE))
        {
            return false;
        }
        NEW_TERM(result);
        lbraceFlag = true;
        if (!lbraceFlag)
        {
            return false;
        }
    }
    if (!constExpFlag && !lbraceFlag)
    {
        return false;
    }

    return true;
}

// VarDecl -> BType VarDef { ',' VarDef } ';'
bool Parser::parseVarDecl(AstNode *parent)
{
    VarDecl *result = new VarDecl(parent);

    if (!parseBType(result))
    {
        return false;
    }
    if (!parseVarDef(result))
    {
        return false;
    }
    while (CUR_TOKEN_IS(COMMA))
    {
        NEW_TERM(result);
        if (!parseVarDef(result))
        {
            return false;
        }
    }
    if (!CUR_TOKEN_IS(SEMICN))
    {
        return false;
    }
    NEW_TERM(result);
    return true;
}

// VarDef -> Ident { '[' ConstExp ']' } [ '=' InitVal ]
bool Parser::parseVarDef(AstNode *parent)
{
    VarDef *result = new VarDef(parent);

    if (!CUR_TOKEN_IS(IDENFR))
    {
        return false;
    }
    NEW_TERM(result);

    while (CUR_TOKEN_IS(LBRACK))
    {
        NEW_TERM(result);
        if (!parseConstExp(result))
        {
            return false;
        }

        // if (!CUR_TOKEN_IS(RBRACK))
        if (!CUR_TOKEN_IS(RBRACK))
        {
            return false;
        }
        NEW_TERM(result);
    }

    if (CUR_TOKEN_IS(ASSIGN))
    {
        NEW_TERM(result);
        if (!parseInitVal(result))
        {
            return false;
        }
    }

    return true;
}

// InitVal -> Exp | '{' [ InitVal { ',' InitVal } ] '}'
bool Parser::parseInitVal(AstNode *parent)
{
    InitVal *result = new InitVal(parent);

    bool expFlag = false;
    bool lbraceFlag = false;
    int lastIndex = index;
    int childNums = result->children.size();

    if (First_Exp().count(token_stream[index].type))
    {
        expFlag = parseExp(result);
        if (!expFlag)
        {
            backtrack(lastIndex, result, childNums);
        }
    }
    if (!expFlag && CUR_TOKEN_IS(LBRACE))
    {
        NEW_TERM(result);

        if (First_InitVal().count(token_stream[index].type))
        {
            if (!parseInitVal(result))
            {
                return false;
            }

            while (CUR_TOKEN_IS(COMMA))
            {
                NEW_TERM(result);

                if (!parseInitVal(result))
                {
                    return false;
                }
            }
        }

        if (!CUR_TOKEN_IS(RBRACE))
        {
            return false;
        }
        NEW_TERM(result);
        lbraceFlag = true;
    }
    if (!lbraceFlag && !expFlag)
    {
        return false;
    }

    return true;
}

// FuncDef -> FuncType Ident '(' [FuncFParams] ')' Block
bool Parser::parseFuncDef(AstNode *parent)
{
    FuncDef *result = new FuncDef(parent);
    if (!parseFuncType(result))
    {
        return false;
    }

    if (!CUR_TOKEN_IS(IDENFR))
    {
        return false;
    }
    NEW_TERM(result);

    if (!CUR_TOKEN_IS(LPARENT))
    {
        return false;
    }
    NEW_TERM(result);

    if (First_FuncFParams().count(token_stream[index].type))
    {
        if (!parseFuncFParams(result))
        {
            return false;
        }
    }

    if (!CUR_TOKEN_IS(RPARENT))
    {
        return false;
    }
    NEW_TERM(result);
    if (!parseBlock(result))
    {
        return false;
    }
    return true;
}

// FuncType -> 'void' | 'int' | 'float'
bool Parser::parseFuncType(AstNode *parent)
{
    FuncType *result = new FuncType(parent);

    if (CUR_TOKEN_IS(VOIDTK))
    {
        NEW_TERM(result);
    }
    else if (CUR_TOKEN_IS(INTTK))
    {
        NEW_TERM(result);
    }
    else if (CUR_TOKEN_IS(FLOATTK))
    {
        NEW_TERM(result);
    }
    else
    {
        return false;
    }

    return true;
}

// FuncFParam -> BType Ident ['[' ']' { '[' Exp ']' }]
bool Parser::parseFuncFParam(AstNode *parent)
{
    FuncFParam *result = new FuncFParam(parent);
    if (!parseBType(result))
    {
        return false;
    }
    if (!CUR_TOKEN_IS(IDENFR))
    {
        return false;
    }
    NEW_TERM(result);
    if (CUR_TOKEN_IS(LBRACK))
    {
        NEW_TERM(result);
        if (!CUR_TOKEN_IS(RBRACK))
        {
            return false;
        }
        NEW_TERM(result);
        while (CUR_TOKEN_IS(LBRACK))
        {
            NEW_TERM(result);
            if (!parseExp(result))
            {
                return false;
            }
            if (!CUR_TOKEN_IS(RBRACK))
            {
                return false;
            }
            NEW_TERM(result);
        }
    }
    return true;
}

bool Parser::parseFuncFParams(AstNode *parent)
{ // FuncFParams -> FuncFParam { ',' FuncFParam }
    FuncFParams *result = new FuncFParams(parent);
    if (!parseFuncFParam(result))
    {
        return false;
    }
    while (CUR_TOKEN_IS(COMMA))
    {
        NEW_TERM(result);
        if (!parseFuncFParam(result))
        {
            return false;
        }
    }
    return true;
}

// Block -> '{' { BlockItem } '}'
bool Parser::parseBlock(AstNode *parent)
{
    Block *result = new Block(parent);
    if (!CUR_TOKEN_IS(LBRACE))
    {
        return false;
    }
    NEW_TERM(result);

    while (First_BlockItem().count(token_stream[index].type))
    {
        bool blockitem_result = parseBlockItem(result);
        if (!blockitem_result)
        {
            return false;
        }
    }
    if (!CUR_TOKEN_IS(RBRACE))
    {
        return false;
    }
    NEW_TERM(result);
    return true;
}

// BlockItem -> Decl | Stmt
bool Parser::parseBlockItem(AstNode *parent)
{
    BlockItem *result = new BlockItem(parent);

    bool decl_flag = false;
    bool stmt_flag = false;
    int lastIndex = index;
    int childNums = result->children.size();

    if (First_Decl().count(token_stream[index].type))
    {
        decl_flag = parseDecl(result);
        if (!decl_flag)
        {
            backtrack(lastIndex, result, childNums);
        }
    }
    if (!decl_flag && First_Stmt().count(token_stream[index].type))
    {
        stmt_flag = parseStmt(result);
    }
    if (!decl_flag && !stmt_flag)
    {
        return false;
    }

    return true;
}

bool Parser::parseStmt(AstNode *parent)
{ /* Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 
 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | [Exp] ';'
    */
    // 语句
    // 表示语句的语法树节点
    Stmt *result = new Stmt(parent);
    bool lValFlag = false;
    bool blockFlag = false;
    bool keywordFlag = false;
    bool expFlag = false;
    bool semicnFlag = false;
    int lastIndex = index;
    int childNums = result->children.size();
    if (First_LVal().count(token_stream[index].type))
    {
        bool flag = true;
        lValFlag = parseLVal(result); // LVal
        if (!lValFlag && flag)
        {
            backtrack(lastIndex, result, childNums);
            flag = false;
        }
        else if (!CUR_TOKEN_IS(ASSIGN) && flag)
        {
            lValFlag = false;
            backtrack(lastIndex, result, childNums);
            flag = false;
        }
        if (flag)
            NEW_TERM(result);
        if (!parseExp(result) && flag)
        {
            lValFlag = false;
            backtrack(lastIndex, result, childNums);
            flag = false;
            
        }
        else if (!CUR_TOKEN_IS(SEMICN) && flag)
        {
            lValFlag = false;
            backtrack(lastIndex, result, childNums);
            flag =false;
        }
        if (flag)
        {
            NEW_TERM(result);
            return true;
        }
    }
    if (!lValFlag && First_Block().count(token_stream[index].type))
    {
        blockFlag = parseBlock(result);
        if (!blockFlag)
        {
            backtrack(lastIndex, result, childNums);
        }
        else
        {
            return true;
        }
    }
    if (CUR_TOKEN_IS(IFTK))
    {
        NEW_TERM(result);
        bool ifFlag = true;
        if (!CUR_TOKEN_IS(LPARENT) && ifFlag)
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
            ifFlag = false;
        }
        if (ifFlag)
            NEW_TERM(result);
        if (!parseCond(result) && ifFlag)
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
            ifFlag = false;
        }
        if (!CUR_TOKEN_IS(RPARENT) && ifFlag)
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
            ifFlag = false;
        }
        if (ifFlag)
            NEW_TERM(result);
        if (!parseStmt(result) && ifFlag)
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
            ifFlag = false;
        }
        if (CUR_TOKEN_IS(ELSETK) && ifFlag)
        {
            NEW_TERM(result);
            if (!parseStmt(result))
            {
                keywordFlag = false;
                backtrack(lastIndex, result, childNums);
                ifFlag = false;
            }
        }
        if (ifFlag)
            return true;
    }
    else if (CUR_TOKEN_IS(WHILETK))
    {
        NEW_TERM(result);
        bool whileFlag = true;
        if (!CUR_TOKEN_IS(LPARENT) && whileFlag)
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
            whileFlag = false;
        }
        if (whileFlag)
            NEW_TERM(result);
        if (!parseCond(result) && whileFlag)
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
            whileFlag = false;
        }
        if (!CUR_TOKEN_IS(RPARENT) && whileFlag)
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
            whileFlag = false;
        }
        if (whileFlag)
            NEW_TERM(result);
        if (!parseStmt(result) && whileFlag)
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
            whileFlag = false;
        }
        if (whileFlag)
            return true;
    }
    else if (CUR_TOKEN_IS(BREAKTK))
    {
        NEW_TERM(result);

        if (!CUR_TOKEN_IS(SEMICN))
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
        }
        else
        {
            NEW_TERM(result);
            return true;
        }
    }
    else if (CUR_TOKEN_IS(CONTINUETK))
    {
        NEW_TERM(result);

        if (!CUR_TOKEN_IS(SEMICN))
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
        }
        else
        {
            NEW_TERM(result);
            return true;
        }
    }
    else if (CUR_TOKEN_IS(RETURNTK))
    {
        NEW_TERM(result);
        if (First_Exp().count(token_stream[index].type))
        {
            parseExp(result);
            NEW_TERM(result);
            return true;
        }
        if (!CUR_TOKEN_IS(SEMICN))
        {
            keywordFlag = false;
            backtrack(lastIndex, result, childNums);
        }
    }
    // EXP
    if (!lValFlag && !blockFlag && !keywordFlag && First_Exp().count(token_stream[index].type))
    {
        expFlag = parseExp(result);
        if (expFlag && CUR_TOKEN_IS(SEMICN))
        {
            NEW_TERM(result);
            return true;
        }
        else
        {
            expFlag = false;
            backtrack(lastIndex, result, childNums);
        }
    }
    // SEMICN;
    if (!lValFlag && !blockFlag && !keywordFlag && !expFlag && CUR_TOKEN_IS(SEMICN))
    {
        NEW_TERM(result);
        return true;
    }
    if (!lValFlag && !blockFlag && !keywordFlag && !expFlag && !semicnFlag)
    {
        return false;
    }
    return true;
}

// Exp -> AddExp
bool Parser::parseExp(AstNode *parent)
{
    Exp *result = new Exp(parent);
    if (!parseAddExp(result))
    {
        return false;
    }
    return true;
}

// Cond -> LOrExp
bool Parser::parseCond(AstNode *parent)
{
    Cond *result = new Cond(parent);

    if (!parseLOrExp(result))
    {
        return false;
    }

    return true;
}

// LVal -> Ident {'[' Exp ']'}
bool Parser::parseLVal(AstNode *parent)
{
    LVal *result = new LVal(parent);

    if (!CUR_TOKEN_IS(IDENFR))
    {
        return false;
    }
    NEW_TERM(result);

    while (CUR_TOKEN_IS(LBRACK))
    {
        NEW_TERM(result);

        if (!parseExp(result))
        {
            return false;
        }

        if (!CUR_TOKEN_IS(RBRACK))
        {
            return false;
        }
        NEW_TERM(result);
    }

    return true;
}

// Number -> IntConst | floatConst
bool Parser::parseNumber(AstNode *parent)
{
    Number *result = new Number(parent);

    if (!CUR_TOKEN_IS(INTLTR) && !CUR_TOKEN_IS(FLOATLTR))
    {
        return false;
    }
    NEW_TERM(result);

    return true;
}

// PrimaryExp -> '(' Exp ')' | LVal | Number
bool Parser::parsePrimaryExp(AstNode *parent)
{
    PrimaryExp *result = new PrimaryExp(parent);

    bool LeftBracketFlag = false;
    bool lValFlag = false;
    bool numberFlag = false;
    int lastIndex = index;
    int childNums = result->children.size();
    if (CUR_TOKEN_IS(LPARENT))
    {
        //'(' Exp ')'
        NEW_TERM(result);
        LeftBracketFlag = parseExp(result);
        if (LeftBracketFlag && CUR_TOKEN_IS(RPARENT))
        {
            NEW_TERM(result);
            return true;
        }
        else
        {
            LeftBracketFlag = false;
            backtrack(lastIndex, result, childNums);
        }
    }
    if (!LeftBracketFlag && First_LVal().count(token_stream[index].type))
    { // LVal
        lValFlag = parseLVal(result);
        if (!lValFlag)
        {
            backtrack(lastIndex, result, childNums);
        }
        else
        {
            return true;
        }
    }
    if (!LeftBracketFlag && !lValFlag && First_Number().count(token_stream[index].type))
    { // Number
        numberFlag = parseNumber(result);
        if (!numberFlag)
        {
            backtrack(lastIndex, result, childNums);
        }
        else
        {
            return true;
        }
    }
    if (!LeftBracketFlag && !lValFlag && !numberFlag)
    {
        return false;
    }
    return true;
}

bool Parser::parseUnaryExp(AstNode *parent)
{
    UnaryExp *result = new UnaryExp(parent);
    if (CUR_TOKEN_IS(IDENFR))
    {
        if (token_stream[index + 1].type == frontend::TokenType::LPARENT)
        {
            NEW_TERM(result);
            NEW_TERM(result);

            if (First_FuncRParams().count(token_stream[index].type))
            {
                if (!parseFuncRParams(result))
                {
                    return false;
                }
            }
            if (!CUR_TOKEN_IS(RPARENT))
            {
                return false;
            }
            NEW_TERM(result);
            return true;
        }
        else
        {
            if (!parsePrimaryExp(result))
            {
                return false;
            }
            return true;
        }
    }
    if (First_PrimaryExp().count(token_stream[index].type))
    {
        if (!parsePrimaryExp(result))
        {
            return false;
        }
        return true;
    }
    if (First_UnaryOp().count(token_stream[index].type))
    {
        if (!parseUnaryOp(result))
        {
            return false;
        }
        if (!parseUnaryExp(result))
        {
            return false;
        }
        return true;
    }
    return false;
}

// UnaryOp -> '+' | '-' | '!'
bool Parser::parseUnaryOp(AstNode *parent)
{
    UnaryOp *result = new UnaryOp(parent);
    if (!CUR_TOKEN_IS(PLUS) && !CUR_TOKEN_IS(MINU) && !CUR_TOKEN_IS(NOT))
    {
        return false;
    }
    NEW_TERM(result);
    return true;
}

// FuncRParams -> Exp { ',' Exp }
bool Parser::parseFuncRParams(AstNode *parent)
{
    FuncRParams *result = new FuncRParams(parent);
    if (!parseExp(result))
    {
        return false;
    }
    while (CUR_TOKEN_IS(COMMA))
    {
        NEW_TERM(result);
        if (!parseExp(result))
        {
            return false;
        }
    }
    return true;
}

// MulExp -> UnaryExp { ('*' | '/' | '%') UnaryExp }
bool Parser::parseMulExp(AstNode *parent)
{
    MulExp *result = new MulExp(parent);
    if (!parseUnaryExp(result))
    {
        return false;
    }
    while (CUR_TOKEN_IS(MULT) || CUR_TOKEN_IS(DIV) || CUR_TOKEN_IS(MOD))
    {
        NEW_TERM(result);
        if (!parseUnaryExp(result))
        {
            return false;
        }
    }
    return true;
}

// AddExp -> MulExp { ('+' | '-') MulExp }
bool Parser::parseAddExp(AstNode *parent)
{
    AddExp *result = new AddExp(parent);
    if (!parseMulExp(result))
    {
        return false;
    }
    while (CUR_TOKEN_IS(PLUS) || CUR_TOKEN_IS(MINU))
    {
        NEW_TERM(result);
        if (!parseMulExp(result))
        {
            return false;
        }
    }
    return true;
}

bool Parser::parseRelExp(AstNode *parent)
{ // RelExp -> AddExp { ('<' | '>' | '<=' | '>=') AddExp }
    RelExp *result = new RelExp(parent);
    if (!parseAddExp(result))
    {
        return false;
    }
    while (CUR_TOKEN_IS(LSS) || CUR_TOKEN_IS(GTR) || CUR_TOKEN_IS(LEQ) || CUR_TOKEN_IS(GEQ))
    {
        NEW_TERM(result);
        if (!parseAddExp(result))
        {
            return false;
        }
    }
    return true;
}

bool Parser::parseEqExp(AstNode *parent)
{ // EqExp -> RelExp { ('==' | '!=') RelExp }
    EqExp *result = new EqExp(parent);
    if (!parseRelExp(result))
    {
        return false;
    }
    while (CUR_TOKEN_IS(EQL) || CUR_TOKEN_IS(NEQ))
    {
        NEW_TERM(result);
        if (!parseRelExp(result))
        {
            return false;
        }
    }
    return true;
}

bool Parser::parseLAndExp(AstNode *parent)
{ // LAndExp -> EqExp [ '&&' LAndExp ]
    LAndExp *result = new LAndExp(parent);
    if (!parseEqExp(result))
    {
        return false;
    }
    if (CUR_TOKEN_IS(AND))
    {
        NEW_TERM(result);
        if (!parseLAndExp(result))
        {
            return false;
        }
    }
    return true;
}

bool Parser::parseConstExp(AstNode *parent)
{ // ConstExp -> AddExp
    ConstExp *result = new ConstExp(parent);
    if (!parseAddExp(result))
    {
        return false;
    }
    return true;
}

bool Parser::parseLOrExp(AstNode *parent)
{ // LOrExp -> LAndExp [ '||' LOrExp ]
    LOrExp *result = new LOrExp(parent);
    if (!parseLAndExp(result))
    {
        return false;
    }
    if (CUR_TOKEN_IS(OR))
    {
        NEW_TERM(result);
        if (!parseLOrExp(result))
        {
            return false;
        }
    }
    return true;
}
