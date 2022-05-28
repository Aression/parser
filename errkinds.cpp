//
// Created by Administrator on 2022/5/28 0028.
//

#include <string>
#include "errkinds.h"

std::string ErrDescription[16]={
        "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p"
};

std::string des4dev[16]={
        "illegal symbol",
        "redefinition of name within the same scope",
        "ref of the mysterious name",
        "variable count unmatched in function invoke",
        "variable type unmatched in function invoke",
        "illegal type within condition judgement",
        "redundant return phase in func def without return value",
        "no return phase in funcdef with return value",
        "index of list element can only be integer expression",
        "cannot change const variable's value",
        "should be ; where there is not",
        "should be ) where there is not",
        "should be ] where there is not",
        "list init unmatched in element count",
        "const type unmatched",
        "short of default phase"
};

std::string ERR::meaning(int errKind) {
    return ErrDescription[errKind];
}
