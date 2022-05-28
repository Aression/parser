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
    void parseUnsignedInteger();

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
    void parseDeclHeader();

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
    void parseStmt();

    /**
     * 语句列
     */
    void parseStmtList();

    /**
     * 复合语句
     */
    void parseMulStmt();

    /**
     * 赋值语句
     */
    void parseAssignStmt();

    /**
     * 条件语句
     */
    void parseCondStmt();

    /**
     * 条件
     */
    void parseCond();

    /**
     * 循环语句
     */
    void parseLoopStmt();

    /**
     * 值参数表
     */
    void parseValArgList();

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
    void parseReturnStmt();

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
    void parseExpr();

    /**
     * 项
     */
    void parseItem();

    /**
     * 因子
     */
    void parseFactor();


    void parseState();

    void parseConditionTable();

    void parseDefault();

    void parseSubConditionPhase();

    void parseConstVal();

    void checkSymbolTable();

    void ins();

    void refsymbol(const string &name, const string &type, const int refrow);
};

#endif //PARSER_PARSER_H
