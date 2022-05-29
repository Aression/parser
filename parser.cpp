//
// Created by Administrator on 2022/5/15 0015.
//
#include "parser.h"

Token unregonized = Token(TokenType::UNREGONIZED, "");
unordered_map<string, bool> has_retval;

node cursymbol("UNDEF","UNDEF");
static node undefined("UNDEF","UNDEF");

void clearinfo(){
    cursymbol = node("UNDEF","UNDEF");
}

void parser::ins(){
    if(!(cursymbol==undefined)){
        //测试子表中是否已经存在重名变量
        if(!table.insert(cursymbol)){
            logerr(ERR::meaning(errs::b));// 同一作用域内重命名
        }
    }else{
        printf("[ERROR] CANNOT INSERT SYMBOL WITHOUT INIT.");
        exit(-1);
    }
}

void parser::reffunc(const string& name, const vector<string>& params, int refrow){
    int res = table.refFunc(name,params,refrow);
    if(res == 0){
        logerr(ERR::meaning(errs::c));//c 引用了未定义的名字
    }else if(res == -1){
        logerr(ERR::meaning(errs::d));//d 调用实参数目和形参数目不匹配
    }else if(res == -2){
        logerr(ERR::meaning(errs::e));//e 函数参数类型不匹配
    }else {
        //do nothing
    }
}

void parser::refsymbol(const string& name, int refrow){
    if(table.ref(name,refrow)==1){
        // do nothing
    }else{
        logerr(ERR::meaning(errs::c));//c 引用了未定义的名字
    }
}

parser::parser(lexer &lexer, ofstream &out) : lex(lexer), out(out){
    while (true) {
        // get all the offered token.
        Token token = lexer.get_token();
        if (token.type == TokenType::EOFTOK) break;
        tokList.emplace_back(token);
        // 不管这个token是不是可以被识别的token，都把他放进符号列表中。
        // 在后面的处理过程中，根据实际情况要求，原则上碰见了unrecognized的符号就把这一整句都跳掉
//        //out << "valid token position is: line=" << token.position.first << ", col=" << token.position.second << endl;
    }
    tokNum = tokList.size();//读取到的总token数目
    curIndex = 0;//将当前的读到的token的位置存储到index变量中
    table = SymbolTable();//初始化符号表
    depth = 0;//顶层 (全局)
}

//void parser::err(string msg, int errCode) {
//    /* err code:
//     * -1: no error
//     * 1: unsupported token type
//     * 2: SeekN Exceed Range
//     * 3: NextToken Exceed Range
//     */
//    string errPrefix;
//    switch(errCode){
//        case(-1): errPrefix = "Normal Exit";break;
//        case(1): errPrefix = "Unsupported Token Error";break;
//        case(2): errPrefix ="SeekN Exceed Range";break;
//        case(3): errPrefix = "NextToken Exceed Range";break;
//        default: break;
//    }
//    printf("[%s] %s\n",errPrefix.c_str(),msg.c_str());
//    printf("[ERROR] parse failed at the %dth token: [%s]\n", curIndex, curTok.literal.c_str());
//    exit(-1);
//}


Token &parser::seekN(int step) {
    if (curIndex + step >= 0 && curIndex + step - 1 < tokNum)
        return tokList[curIndex + step - 1];
    else {
        unregonized = Token(TokenType::UNREGONIZED, "EXCEED RANGE");
//        err("seekN exceed of range",2);
        return unregonized;
    }
}

void parser::getNextToken() {
    if (curIndex < tokNum) {
        curTok = tokList[curIndex++];
    } else {
        curTok = unregonized;
//        err("getNextToken exceed of range",3);
    }
    // 在更新curTok的同时向文件输出流中写入token的信息[exp3-4屏蔽]
////    out << curTok.get_type() << " " << curTok.literal << endl;
}

/*处理过程：
 * 每个parse函数的处理过程均从第一个字符开始，处理到当前解析语句结束。
 * 比如switch ----> },
 * 调用时curtoken必须是switch， 结束时必须是}
 */


void parser::checkSymbolTable(){
    table.check();
}

// ＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞} ＜主函数＞
void parser::parseProgram() {
    getNextToken();//获得第一个token,此时tokIndex=1

    //常量&变量声明块
    while((curTok.type == TokenType::CONSTTK) ||
    ((curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK)
    && (seekN(2).type == TokenType::SEMICN || seekN(2).type == TokenType::COMMA ||
    seekN(2).type == TokenType::LBRACK || seekN(2).type == TokenType::ASSIGN))){
        //判断是否存在常量说明[将变量压入符号表]
        if (curTok.type == TokenType::CONSTTK) {
            parseConstStmt();
//            checkSymbolTable();
            getNextToken();
        }
        //判断是否存在变量说明
        if ((curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK)
            && (seekN(2).type == TokenType::SEMICN || seekN(2).type == TokenType::COMMA ||
                seekN(2).type == TokenType::LBRACK || seekN(2).type == TokenType::ASSIGN)) {
            parseVarStmt();
//            checkSymbolTable();
            getNextToken();
        }
    }


    // 需要添加递归深度，新建loc等操作。实现在funcdef，voidfuncdef的parse函数里面
    // 函数定义块
    while (((curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) &&
            seekN(2).type == TokenType::LPARENT && seekN(1).type != TokenType::MAINTK)
           || (curTok.type == TokenType::VOIDTK && seekN(2).type == TokenType::LPARENT &&
               seekN(1).type != TokenType::MAINTK)) {
        if (((curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) &&
             seekN(2).type == TokenType::LPARENT)) {
            parseFuncDef();
            getNextToken();//eat 有返回值函数定义
        } else if (curTok.type == TokenType::VOIDTK && seekN(2).type == TokenType::LPARENT) {
            parseVoidFuncDef();
            getNextToken();//eat 无返回值函数定义
        }
//        checkSymbolTable();
        table.reloc();
    }
    parseMain();//分析主函数
//    checkSymbolTable();
    table.reloc();

//    out << "<程序>" << endl;
}

// ＜整数＞ ::= ［＋｜－］＜无符号整数＞
void parser::parseInteger() {
    if (curTok.type == TokenType::PLUS || curTok.type == TokenType::MINU) {
        getNextToken();//eat +\-
        parseUnsignedInteger();
    } else if (curTok.type == TokenType::INTCON) {
        parseUnsignedInteger();
    }
//    out << "<整数>" << endl;
}

// ＜无符号整数＞ ::= ＜非零数字＞｛＜数字＞}
int parser::parseUnsignedInteger() {
    if (curTok.type == TokenType::INTCON) {
//        out << "<无符号整数>" << endl;
        return stoi(curTok.literal);
    }
    return -1;//-1表示不符合无符号整数
}

// ＜字符串＞ ::= "｛十进制编码为32,33,35-126的ASCII字符｝"
void parser::parseString() {
    if (curTok.type == TokenType::STRCON) {
//        out << "<字符串>" << endl;
    }
}

// ＜常量说明＞ ::= const＜常量定义＞;{ const＜常量定义＞;}
void parser::parseConstStmt() {
    getNextToken();//eat const
    parseConstDef();
    getNextToken();
    while (seekN(1).type == TokenType::CONSTTK) {
        getNextToken();
        getNextToken();//eat "const"
        parseConstDef();
        getNextToken();
    }
//    out << "<常量说明>" << endl;
}


//  ＜常量定义＞ ::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
//               | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void parser::parseConstDef() {
    clearinfo();
//    int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
    if (curTok.type == TokenType::INTTK) {
        getNextToken();//eat int

        cursymbol.name=curTok.literal;
        cursymbol.type="Const Integer";
        cursymbol.blkn=depth;
        cursymbol.dims=-1;
        cursymbol.declarRow=curTok.position.first;
        ins();

        getNextToken();//eat 标识符
        getNextToken();//eat =
        parseInteger();
        while (seekN(1).type == TokenType::COMMA) {
            getNextToken();
            getNextToken();// ,

            cursymbol.name=curTok.literal;
            cursymbol.type="Const Integer";
            cursymbol.blkn=depth;
            cursymbol.dims=-1;
            cursymbol.declarRow=curTok.position.first;
            ins();

            getNextToken();//eat 标识符
            getNextToken();//eat =
            parseInteger();
        }

    }
//    char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
    else if (curTok.type == TokenType::CHARTK) {
        getNextToken();//eat char

        cursymbol.name=curTok.literal;
        cursymbol.type="Const Char";
        cursymbol.blkn=depth;
        cursymbol.dims=-1;
        cursymbol.declarRow=curTok.position.first;
        ins();

        getNextToken();//eat 标识符
        getNextToken();//eat =
        while (seekN(1).type == TokenType::COMMA) {
            getNextToken();//eat 字符
            getNextToken();//eat ,

            cursymbol.name=curTok.literal;
            cursymbol.type="Const Char";
            cursymbol.blkn=depth;
            cursymbol.dims=-1;
            cursymbol.declarRow=curTok.position.first;
            ins();

            getNextToken();//eat 标识符
            getNextToken();//eat =
        }
    }
//    out << "<常量定义>" << endl;
}

// ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
void parser::parseVarStmt() {
    parseVarDef();
    parseSEMICN();
    while ((seekN(1).type == TokenType::INTTK || seekN(1).type == TokenType::CHARTK) &&
           seekN(3).type != TokenType::LPARENT/*区别声明头和变量声明*/) {
        getNextToken();//eat ;
        parseVarDef();
        parseSEMICN();
    }
//    out << "<变量说明>" << endl;
}

string parser::parseConstVal(){
    if(curTok.type == TokenType::CHARCON) {
//        out << "<常量>" << endl;
        return "char";
    }else{
        parseInteger();
//        out << "<常量>" << endl;
        return "int";
    }

}

