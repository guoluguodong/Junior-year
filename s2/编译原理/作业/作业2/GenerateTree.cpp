/*
Exp  ->  AddExp

        Exp.v

Number  ->  IntConst  |  floatConst

PrimaryExp  ->  '('  Exp  ')'  |  Number
        PrimaryExp.v

UnaryExp  ->  PrimaryExp  |  UnaryOp  UnaryExp
        UnaryExp.v

UnaryOp  ->  '+'  |  '-'

MulExp  ->  UnaryExp  {  ('*'  |  '/')  UnaryExp  }
        MulExp.v

AddExp  ->  MulExp  {  ('+'  |  '-')  MulExp  }
        AddExp.v
*/
#include <map>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <stdint.h>
#define TODO assert(0 && "TODO")
//  #define  DEBUG_DFA
//  #define  DEBUG_PARSER

//  enumerate  for  Status
enum class State
{
        Empty,      //  space,  \n,  \r  ...
        IntLiteral, //  int  literal,  like  '1'  '01900',  '0xAB',  '0b11001'
        op          //  operators  and  '(',  ')'
};
std::string toString(State s)
{
        switch (s)
        {
        case State::Empty:
                return "Empty";
        case State::IntLiteral:
                return "IntLiteral";
        case State::op:
                return "op";
        default:
                assert(0 && "invalid  State");
        }
        return "";
}

//  enumerate  for  Token  type
enum class TokenType
{
        INTLTR,  //  int  literal
        PLUS,    //  +
        MINU,    //  -
        MULT,    //  *
        DIV,     //  /
        LPARENT, //  (
        RPARENT, //  )
};
std::string toString(TokenType type)
{
        switch (type)
        {
        case TokenType::INTLTR:
                return "INTLTR";
        case TokenType::PLUS:
                return "PLUS";
        case TokenType::MINU:
                return "MINU";
        case TokenType::MULT:
                return "MULT";
        case TokenType::DIV:
                return "DIV";
        case TokenType::LPARENT:
                return "LPARENT";
        case TokenType::RPARENT:
                return "RPARENT";
        default:
                assert(0 && "invalid  token  type");
        }
        return "";
}

//  definition  of  Token
struct Token
{
        TokenType type;
        std::string value;
};

//  definition  of  DFA
struct DFA
{
        /**
         *  @brief  constructor,  set  the  init  state  to  State::Empty
         */
        DFA();

        /**
         *  @brief  destructor
         */
        ~DFA();

        //  the  meaning  of  copy  and  assignment  for  a  DFA  is  not  clear,  so  we  do  not  allow  them
        DFA(const DFA &) = delete;            //  copy  constructor
        DFA &operator=(const DFA &) = delete; //  assignment

        /**
         *  @brief  take  a  char  as  input,  change  state  to  next  state,  and  output  a  Token  if  necessary
         *  @param[in]  input:  the  input  character
         *  @param[out]  buf:  the  output  Token  buffer
         *  @return    return  true  if  a  Token  is  produced,  the  buf  is  valid  then
         */
        bool next(char input, Token &buf);

        /**
         *  @brief  reset  the  DFA  state  to  begin
         */
        void reset();

private:
        State cur_state;     //  record  current  state  of  the  DFA
        std::string cur_str; //  record  input  characters
};

DFA::DFA() : cur_state(State::Empty), cur_str() {}

DFA::~DFA() {}

//  helper  function,  you  are  not  require  to  implement  these,  but  they  may  be  helpful
bool isoperator(char c)
{
        return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')');
}

TokenType get_op_type(std::string s)
{
        if (s[0] == '-' && s.size() > 1)
        {
                return TokenType::INTLTR;
        }
        switch (s[0])
        {
        case '+':
                return TokenType::PLUS;
                break;
        case '-':
                return TokenType::MINU;
                break;
        case '*':
                return TokenType::MULT;
                break;
        case '/':
                return TokenType::DIV;
                break;
        case '(':
                return TokenType::LPARENT;
                break;
        case ')':
                return TokenType::RPARENT;
                break;
        default:
                return TokenType::INTLTR;
                break;
        }
        return TokenType::RPARENT;
}

