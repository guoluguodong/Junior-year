#include "front/lexical.h"

#include <map>
#include <cassert>
#include <string>

#define TODO assert(0 && "todo")
using namespace std;
using namespace frontend;
// 在token.h里
// #define DEBUG_DFA
// #define DEBUG_SCANNER

string toString(State s)
{
    switch (s)
    {
    case State::Empty:
        return "Empty";
    case State::Ident:
        return "Ident";
    case State::IntLiteral:
        return "IntLiteral";
    case State::FloatLiteral:
        return "FloatLiteral";
    case State::op:
        return "op";
    default:
        assert(0 && "invalid State");
    }
    return "";
}

set<string> keywords = {
    "const", "int", "float", "if", "else", "while", "continue", "break", "return", "void"};
// 在token.h定义了TokenType的枚举类
/*
    IDENFR,     // identifier
    INTLTR,		// int literal
    FLOATLTR,		// float literal
    CONSTTK,		// const
    VOIDTK,		// void
    INTTK,		// int
    FLOATTK,		// float
    IFTK,		// if
    ELSETK,		// else
    WHILETK,		// while
    CONTINUETK,		// continue
    BREAKTK,		// break
    RETURNTK,		// return
 */
TokenType get_ident_type(string str)
{   
    if (str == "const")
        return TokenType::CONSTTK;
    else if (str == "void")
        return TokenType::VOIDTK;
    else if (str == "int")
        return TokenType::INTTK;
    else if (str == "float")
        return TokenType::FLOATTK;
    else if (str == "if")
        return TokenType::IFTK;
    else if (str == "else")
        return TokenType::ELSETK;
    else if (str == "while")
        return TokenType::WHILETK;
    else if (str == "continue")
        return TokenType::CONTINUETK;
    else if (str == "break")
        return TokenType::BREAKTK;
    else if (str == "return")
        return TokenType::RETURNTK;
    else 
        return TokenType::IDENFR;
}
/*
 PLUS,		// +
    MINU,		// -
    MULT,		// *
    DIV,		// /
    MOD,      // %
    LSS,		// <
    GTR,		// >
    COLON,		// :
    ASSIGN,		// =
    SEMICN,		// ;
    COMMA,		// ,
    LPARENT,		// (
    RPARENT,		// )
    LBRACK,		// [
    RBRACK,		// ]
    LBRACE,		// {
    RBRACE,		// }
    NOT,		// !
    LEQ,		// <=
    GEQ,		// >=
    EQL,		// ==
    NEQ,		// !=
    AND,        // &&
    OR,         // ||
*/
TokenType getOpType(string str)
{
    if (str == "+")
        return TokenType::PLUS;
    else if (str == "-")
        return TokenType::MINU;
    else if (str == "*")
        return TokenType::MULT;
    else if (str == "/")
        return TokenType::DIV;
    else if (str == "%")
        return TokenType::MOD;
    else if (str == "<")
        return TokenType::LSS;
    else if (str == ">")
        return TokenType::GTR;
    else if (str == ":")
        return TokenType::COLON;
    else if (str == "=")
        return TokenType::ASSIGN;
    else if (str == ";")
        return TokenType::SEMICN;
    else if (str == ",")
        return TokenType::COMMA;
    else if (str == "(")
        return TokenType::LPARENT;
    else if (str == ")")
        return TokenType::RPARENT;
    else if (str == "[")
        return TokenType::LBRACK;
    else if (str == "]")
        return TokenType::RBRACK;
    else if (str == "{")
        return TokenType::LBRACE;
    else if (str == "}")
        return TokenType::RBRACE;
    else if (str == "!")
        return TokenType::NOT;
    else if (str == "<=")
        return TokenType::LEQ;
    else if (str == ">=")
        return TokenType::GEQ;
    else if (str == "==")
        return TokenType::EQL;
    else if (str == "!=")
        return TokenType::NEQ;
    else if (str == "&&")
        return TokenType::AND;
    else if (str == "||")
        return TokenType::OR;
    else return TokenType::PLUS;
}

bool opIsChar(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '<' || c == '>' || c == '=' || c == ':' ||\
     c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '!' || c == '&' || c == '|' || c == ',';
}

bool opIsString(string str)
{
    if(str.size()==1)   return opIsChar(str[0]);
    else    return str == "<=" || str == ">=" || str == "==" || str == "!=" || str == "&&" || str == "||";
}

string removeComments(ifstream &fileStream)
{
    string l;
    string res;
    bool multiLineComment = false;

    while (getline(fileStream, l))
    {
        size_t pos = 0;
        if (multiLineComment)
        {
            pos = l.find("*/");
            if (pos != string::npos)
            {
                multiLineComment = false;
                l.erase(0, pos + 2);
            }
            else
            {
                continue;
            }
        }

        while (pos != string::npos)
        {
            size_t posSingle = l.find("//", pos);
            size_t posMulti = l.find("/*", pos);
            if (posSingle == pos)
            {
                l.erase(pos);
                break;
            }
            else if (posMulti == pos)
            {
                size_t posMultiEnd = l.find("*/", posMulti + 2);
                if (posMultiEnd != string::npos)
                {
                    l.erase(posMulti, posMultiEnd - posMulti + 2);
                }
                else
                {
                    l.erase(posMulti);
                    multiLineComment = true;
                }
                break;
            }
            pos = min(posSingle, posMulti);
        }
        res += l + "\n";
    }
    return res;
}