//＜变量定义＞ ::= ＜变量定义无初始化＞|＜变量定义及初始化＞
void parser::parseVarDef() {
    string types[2] = {"<变量定义无初始化>", "<变量定义及初始化>"};
    int type = 0;

    string vartype;
    if(curTok.type==INTTK){
        vartype = "Integer Variable";
    }else if(curTok.type==CHARTK){
        vartype = "Char Variable";
    }

    getNextToken();//标识符

    clearinfo();
    cursymbol.name=curTok.literal;
    cursymbol.blkn=depth;
    cursymbol.declarRow=curTok.position.first;

    int dx=-1,dy=-1;//-1:没有这一维度;n:在这一维度上取值为n

    // 声明部分，这一部分负责插入符号
    if (seekN(1).type == TokenType::LBRACK) {
        getNextToken();//[
        getNextToken();//num
        dx = parseUnsignedInteger();//处理无符号整数
        parseRBRACK();//]
        if (seekN(1).type == TokenType::LBRACK){
            getNextToken();//[
            getNextToken();//num
            dy = parseUnsignedInteger();//处理无符号整数
            parseRBRACK();//]
            cursymbol.type=vartype + " list2d";
            cursymbol.dims=2;
            ins();
        }else{
            cursymbol.type=vartype + " list1d";
            cursymbol.dims=1;
            ins();
        }
    }else{
        cursymbol.type=vartype;
        cursymbol.dims=-1;
        ins();
    }

    while (seekN(1).type == TokenType::COMMA) {
        getNextToken();//现在是逗号
        getNextToken();//现在是标识符

        cursymbol.name=curTok.literal;
        cursymbol.blkn=depth;
        cursymbol.declarRow=curTok.position.first;

        if (seekN(1).type == TokenType::LBRACK) {
            getNextToken();//[
            getNextToken();//num
            parseUnsignedInteger();//处理无符号整数
            getNextToken();//]
            if (seekN(1).type == TokenType::LBRACK){
                getNextToken();//[
                getNextToken();//num
                parseUnsignedInteger();//处理无符号整数
                getNextToken();//]
                cursymbol.type=vartype + " list2d";
                cursymbol.dims=2;
                ins();
            }else{
                cursymbol.type=vartype + " list1d";
                cursymbol.dims=1;
                ins();
            }
        }
    }

    if (seekN(1).type == TokenType::ASSIGN) {
        type = 1;
        int rows=0, cols=0;
        string valtype;
        //处理赋值部分
        getNextToken();//现在是=号
        if(seekN(1).type==TokenType::LBRACE && seekN(2).type==TokenType::LBRACE){
            rows++;
            getNextToken();//现在是内侧第一个{
            getNextToken();//现在是内侧第二个{
            getNextToken();//现在是内侧第一个常量
            valtype = parseConstVal();
            if((valtype == "int" && vartype == "Char Variable") ||
                    (valtype == "char" && vartype == "Integer Variable")){
                //声明与实例化类别不一致
                logerr(ERR::meaning(errs::o));//常量类别不一致
            }
            cols++;
            while(seekN(1).type == TokenType::COMMA){
                getNextToken();//现在是逗号
                getNextToken();//现在是常量
                valtype = parseConstVal();
                if((valtype == "int" && vartype == "Char Variable") ||
                   (valtype == "char" && vartype == "Integer Variable")){
                    //声明与实例化类别不一致
                    logerr(ERR::meaning(errs::o));//常量类别不一致
                }
                cols++;
            }
            if(cols!=dy){
                logerr(ERR::meaning(errs::n));//cols不匹配
            }
            cols=0;
            getNextToken();//现在是内侧对应的}
            while(seekN(1).type == TokenType::COMMA){
                rows++;
                getNextToken();//现在是逗号
                getNextToken();//现在是内侧第二个{
                getNextToken();//现在是内侧第一个常量
                valtype = parseConstVal();
                if((valtype == "int" && vartype == "Char Variable") ||
                   (valtype == "char" && vartype == "Integer Variable")){
                    //声明与实例化类别不一致
                    logerr(ERR::meaning(errs::o));//常量类别不一致
                }
                cols++;
                while(seekN(1).type == TokenType::COMMA){
                    getNextToken();//现在是逗号
                    getNextToken();//现在是常量
                    valtype = parseConstVal();
                    if((valtype == "int" && vartype == "Char Variable") ||
                       (valtype == "char" && vartype == "Integer Variable")){
                        //声明与实例化类别不一致
                        logerr(ERR::meaning(errs::o));//常量类别不一致
                    }
                    cols++;
                }
                if(cols!=dy){
                    logerr(ERR::meaning(errs::n));//cols不匹配
                }
                getNextToken();//现在是内侧对应的}
            }
            if(rows!=dx){
                logerr(ERR::meaning(errs::n));//rows不匹配
            }
            rows=0;
            getNextToken();//现在是外侧对应的}
        }
        else if(seekN(1).type==TokenType::LBRACE){
            getNextToken();//外侧{
            getNextToken();//常量
            valtype = parseConstVal();
            if((valtype == "int" && vartype == "Char Variable") ||
               (valtype == "char" && vartype == "Integer Variable")){
                //声明与实例化类别不一致
                logerr(ERR::meaning(errs::o));//常量类别不一致
            }
            rows++;
            while(seekN(1).type == TokenType::COMMA){
                getNextToken();//逗号
                getNextToken();//常量
                valtype = parseConstVal();
                if((valtype == "int" && vartype == "Char Variable") ||
                   (valtype == "char" && vartype == "Integer Variable")){
                    //声明与实例化类别不一致
                    logerr(ERR::meaning(errs::o));//常量类别不一致
                }
                rows++;
            }
            if(rows!=dx){
                logerr(ERR::meaning(errs::n));//rows不匹配
            }
            rows=0;
            getNextToken();//外侧}
        }
        else{
            getNextToken();//常量
            valtype = parseConstVal();
            if((valtype == "int" && vartype == "Char Variable") ||
               (valtype == "char" && vartype == "Integer Variable")){
                //声明与实例化类别不一致
                logerr(ERR::meaning(errs::o));//常量类别不一致
            }
        }
    }else if(seekN(1).type==TokenType::SEMICN){
        //普通的变量声明，没有初始化
        type = 0;
    }
//    out << types[type] << endl;
//    out << "<变量定义>" << endl;
}