bool DFA::next(char input, Token &buf)
{
        if (this->cur_state == State::Empty)
        {
                if (input == '\n')
                {
                        buf.type = get_op_type(this->cur_str);
                        buf.value = this->cur_str;
                        this->cur_state = State::Empty;
                        this->cur_str = "";
                        if (buf.value != "")
                                return true;
                        else
                                return false;
                }
                else if (input == ' ')
                {
                        this->cur_state = State::Empty;
                        this->cur_str = "";
                        return false;
                }
                else if (isoperator(input))
                {
                        // 读取到符号
                        this->cur_state = State::op;
                        this->cur_str = std::string(1, input);
                        return false;
                }
                else
                {
                        this->cur_state = State::IntLiteral;
                        this->cur_str += std::string(1, input);
                        return false;
                }
        }
        else if (this->cur_state == State::IntLiteral)
        {
                if (input == '\n')
                {
                        buf.type = get_op_type(this->cur_str);
                        buf.value = this->cur_str;
                        this->cur_state = State::Empty;
                        this->cur_str = "";
                        if (buf.value != "")
                                return true;
                        else
                                return false;
                }
                else if (input == ' ')
                {
                        buf.type = get_op_type(this->cur_str);
                        buf.value = this->cur_str;
                        this->cur_state = State::Empty;
                        this->cur_str = "";
                        if (buf.value != "")
                                return true;
                        else
                                return false;
                }
                else if (isoperator(input))
                {
                        // 读取到符号
                        buf.type = get_op_type(this->cur_str);
                        buf.value = this->cur_str;
                        this->cur_state = State::op;
                        this->cur_str = std::string(1, input);
                        if (buf.value != "")
                                return true;
                        else
                                return false;
                }
                else
                {
                        this->cur_state = State::IntLiteral;
                        this->cur_str += std::string(1, input);
                        return false;
                }
        }
        else
        {
                if (input == '\n')
                {
                        buf.type = get_op_type(this->cur_str);
                        buf.value = this->cur_str;
                        this->cur_state = State::Empty;
                        this->cur_str = "";
                        if (buf.value != "")
                                return true;
                        else
                                return false;
                }
                else if (input == ' ')
                {
                        buf.type = get_op_type(this->cur_str);
                        buf.value = this->cur_str;
                        this->cur_state = State::op;
                        this->cur_str = "";
                        if (buf.value != "")
                                return true;
                        else
                                return false;
                }
                else if (isoperator(input))
                {
                        // 已经读入了1个符号，再读取到符号
                        // 除了）-，是减法，--，+-(-，此情形都是负号

                        if (input != '-' || (this->cur_str == ")" && input == '-'))
                        {
                                buf.type = get_op_type(this->cur_str);
                                buf.value = this->cur_str;
                                this->cur_state = State::op;
                                this->cur_str = std::string(1, input);
                                if (buf.value != "")
                                        return true;
                                else
                                        return false;
                        }
                        else
                        {
                                buf.type = get_op_type(this->cur_str);
                                buf.value = this->cur_str;
                                this->cur_state = State::IntLiteral;
                                this->cur_str = "-";
                                if (buf.value != "")
                                        return true;
                                else
                                        return false;
                        }
                }
                else
                {
                        buf.type = get_op_type(this->cur_str);
                        buf.value = this->cur_str;
                        this->cur_state = State::IntLiteral;
                        this->cur_str = std::string(1, input);
                        if (buf.value != "")
                                return true;
                        else
                                return false;
                }
        }
}
void DFA::reset()
{
        cur_state = State::Empty;
        cur_str = "";
}