DFA::DFA() : cur_state(State::Empty), cur_str() {}

DFA::~DFA() {}

bool DFA::next(char input, Token &buf)
{
    //      Empty,        // space, \n, \r ...
    //     Ident,        // a keyword or identifier, like 'int' 'a0' 'else' ...
    //     IntLiteral,   // int literal, like '1' '1900', only in decimal
    //     FloatLiteral, // float literal, like '0.1'
    //     op            // operators and '{', '[', '(', ',' ...
    bool tkSignal = false;
    switch (cur_state)
    {
    case State::Empty:
        if (isspace(input))
        { // sapce
            reset();
            break;
        }
        else if (isalpha(input) || input == '_')
        {
            cur_state = State::Ident;
            cur_str = cur_str +input;
            break;
        }
        else if (isdigit(input))
        { // 数字
            cur_state = State::IntLiteral;
            cur_str = cur_str +input;
            break;
        }
        else if (input == '.')
        { // 浮点数
            cur_state = State::FloatLiteral;
            cur_str = cur_str +input;
            break;
        }
        else if (opIsChar(input))
        { // charOp
            cur_state = State::op;
            cur_str = cur_str +input;
            break;
        }
        else
        { // else invalid
            assert(0 && "invalid next State");
            break;
        }
    case State::Ident:
        //  Ident,        // a keyword or identifier, like 'int' 'a0' 'else' ...
        if (isspace(input))
        {
            buf.type = get_ident_type(cur_str);
            buf.value = cur_str;
            reset();
            tkSignal = true;
            break;
        }
        else if (isalpha(input) || isdigit(input) || input == '_')
        {
            cur_str = cur_str +input;
            break;
        }
        else if (opIsChar(input))
        {
            buf.type = get_ident_type(cur_str);
            buf.value = cur_str;
            cur_state = State::op;
            cur_str = input;
            tkSignal = true;
            break;
        }
    case State::op:
        //     op            // operators and '{', '[', '(', ',' ...
        if (isspace(input))
        {
            buf.type = getOpType(cur_str);
            buf.value = cur_str;
            reset();
            tkSignal = true;
            break;
        }
        else if (isalpha(input) || input == '_')
        { // 字母或者下划线
            buf.type = getOpType(cur_str);
            buf.value = cur_str;
            cur_state = State::Ident;
            cur_str = input;
            tkSignal = true;
            break;
        }
        else if (isdigit(input))
        { // 数字
            buf.type = getOpType(cur_str);
            buf.value = cur_str;
            cur_state = State::IntLiteral;
            cur_str = input;
            tkSignal = true;
            break;
        }
        else if (input == '.')
        { // 浮点数
            buf.type = getOpType(cur_str);
            buf.value = cur_str;
            cur_state = State::FloatLiteral;
            cur_str = input;
            tkSignal = true;
            break;
        }
        else if (opIsChar(input))
        {
            if (opIsString(cur_str + input))
            // consider stringop, eg: >=
            {
                cur_str = cur_str +input;
                break;
            }
            else
            {
                buf.type = getOpType(cur_str);
                buf.value = cur_str;
                cur_state = State::op;
                cur_str = input;
                tkSignal = true;
                break;
            }
        }
    case State::IntLiteral:
        if (isspace(input))
        {
            buf.type = TokenType::INTLTR;
            buf.value = cur_str;
            reset();
            tkSignal = true;
            break;
        }
        else if (isdigit(input) || (input >= 'a' && input <= 'f') || (input >= 'A' && input <= 'F') || input == 'x' || input == 'X')
        {
            cur_str = cur_str +input;
            break;
        }
        else if (input == '.')
        {
            cur_state = State::FloatLiteral;
            cur_str = cur_str +input;
            break;
        }
        else if (opIsChar(input))
        {
            buf.type = TokenType::INTLTR;
            buf.value = cur_str;
            cur_state = State::op;
            cur_str = input;
            tkSignal = true;
            break;
        }
        
    case State::FloatLiteral:
        if (isspace(input))
        {
            buf.type = TokenType::FLOATLTR;
            buf.value = cur_str;
            reset();
            tkSignal = true;
            break;
        }
        else if (isdigit(input))
        { // 保持浮点数
            cur_str = cur_str +input;
            break;
        }
        else if (opIsChar(input))
        {
            buf.type = TokenType::FLOATLTR;
            buf.value = cur_str;
            cur_state = State::op;
            cur_str = input;
            tkSignal = true;
            break;
        }
    default:
        assert(0 && "invalid State");
    }
    return tkSignal;
}

void DFA::reset()
{
    cur_state = State::Empty;
    cur_str = "";
}

Scanner::Scanner(string filename) : fin(filename)
{
    if (!fin.is_open())
    {
        assert(0 && "in Scanner constructor, input file cannot open");
    }
}

Scanner::~Scanner()
{
    fin.close();
}

vector<Token> Scanner::run()
{
    // 仿照作业1，但多了删除注释的步骤
    string str = removeComments(fin);
    str += "\n";
    vector<Token> result;
    Token token;
    DFA dfa; 
    // 有限自动机
    for (char c : str)
    {
        if (dfa.next(c, token))
        {
            result.push_back(token);
        }
    }
    return result;
}