#ifndef SYNTAX_H
#define SYNTAX_H

#include"front/abstract_syntax_tree.h"
#include"front/token.h"

#include<vector>

#include<unordered_set>
namespace frontend {

// definition of Parser
// a parser should take a token stream as input, then parsing it, output a AST
struct Parser {
    uint32_t index; // current token index
    const std::vector<Token>& token_stream;

    /**
     * @brief constructor
     * @param tokens: the input token_stream
     */
    Parser(const std::vector<Token>& tokens);

    /**
     * @brief destructor
     */
    ~Parser();
    
    /**
     * @brief creat the abstract syntax tree
     * @return the root of abstract syntax tree
     */
        CompUnit* get_abstract_syntax_tree();

    CompUnit *parseCompUnit(AstNode *parent);
    bool parseDecl(AstNode *parent);
    bool parseFuncDef(AstNode *parent);
    bool parseConstDecl(AstNode *parent);
    bool parseVarDecl(AstNode *parent);
    bool parseBType(AstNode *parent);
    bool parseConstDef(AstNode *parent);
    bool parseConstInitVal(AstNode *parent);
    bool parseConstExp(AstNode *parent);
    bool parseTerm(AstNode *parent);
    bool parseVarDef(AstNode *parent);
    bool parseInitVal(AstNode *parent);
    bool parseExp(AstNode *parent);
    bool parseFuncType(AstNode *parent);
    bool parseFuncFParams(AstNode *parent);
    bool parseBlock(AstNode *parent);
    bool parseFuncFParam(AstNode *parent);
    bool parseBlockItem(AstNode *parent);
    bool parseStmt(AstNode *parent);
    bool parseLVal(AstNode *parent);
    bool parseCond(AstNode *parent);
    bool parseAddExp(AstNode *parent);
    bool parseLOrExp(AstNode *parent);
    bool parseNumber(AstNode *parent);
    bool parsePrimaryExp(AstNode *parent);
    bool parseUnaryExp(AstNode *parent);
    bool parseUnaryOp(AstNode *parent);
    bool parseFuncRParams(AstNode *parent);
    bool parseMulExp(AstNode *parent);
    bool parseRelExp(AstNode *parent);
    bool parseEqExp(AstNode *parent);
    bool parseLAndExp(AstNode *parent);

    void backtrack(int saved_index, AstNode* curr_result, int target_child_nums);

    std::unordered_set<frontend::TokenType> First_Compunit();
    std::unordered_set<frontend::TokenType> First_Decl();
    std::unordered_set<frontend::TokenType> First_ConstDecl();
    std::unordered_set<frontend::TokenType> First_BType();
    std::unordered_set<frontend::TokenType> First_ConstDef();
    std::unordered_set<frontend::TokenType> First_ConstInitVal();
    std::unordered_set<frontend::TokenType> First_VarDecl();
    std::unordered_set<frontend::TokenType> First_VarDef();
    std::unordered_set<frontend::TokenType> First_InitVal();
    std::unordered_set<frontend::TokenType> First_FuncDef();
    std::unordered_set<frontend::TokenType> First_FuncType();
    std::unordered_set<frontend::TokenType> First_FuncFParam();
    std::unordered_set<frontend::TokenType> First_FuncFParams();
    std::unordered_set<frontend::TokenType> First_Block();
    std::unordered_set<frontend::TokenType> First_BlockItem(); // 1
    std::unordered_set<frontend::TokenType> First_Stmt();
    std::unordered_set<frontend::TokenType> First_Exp();
    std::unordered_set<frontend::TokenType> First_Cond();
    std::unordered_set<frontend::TokenType> First_LVal();
    std::unordered_set<frontend::TokenType> First_Number();
    std::unordered_set<frontend::TokenType> First_PrimaryExp();
    std::unordered_set<frontend::TokenType> First_UnaryExp();
    std::unordered_set<frontend::TokenType> First_UnaryOp();
    std::unordered_set<frontend::TokenType> First_FuncRParams();
    std::unordered_set<frontend::TokenType> First_MulExp();
    std::unordered_set<frontend::TokenType> First_AddExp();
    std::unordered_set<frontend::TokenType> First_RelExp();
    std::unordered_set<frontend::TokenType> First_EqExp();
    std::unordered_set<frontend::TokenType> First_LAndExp();
    std::unordered_set<frontend::TokenType> First_LOrExp();
    std::unordered_set<frontend::TokenType> First_ConstExp();
    
    /**
     * @brief for debug, should be called in the beginning of recursive descent functions 
     * @param node: current parsing node 
     */
    void log(AstNode* node);
};

} // namespace frontend

#endif