// ＜声明头部＞ ::= int＜标识符＞
//              |char＜标识符＞
string parser::parseDeclHeader() {
    clearinfo();
    string returntype;
    if(curTok.type==TokenType::INTTK){
        cursymbol.type = "Function With Integer Return Value";
        returntype = "int";
    }else if(curTok.type==TokenType::CHARTK){
        cursymbol.type="Function With Char Return Value";
        returntype = "char";
    }
    getNextToken();//eat int/char
    cursymbol.name=curTok.literal;
    cursymbol.blkn=depth;
    cursymbol.declarRow=curTok.position.first;
    cursymbol.dims=-1;//暂时不插入，后面还要补充参数表信息
    has_retval.insert(pair<string, bool>(curTok.literal, true));//记录当前函数类型: 有返回值
//    out << "<声明头部>" << endl;
    return returntype;
}

// ＜有返回值函数定义＞ ::= ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void parser::parseFuncDef() {
    string returntype = parseDeclHeader();

    getNextToken();//eat 声明头部
    if (seekN(1).type!=TokenType::RPARENT){
        getNextToken();//eat (
        parseArgList();
//        getNextToken();//eat 参数表
    }else{
//        out << "<参数表>" << endl;
//        getNextToken();//eat (
    }

    parseRPARENT();// )
    getNextToken();// {
    getNextToken();//现在是mulstmt里面第一个token
    parseMulStmt(returntype);
    getNextToken();//eat 复合语句
//    out << "<有返回值函数定义>" << endl;
    depth--;
}

// ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void parser::parseVoidFuncDef() {
    getNextToken();//eat void
    has_retval.insert(pair<string, bool>(curTok.literal, false));//记录当前函数类型: 无返回值

    cursymbol.type="Function Without Return Value";
    cursymbol.name=curTok.literal;
    cursymbol.blkn=depth;
    cursymbol.declarRow=curTok.position.first;
    cursymbol.dims=-1;

    getNextToken();//eat 标识符
    if (seekN(1).type!=TokenType::RPARENT){
        getNextToken();//eat (
        parseArgList();
//        getNextToken();//eat 参数表
    }else{
//        out << "<参数表>" << endl;
//        getNextToken();//eat (
        cursymbol.params=0;
        ins();
        table.loc();
        depth++;
    }

    parseRPARENT();// )
    getNextToken();// {
    getNextToken();//现在是mulstmt里面第一个token
    parseMulStmt("void");
    getNextToken();//eat 复合语句
//    out << "<无返回值函数定义>" << endl;
    depth--;
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void parser::parseFuncCall() {
    //对于函数调用的处理类似于函数声明的处理，不过这里是处理标识符和值参数表

    //函数调用语句首先应该调用refsymbol
    string name = curTok.literal;
    int row = curTok.position.first;
    vector<string> params;

    getNextToken();//eat 标识符
    if (seekN(1).type!=TokenType::RPARENT){
        getNextToken();//eat (
        parseValArgList(params);

        //引用function
        reffunc(name,params,row);

        parseRPARENT();//eat 值参数表
    }else{
        out<<"<值参数表>"<<endl;
        parseRPARENT();//eat (
    }
//    out << "<有返回值函数调用语句>" << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void parser::parseVoidFuncCall() {

    //函数调用语句首先应该调用refsymbol
    string name = curTok.literal;
    int row = curTok.position.first;
    vector<string> params;

    getNextToken();//eat 标识符
    if (seekN(1).type!=TokenType::RPARENT){
        getNextToken();//eat (
        parseValArgList(params);

        //引用function
        reffunc(name,params,row);

        parseRPARENT();//eat 值参数表
    }else{
        out<<"<值参数表>"<<endl;
        parseRPARENT();//eat (
    }
//    out << "<无返回值函数调用语句>" << endl;
}

