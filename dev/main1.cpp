#include <bits/stdc++.h>
#include "file_reader.h"
#include "lexer.h"
#include "parser.h"


int main(int argc, char const *argv[])
{
    FileReader reader("testfile.txt");
    lexer lex(&reader, cerr);
    std::ofstream out("output.txt");
    parser parse(lex,out);
    parse.parseProgram();
    return 0;
}