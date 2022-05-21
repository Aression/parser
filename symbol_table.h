//
// Created by 吕迪 on 2022/5/19.
//

#ifndef PARSER_SYMBOL_TABLE_H
#define PARSER_SYMBOL_TABLE_H

#include "token.h"
#include "unordered_map"

/*
1.符号表的作用：
 符号表是编译程序的一个主要数据结构，符号表通常与编译程序的各个阶段有关，编译程序的各阶段需要不断向符号表输入信息，或者通过它消除二义性。

具体有下面几个：
（1）收集符号的属性。
（2）提供上下文语义的合法性检查的依据。
（3）作为目标代码生成阶段地址分配的依据。

2.符号表的内容：
 一般来说，符号表的每一项（称为表项）包含两个部分（或称区段、字域），即名字栏和信息栏。

3.符号表的总体组织：
第1种组织结构：按照属性种类完全相同的那些符号组织在一起。
第2种组织结构：把所有语言中的符号都组织在一张符号表中。
第3种组织结构：折中方式。

4.符号表的构建与查找：
- 线性查找；
- 对折查找；
- 杂凑技术（杂凑法是一种争取查表、填表两方面能高速进行的统一技术。 ）。

 设计思路：
 1. 将所有符号组织在一张符号表中
 2. 表项：
    - 名字栏：
    - 信息栏：
        · 符号类型
        · 符号存储类别
        · 符号作用域和可视性
        · 符号变量的存储分配信息
        · 符号的其他属性
 3. 数据结构：
    - 哈希表
 4. 查找技术：
    - 随机映射
*/

using namespace std;

struct node{
    Token token;//符号
    string storageType;//符号存储类别
    string effectArea;//符号作用域和可视性
    string storageAlloc;//符号变量的存储分配信息
};

class SymbolTable{
private:
    unordered_map<string, node> table;
public:
    SymbolTable();
    ~SymbolTable();

    int insert(Token token);
};

#endif //PARSER_SYMBOL_TABLE_H