//＜步长＞ ::= ＜无符号整数＞
void parser::parseStep() {
    parseUnsignedInteger();
//    out << "<步长>" << endl;
}

void parser::parseSEMICN(){
    if(seekN(1).type == TokenType::SEMICN){
        getNextToken();//eat ;
    }else{
        logerr(ERR::meaning(errs::k));
    }
}

void parser::parseRPARENT(){
    if(seekN(1).type == TokenType::RPARENT){
        getNextToken();//eat )
    }else{
        logerr(ERR::meaning(errs::l));
    }
}

void parser::parseRBRACK(){
    //todo 检验右中括号
    if(seekN(1).type == TokenType::RBRACK){
        getNextToken();//eat )
    }else{
        logerr(ERR::meaning(errs::m));
    }
}

/*  ＜循环语句＞1
 * ｜＜条件语句＞1
 * | ＜有返回值函数调用语句＞;1
 * |＜无返回值函数调用语句＞;1
 * ｜＜赋值语句＞;1
 * ｜＜读语句＞;1
 * ｜＜写语句＞;1
 * ｜＜情况语句＞1
 * ｜＜空＞;1
 * |＜返回语句＞;1
 * | '{'＜语句列＞'}'1
 */
int parser::parseStmt(const string &returntype) {
    //parsestmt返回int标识读取到了啥语句

    // 1: return stmt
    // -1: unrecognized stmt
    int stmttype=-1;

    //条件语句
    if (curTok.type == TokenType::IFTK) {
        parseCondStmt(returntype);
    //循环语句
    } else if (curTok.type == TokenType::WHILETK || curTok.type == TokenType::FORTK) {
        parseLoopStmt(returntype);
        //评测机文法规则有问题
        if (seekN(1).type == TokenType::SEMICN){
            out<<"<语句>"<<endl;
            getNextToken();//eat )
        }
    //语句列
    } else if (curTok.type == TokenType::LBRACE) {
        getNextToken();//eat {
        parseStmtList(returntype);
        getNextToken();//eat 语句列
    //有\无返回值函数调用
    } else if (curTok.type == TokenType::IDENFR) {
        //有返回值函数
        if (has_retval[curTok.literal]) {
            parseFuncCall();
            parseSEMICN();
        } else if (seekN(1).type == TokenType::ASSIGN || seekN(1).type == TokenType::LBRACK) {
            //赋值语句
            parseAssignStmt();
            parseSEMICN();
        //无返回值函数
        } else if (!has_retval[curTok.literal]) {
            parseVoidFuncCall();
            parseSEMICN();
        }
    //读语句
    } else if (curTok.type == TokenType::SCANFTK) {
        parseReadStmt();
        parseSEMICN();
    //写语句
    } else if (curTok.type == TokenType::PRINTFTK) {
        parseWriteStmt();
        parseSEMICN();
    //返回语句
    } else if (curTok.type == TokenType::RETURNTK) {
        parseReturnStmt(returntype);
        stmttype=1;
        parseSEMICN();
    //空语句（只有一个分号）
    } else if (curTok.type == TokenType::SEMICN){
//        out << "<空>" << endl;
    //情况语句，switch
    } else if (curTok.type == TokenType::SWITCHTK){
        parseState(returntype);
    } else{
//        err("parse <phase> failed!",1);
    }
//    out << "<语句>" << endl;
    return stmttype;
}

//＜语句列＞ ::= ｛＜语句＞｝
void parser::parseStmtList(const string & returntype) {
    bool havereturn= false;
    if (curTok.type != TokenType::RBRACE) {
        while (seekN(1).type != TokenType::RBRACE&&curIndex<tokNum) {

            if(parseStmt(returntype)==1){
                havereturn=true;
            }

            if (seekN(1).type != TokenType::RBRACE&&curIndex<tokNum) {
                getNextToken();
            }
        }
    }
    if(!havereturn && returntype != "void"){
        logerr(ERR::meaning(errs::h));
    }
//    out << "<语句列>" << endl;
}

//＜复合语句＞ ::= ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void parser::parseMulStmt(const string &returntype) {
    while( (curTok.type == TokenType::CONSTTK)||(curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK)){
        //［＜常量说明＞］
        if (curTok.type == TokenType::CONSTTK) {
            parseConstStmt();
            getNextToken();
        }
        //［＜变量说明＞］
        else if (curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) {
            parseVarStmt();
            getNextToken();
        }
    }
    parseStmtList(returntype);
//    out << "<复合语句>" << endl;
}