//  hw2
enum class NodeType
{
        TERMINAL, //  terminal  lexical  unit
        EXP,
        NUMBER,
        PRIMARYEXP,
        UNARYEXP, // /
        UNARYOP,  // -
        MULEXP,   // *
        ADDEXP,   // +
        NONE
};
std::string toString(NodeType nt)
{
        switch (nt)
        {
        case NodeType::TERMINAL:
                return "Terminal";
        case NodeType::EXP:
                return "Exp";
        case NodeType::NUMBER:
                return "Number";
        case NodeType::PRIMARYEXP:
                return "PrimaryExp";
        case NodeType::UNARYEXP:
                return "UnaryExp";
        case NodeType::UNARYOP:
                return "UnaryOp";
        case NodeType::MULEXP:
                return "MulExp";
        case NodeType::ADDEXP:
                return "AddExp";
        case NodeType::NONE:
                return "NONE";
        default:
                assert(0 && "invalid  node  type");
                break;
        }
        return "";
}

//  tree  node  basic  class
struct AstNode
{
        int value;
        NodeType type;                   //  the  node  type
        AstNode *parent;                 //  the  parent  node
        std::vector<AstNode *> children; //  children  of  node

        /**
         *  @brief  constructor
         */
        AstNode(NodeType t = NodeType::NONE, AstNode *p = nullptr) : type(t), parent(p), value(0) {}

        /**
         *  @brief  destructor
         */
        virtual ~AstNode()
        {
                for (auto child : children)
                {
                        delete child;
                }
        }

        //  rejcet  copy  and  assignment
        AstNode(const AstNode &) = delete;
        AstNode &operator=(const AstNode &) = delete;
};

//  definition  of  Parser
//  a  parser  should  take  a  token  stream  as  input,  then  parsing  it,  output  a  AST
struct Parser
{
        uint32_t index; //  current  token  index
        const std::vector<Token> &token_stream;

        /**
         *  @brief  constructor
         *  @param  tokens:  the  input  token_stream
         */
        Parser(const std::vector<Token> &tokens) : index(0), token_stream(tokens) {}

        /**
         *  @brief  destructor
         */
        ~Parser() {}

        /**
         *  @brief  creat  the  abstract  syntax  tree
         *  @return  the  root  of  abstract  syntax  tree
         */
        AstNode *get_abstract_syntax_tree()
        {
                int i = 0;
                Token tail;
                tail.type = TokenType::PLUS;
                tail.value = "+";
                std::vector<Token> my_token_stream = this->token_stream;
                my_token_stream.push_back(tail);
                AstNode *root = nullptr;
                initMap();
                for (i; i < my_token_stream.size() - 1;)
                {
                        root = get_abstract_syntax_subTree(i, my_token_stream, root);
                        i = i - 1;
                }
                return root;
        }

