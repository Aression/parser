//
// Created by Administrator on 2022/5/15 0015.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "lexer.h"
#include "symbol_table.h"

using namespace std;

class parser
{
private:
    lexer &lex;
    ofstream &out;

    vector<Token> tokList;
    SymbolTable table;//语法翻译用的符号表

    int tokNum;
    int curIndex;

    // 记录当前块的嵌套深度
    int depth=0;

    // get the next token and output its value
    void getNextToken();

    // Look back at a few tokens
    Token &seekN(int index);

//    void err(string msg, int errCode);
public:

    Token curTok = Token(TokenType::UNREGONIZED, "");// 当前token

    void logerr(const string & msg){
        lex.reminder.report(curTok.position.first,msg);
    }

    parser(lexer &lex, ofstream &out);

    /**
     * 程序
     */
    void parseProgram();

    /**
     * 无符号整数
     */
    int parseUnsignedInteger();

    /**
     * 整数
     */
    void parseInteger();

    /**
     * 字符串
     */
    void parseString();

    /**
     * 常量说明
     */

    void parseConstStmt();

    /**
     * 常量定义
     */
    void parseConstDef();

    /**
     * 声明头部
     */
    string parseDeclHeader();

    /**
     * 变量说明
     */
    void parseVarStmt();

    /**
     * 变量定义
     */
    void parseVarDef();

    /**
     * 有返回值函数定义
     */
    void parseFuncDef();

    /**
     * 无返回值函数定义
     */
    void parseVoidFuncDef();

    /**
     * 有返回值函数调用语句
     */
    void parseFuncCall();

    /**
     * 无返回值函数调用语句
     */
    void parseVoidFuncCall();

    /**
     * 步长
     */
    void parseStep();

    /**
     * 语句
     */
    int parseStmt(const string &returntype, bool &havereturn);

    /**
     * 语句列
     */
    bool parseStmtList(const string &returntype, bool &havereturn);

    /**
     * 复合语句
     */
    bool parseMulStmt(const string &returntype, bool &havereturn);

    /**
     * 赋值语句
     */
    void parseAssignStmt();

    /**
     * 条件语句
     */
    void parseCondStmt(const string &returntype, bool &havereturn);

    /**
     * 条件
     */
    void parseCond();

    /**
     * 循环语句
     */
    void parseLoopStmt(const string &returntype, bool &havereturn);

    /**
     * 值参数表
     */
    void parseValArgList(vector<string> &params);

    /**
     * 读语句
     */
    void parseReadStmt();

    /**
     * 写语句
     */
    void parseWriteStmt();

    /**
     * 返回语句
     */
    void parseReturnStmt(const string &returntype);

    /**
     * 主函数
     */
    void parseMain();

    /**
     * 参数表
     */
    void parseArgList();

    /**
     * 表达式
     */
    string parseExpr();

    /**
     * 项
     */
    string parseItem();

    /**
     * 因子
     */
    string parseFactor();


    void parseState(const string &returntype, bool &havereturn);

    void parseConditionTable(const string &returntype, const string &vartype, bool &havereturn);

    void parseDefault(const string &returntype, bool &havereturn);

    void parseSubConditionPhase(const string &returntype, const string &vartype, bool &havereturn);

    string parseConstVal();

    void checkSymbolTable();

    void ins();

    void refsymbol(const string &name, int refrow);

    void reffunc(const string &name, const vector<string> &params, int refrow);

    void parseSEMICN();

    void parseRPARENT();

    void parseRBRACK();


};

#endif //PARSER_PARSER_H