//＜赋值语句＞ ::= ＜标识符＞＝＜表达式＞
//              |＜标识符＞'['＜表达式＞']'=＜表达式＞
//              |＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞
void parser::parseAssignStmt() {
    string symboltype = table.searchSymbol(curTok.literal).type;
    if(symboltype == "Const Integer" || symboltype == "Const Char"){
        logerr(ERR::meaning(errs::j));//不能为常量重新赋值
    }
    getNextToken();//eat 标识符
    //＜标识符＞＝＜表达式＞
    if (curTok.type == TokenType::ASSIGN) {
        getNextToken();//eat =
        parseExpr();
    }
    //＜标识符＞'['＜表达式＞']'=＜表达式＞
    else if(curTok.type == TokenType::LBRACK){
        getNextToken();//eat [
        if(parseExpr()!="int"){
            logerr(ERR::meaning(errs::i));
        }
        parseRBRACK();//eat 表达式
        getNextToken();//eat ]
        //|＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞
        if(curTok.type == TokenType::LBRACK){
            getNextToken();//eat [
            if(parseExpr()!="int"){
                logerr(ERR::meaning(errs::i));
            }
            parseRBRACK();//eat 表达式
            getNextToken();//eat ]
        }
        getNextToken();//eat =
        parseExpr();
    }
//    out << "<赋值语句>" << endl;
}

//＜条件语句＞ ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
void parser::parseCondStmt(const string &returntype) {
    //条件判断中的条件只能有整形
    getNextToken();//eat if
    getNextToken();//eat (
    parseCond();
    parseRPARENT();//eat 条件
    getNextToken();//eat )
    parseStmt("void");
    if (seekN(1).type == TokenType::ELSETK) {
        getNextToken();//eat 语句
        getNextToken();//eat else
        parseStmt("void");
    }
//    out << "<条件语句>" << endl;
}

//＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞ //整型表达式之间才能进行关系运算
//          ｜＜表达式＞    //表达式为整型，其值为0条件为假，值不为0时条件为真
void parser::parseCond() {
    if(parseExpr() != "int"){
        logerr(ERR::meaning(errs::f));
    }
    if (seekN(1).type == TokenType::LSS
        || seekN(1).type == TokenType::LEQ
        || seekN(1).type == TokenType::GRE
        || seekN(1).type == TokenType::GEQ
        || seekN(1).type == TokenType::EQL
        || seekN(1).type == TokenType::NEQ) {
        getNextToken();//eat 表达式
        getNextToken();//eat 关系运算符
        if(parseExpr() != "int"){
            logerr(ERR::meaning(errs::f));
        }
    }
//    out << "<条件>" << endl;
}

//＜循环语句＞ ::= while '('＜条件＞')'＜语句＞
//        |for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
void parser::parseLoopStmt(const string &returntype) {
//    while '('＜条件＞')'＜语句＞
    if (curTok.type == TokenType::WHILETK) {
        getNextToken();//eat while
        getNextToken();//eat (
        parseCond();
        parseRPARENT();//eat 条件
        getNextToken();//eat )
        parseStmt("void");
    }
//    for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
    else if (curTok.type == TokenType::FORTK) {
        getNextToken();//eat for
        getNextToken();//eat (
        getNextToken();//eat 标识符
        getNextToken();//eat =
        parseExpr();
        parseSEMICN();
        getNextToken();//eat ;
        parseCond();
        parseSEMICN();//eat cond
        getNextToken();//eat ;
        getNextToken();//eat 标识符
        getNextToken();//eat =
        getNextToken();//eat 标识符
        getNextToken();//eat +/-
        parseStep();
        parseRPARENT();//eat )
        parseStmt("void");
    }
//    out << "<循环语句>" << endl;
}

//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void parser::parseValArgList(vector<string> & params) {
    // 如果不为空
    if (curTok.type != TokenType::RPARENT) {
        params.emplace_back(parseExpr());//添加参数类型
        while (seekN(1).type == TokenType::COMMA) {
            getNextToken();//eat 表达式
            getNextToken();//eat ,
            params.emplace_back(parseExpr());
        }
    }
//    out << "<值参数表>" << endl;
}

//＜读语句＞ ::= scanf '('＜标识符＞{,＜标识符＞}')'
void parser::parseReadStmt() {
    getNextToken();//eat scanf
    getNextToken();//eat (
    while (seekN(1).type == TokenType::COMMA) {
        string symboltype = table.searchSymbol(curTok.literal).type;
        if(symboltype == "Const Integer" || symboltype == "Const Char"){
            logerr(ERR::meaning(errs::j));//不能为常量重新赋值
        }
        getNextToken();//eat 标识符
        getNextToken();//eat ,
    }
    string symboltype = table.searchSymbol(curTok.literal).type;
    if(symboltype == "Const Integer" || symboltype == "Const Char"){
        logerr(ERR::meaning(errs::j));//不能为常量重新赋值
    }
    parseRPARENT();//eat 标识符
//    out << "<读语句>" << endl;
}