        AstNode *get_abstract_syntax_subTree(int &i, std::vector<Token> my_token_stream, AstNode *leftChildren)
        {
                // TODO;
                AstNode *root = new AstNode;
                if (leftChildren != nullptr)
                {
                        root->children.push_back(leftChildren);
                }
                // root->children = new std::vector<AstNode*>;
                for (i; i < my_token_stream.size(); ++i)
                {
                        TokenType type = my_token_stream[i].type;
                        std::string value = my_token_stream[i].value;

                        if (type == TokenType::LPARENT)
                        {
                                // 遇到左括号，递归调用
                                i = i + 1;
                                root->children.push_back(get_abstract_syntax_subTree(i, my_token_stream, nullptr));
                                i=i-2;
                                if(root->type!=NodeType::NUMBER){
                                        root->value=root->children[root->children.size()-1]->children[0]->value;
                                }
                                continue;
                        }
                        
                        if (root->type == NodeType::NUMBER)
                        {
                                // 当根节点已经计算出了结果,栈顶元素>=当前符号优先级,出栈，
                                if (this->proMap[this->typeStack[this->typeStack.size() - 1]] >= this->proMap[type])
                                {
                                        return root;
                                }
                        }
                        else if (type == TokenType::INTLTR)
                        {
                                // 遇到数字
                                int valueInt = changeToint(value);
                                AstNode *node = new AstNode(NodeType::NUMBER, root);
                                node->value = valueInt;
                                root->children.push_back(node);
                        }
                        else
                        {
                                if (root->type == NodeType::NONE)
                                {
                                        // 遇到符号，首先加入符号栈
                                        this->typeStack.push_back(type);
                                        switch (type)
                                        {
                                        case TokenType::PLUS:
                                                root->type = NodeType::ADDEXP;
                                                break;
                                        case TokenType::MINU:
                                                root->type = NodeType::UNARYOP;
                                                break;
                                        case TokenType::MULT:
                                                root->type = NodeType::MULEXP;
                                                break;
                                        case TokenType::DIV:
                                                root->type = NodeType::UNARYEXP;
                                                break;
                                        default:
                                                break;
                                        }
                                }
                                else
                                {
                                        if (this->proMap[this->typeStack[this->typeStack.size() - 1]] < this->proMap[type])
                                        {
                                                // 100 - 3*2 + 5 +,遍历到了*，+<*,先算后面
                                                // this->typeStack.push_back(type);
                                                root->children[1] = get_abstract_syntax_subTree(i, my_token_stream, root->children[1]);
                                                i = i - 2;
                                                continue;
                                        }
                                        // * > -, *=/ 能算
                                        if (root->children.size() == 2)
                                        {
                                                // 如果可以就计算结果
                                                switch (root->type)
                                                {
                                                case NodeType::ADDEXP:
                                                        root->value = root->children[0]->value + root->children[1]->value;
                                                        break;
                                                case NodeType::UNARYOP:
                                                        root->value = root->children[0]->value - root->children[1]->value;
                                                        break;
                                                case NodeType::MULEXP:
                                                        root->value = root->children[0]->value * root->children[1]->value;
                                                        break;
                                                case NodeType::UNARYEXP:
                                                        root->value = root->children[0]->value / root->children[1]->value;
                                                        break;
                                                default:
                                                        break;
                                                }
                                                root->children.clear();
                                                root->type = NodeType::NUMBER;
                                                this->typeStack.pop_back();
                                        }
                                }
                        }
                }
                return root;
        }

        std::map<TokenType, int> proMap;
        void initMap()
        {
                this->proMap.insert({TokenType::RPARENT, 0});
                this->proMap.insert({TokenType::PLUS, 1});
                this->proMap.insert({TokenType::MINU, 1});
                this->proMap.insert({TokenType::MULT, 2});
                this->proMap.insert({TokenType::DIV, 2});
        }
        int changeToint(const std::string &input)
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
                        ans = std::stoi(input.substr(2),0, 2);
                }
                else if (input.size() > 1 && input.substr(0,1) == "0")
                {
                        ans = std::stoi(input.substr(1),0, 8);
                }
                else
                        ans = std::stoi(input);
                return ans;
        }
        // 符号栈
        std::vector<TokenType> typeStack;

        //  u  can  define  member  funcition  of  Parser  here
        // for debug, u r not required to use this
        // how to use this: in ur local enviroment, defines the macro DEBUG_PARSER and add this function in every parse fuction
        void log(AstNode *node)
        {
#ifdef DEBUG_PARSER
                std::cout << "in parse" << toString(node->type) << ", cur_token_type::" << toString(token_stream[index].type) << ", token_val::" << token_stream[index].value << 'n';
#endif
        }
};

//  u  can  define  funcition  here

int main()
{
        std::string stdin_str;
        std::getline(std::cin, stdin_str);
        stdin_str += "\n";
        DFA dfa;
        Token tk;
        std::vector<Token> tokens;
        for (size_t i = 0; i < stdin_str.size(); i++)
        {
                if (dfa.next(stdin_str[i], tk))
                {
                        tokens.push_back(tk);
                }
        }

        //  hw2
        Parser parser(tokens);
        auto root = parser.get_abstract_syntax_tree();
        //  u  may  add  function  here  to  analysis  the  AST,  or  do  this  in  parsing
        //  like  get_value(root);

        std::cout << root->value;

        return 0;
}