//＜写语句＞ ::= printf '(' ＜字符串＞,＜表达式＞ ')'
//            | printf '('＜字符串＞ ')'
//            | printf '('＜表达式＞')'
void parser::parseWriteStmt() {
    getNextToken();//eat printf
    getNextToken();//eat (
    if (curTok.type == TokenType::STRCON) {
        //printf '(' ＜字符串＞,＜表达式＞ ')'
        if (seekN(1).type == TokenType::COMMA) {
            parseString();
            getNextToken();//eat 字符串
            getNextToken();//eat ,
            parseExpr();
            parseRPARENT();//eat 表达式
        }
            // printf '('＜字符串＞ ')'
        else {
            parseString();
            parseRPARENT();//eat 字符串
        }
    }
    // printf '('＜表达式＞')'
    else {
        parseExpr();
        parseRPARENT();//eat 表达式
    }
//    out << "<写语句>" << endl;
}

//＜返回语句＞ ::= return['('＜表达式＞')']
void parser::parseReturnStmt(const string &returntype) {
    if(returntype == "void" ){
        //无返回值而有返回语句，则必须是 return;
        if (seekN(1).type == TokenType::LPARENT && seekN(2).type == TokenType::RPARENT){
            //return ()
            logerr(ERR::meaning(errs::g));
            getNextToken();//eat return
            parseRPARENT();//eat (
        } else if(seekN(1).type == TokenType::LPARENT && seekN(2).type != TokenType::RPARENT){
            //return (?
            //1. return ( expr )
            //2. return ( NAN )
            logerr(ERR::meaning(errs::g));
            getNextToken();//eat return
            getNextToken();//eat (
            parseExpr();
            parseRPARENT();
        }//else pass, "return;" is legal
    }
    else if(returntype == "int" || returntype == "char"){

        //必须是 return (expr);
        if (seekN(1).type == TokenType::LPARENT && seekN(2).type == TokenType::RPARENT){
            //return ()
            logerr(ERR::meaning(errs::h));
            getNextToken();//eat return
            getNextToken();//eat (
            parseRPARENT();//eat )
        } else if(seekN(1).type == TokenType::LPARENT && seekN(2).type != TokenType::RPARENT){
            //1. return ( ;
            //2. return ( expr ;
            //3. return ( expr ) ;
            getNextToken();//eat return
            if(seekN(1).type != TokenType::SEMICN){// 2,3
                getNextToken();//eat (
                if(parseExpr() != returntype){
                    logerr(ERR::meaning(errs::h));//return类型不一致
                }
            }//else 1
            parseRPARENT();
        }else{
            //直接结束了，是return; 不符合形式
            logerr(ERR::meaning(errs::h));
        }
    }

    //如果返回不为空
    if (seekN(1).type == TokenType::LPARENT) {
        getNextToken();// eat return
        getNextToken();// eat (
        parseExpr();
        getNextToken();//eat 表达式
    }
//    out << "<返回语句>" << endl;
}

//＜主函数＞ ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
void parser::parseMain() {
    depth++;
    table.loc();
    getNextToken();//eat void
    getNextToken();//eat main
    parseRPARENT();//eat (, now )
    getNextToken();//eat )
    getNextToken();//eat {
    parseMulStmt("void");//处理复合语句
    getNextToken();//eat 复合语句
//    out << "<主函数>" << endl;
    depth--;
}

//＜参数表＞ ::= ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}
//            | ＜空＞
void parser::parseArgList() {
    //＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}
    string type;
    vector<pair<string,string>> args;
    if (curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) {
        cursymbol.paramTypes.push_back(curTok.literal);
        type=curTok.literal;
        getNextToken();//eat 类型标识符, 现在是标识符名称
        args.emplace_back(type,curTok.literal);

        while (seekN(1).type == TokenType::COMMA) {
            getNextToken();//eat 标识符
            getNextToken();//eat , 现在是类型标识符

            cursymbol.paramTypes.push_back(curTok.literal);
            type=curTok.literal;
            getNextToken();//eat 类型标识符，现在是标识符名称
            args.emplace_back(type,curTok.literal);
        }
        cursymbol.params=args.size();
        ins();//处理完毕，插入定义好的函数
        table.loc();
        depth++;

        //插入参数表中定义的变量
        for(auto & item : args){
            clearinfo();
            cursymbol.type = item.first;
            cursymbol.name = item.second;
            cursymbol.blkn = depth;
            cursymbol.declarRow = curTok.position.first;
            ins();
        }
    }else {
//        out << "<空>" << endl;
    }
//    out << "<参数表>" << endl;
}

//＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   //[+|-]只作用于第一个<项>
string parser::parseExpr() {
    if (curTok.type == TokenType::PLUS || curTok.type == TokenType::MINU) {
        getNextToken(); //eat +/-
    }
    string type = parseItem();
    while (seekN(1).type == TokenType::PLUS || seekN(1).type == TokenType::MINU) {
        getNextToken();//eat 项
        getNextToken();//eat 加法运算符
        parseItem();
    }
//    out << "<表达式>" << endl;
    return type;// 表达式的类型与第一个因子类型一样
}

//＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
string parser::parseItem() {
    string type = parseFactor();
    while (seekN(1).type == TokenType::MULT || seekN(1).type == TokenType::DIV) {
        getNextToken();//eat 因子
        getNextToken();//eat 乘法运算符
        parseFactor();
    }
//    out << "<项>" << endl;
    return type;// 项的类型与第一个因子类型一样
}

//＜因子＞ ::= ＜标识符＞
//          ｜＜标识符＞'['＜表达式＞']'
//          |'('＜表达式＞')'
//          ｜＜整数＞
//          |＜字符＞
//          ｜＜有返回值函数调用语句＞
string parser::parseFactor() {
    // 因子里面存在对于现有变量的引用
    string type;
    string returntype = "NAN";
    if (curTok.type == TokenType::IDENFR) {
        // 函数调用
        if (has_retval[curTok.literal]) {
            type = table.searchSymbol(curTok.literal).type;
            parseFuncCall();//有返回值函数调用语句
            if(type == "Function With Integer Return Value"){
                returntype="int";
            }else if(type == "Function With Char Return Value"){
                returntype="char";
            }
        }
        else if (seekN(1).type == TokenType::LBRACK) {
            //数组元素，1d和2d
            type = table.searchSymbol(curTok.literal).type;
            refsymbol(curTok.literal,curTok.position.first);
            if(type == "Integer Variable list1d" || type == "Integer Variable list2d"){
                returntype = "int";
            }else if(type == "Char Variable list1d"|| type == "Char Variable list2d"){
                returntype = "char";
            }
            getNextToken();//eat 标识符
            getNextToken();//eat [
            if(parseExpr()!="int"){
                logerr(ERR::meaning(errs::i));
            }
            parseRBRACK();//eat 表达式
            if(seekN(1).type == TokenType::LBRACK){
                //还有一个brack组，说明是从二维数组里面提
                getNextToken();//eat ]
                getNextToken();//eat [
                if(parseExpr()!="int"){
                    logerr(ERR::meaning(errs::i));
                }
                parseRBRACK();//eat 表达式
            }
        } else{
            //否则就是个单纯的标识符
            returntype = table.searchSymbol(curTok.literal).type;

            if(returntype == "Integer Variable"){
                returntype = "int";
            }else if(returntype == "Char Variable"){
                returntype = "char";
            }

            refsymbol(curTok.literal,curTok.position.first);
        }

    } else if (curTok.type == TokenType::LPARENT) {
        getNextToken();//eat (
        returntype = parseExpr();
        parseRPARENT();
    } else if (curTok.type == TokenType::INTCON || curTok.type == TokenType::PLUS || curTok.type == TokenType::MINU) {
        parseInteger();
        returntype = "int";
    } else if (curTok.type == TokenType::CHARCON) {
        returntype = "char";
    }
//    out << "<因子>" << endl;
    return returntype;
}

//switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’
void parser::parseState(const string &returntype) {
    bool havedefault = false;
    if (seekN(1).type == TokenType::LPARENT){
        getNextToken();//现在是第一个左括号
        getNextToken();//进入表达式第一个标识符
        string type = parseExpr();//解析表达式
        parseRPARENT();//现在是第二个右括号
        if(seekN(1).type == TokenType::LBRACE){
            getNextToken();//现在是左边第一个大括号
            if(seekN(1).type == TokenType::CASETK){
                getNextToken();//现在是case
                parseConditionTable(returntype,type);
            }
            //todo 匹配每一个switch和default
            if(seekN(1).type == TokenType::DEFAULTTK){
                getNextToken();//现在是default
                parseDefault(returntype);
                havedefault=true;
            }
            getNextToken();//现在是右边第二个大括号
            if(!havedefault){
                logerr(ERR::meaning(errs::p));//switch没有匹配的default
            }
//            out << "<情况语句>" << endl;
        }else{
//            err("Failed to parse switch phase's condition table.", 1);
        }
    }
    else{
//        err("Failed to parse switch phase.", 1);
    }

}

void parser::parseConditionTable(const string &returntype, const string &vartype) {
    parseSubConditionPhase(returntype, vartype);
    while(seekN(1).type == TokenType::CASETK){
        getNextToken();
        parseSubConditionPhase(returntype, vartype);
    }
//    out << "<情况表>" << endl;
}

void parser::parseSubConditionPhase(const string &returntype, const string &vartype) {
    getNextToken();//现在是常量
    if(parseConstVal() != vartype){
        logerr(ERR::meaning(errs::o));//常量类型不一致
    }
    getNextToken();//现在是冒号
    getNextToken();//现在是语句第一个字符
    parseStmt(returntype);//现在是语句
//    out << "<情况子语句>" << endl;
}

void parser::parseDefault(const string &returntype) {
    getNextToken();//现在是冒号
    getNextToken();//现在是语句第一个字符
    parseStmt(returntype);//解析语句
//    out << "<缺省>" << endl;
}


