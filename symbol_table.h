//
// Created by 吕迪 on 2022/5/19.
//

#ifndef PARSER_SYMBOL_TABLE_H
#define PARSER_SYMBOL_TABLE_H

#include "token.h"
#include "bits/stdc++.h"
/*
    1.符号表的作用：
     符号表是编译程序的一个主要数据结构，符号表通常与编译程序的各个阶段有关，编译程序的各阶段需要不断向符号表输入信息，或者通过它消除二义性。

    具体有下面几个：
    （1）收集符号的属性。
    （2）提供上下文语义的合法性检查的依据。
    （3）作为目标代码生成阶段地址分配的依据。

    2. 组织形式：
    含有的数据结构：
        - 栈式符号表
        - struct符号表体，包含id，名字，类型，维数，声明行，引用行
        - 块索引表，存储新块的第一个数据项指针

    3. 进出策略：
        1. 当出现声明语句时
            - 查重，确定表项位置
            - 插入建立新表项
        2. 处理引用：
            - 查找信息以进行语义信息和代码生成
            - 发现未定义的名字
        3. 定位：
            - 识别出块时开始执行，在大表里面新建一个子表
        4. 重定位：
            - 在块结束时删除子表


 ref-vector相关：
    vector：
        http://c.biancheng.net/view/6749.html
    iterator：
        https://www.jianshu.com/p/8115ea277ecd

 【测试用例说明】
  仅包含常量说明、变量说明、读语句、写语句、赋值语句
  无函数定义及调用，无数组声明及引用。

 【目标代码说明】

 （1）PCODE代码的定义可参见教材《编译技术》P458，张莉等，高等教育出版社，Pascal-S指令代码集，可以进行修改，解释执行程序也可以借鉴Pascal-S编译器源码中的解释执行程序，若PCODE代码修改了请相应修改解释执行程序

 （2）MIPS代码可以选择基础指令及伪指令，不能选择宏指令； MARS 使用 4.5 版本，请下载修改过的版本，关闭延迟槽，内存配置为默认选项
*/

using namespace std;


struct node{
    // 名字。
    // 当且仅当两个node的name和type都一样时才会被检测出重复。
    string name="None";

    // 类型：
    // 1. int,char,string,list1d,list2d
    // 2. function
    string type="None";

    // 二元地址
    int blkn=-1; // 块的嵌套深度
    int offset=-1; // 变量的目标地址偏移量

    int dims=-1;// 维数, -1表示没有维度，1-2表示1-2维

    int params=-1;// 参数数目, -1表示不是函数定义，0-n表示若干个参数
    vector<string> paramTypes;// 参数类型列表

    int declarRow=-1;// 声明行
    vector<int> refRows;// 引用行

    // 重载==
    bool operator ==(const node& l){
        return name == l.name;//5.28 进行了一处修改，只根据变量名判定重复
    }

    node(const string& basicString, const string& basicString1, int i, int i1, int i2, int i3, vector<int> vec) {
        name = basicString;type=basicString1;
        blkn=i;offset=i1;dims=i2;
        declarRow=i3;
        copy(vec.begin(),vec.end(),refRows.begin());
    }

    node(const string& basicString, const string& basicString1) {
        name = basicString;type=basicString1;
    }
};

class SymbolTable{
private:
    // 堆栈式符号表
    // 当遇到变量声明时，将包含变量属性的记录入栈
    // 当到达块结尾时，将该块中声明的所有变量的记录出栈
    vector<node> symbols;

    // 栈顶指针，指向当前有效的栈顶元素
    // vector<node>::iterator top;（X)
    // 直接用symbols.back()就行

    // 块索引表，存储一系列迭代器以指向所有块的开始元素的下标
    vector<int> blockIndexTabs;

    // 检查块内重名
    // flag==0: normally
    // flag==-1: multiple definition
    int checkMultipleName(const node& curnode){
        int flag = 0;
        //如果符号表为空直接通过检查
        if(symbols.empty()) return 0;
        //否则检查重名
        for(auto i = blockIndexTabs.back();i<symbols.size();i++){
            if(symbols[i] == curnode) flag=-1;
        }
        return flag;
    }

public:

    SymbolTable();
    ~SymbolTable();

    // 打印所有已存储符号,测试用
    void check();

    //检查子表中是否有重名变量
    //无，新记录压入栈顶
    //有，报告错误
    int insert(const node& newnode);

    //从栈顶到栈底线性检索
    //在当前子表中找到，局部变量
    //在其他子表中找到，非局部名字
    //实现了最近嵌套作用域原则
    int islocal(const string& name, const string& type);

    //将栈顶指针top的值压入块索引表顶端。
    //块索引表的元素是指针，指向相应块的子表中第一个记录在栈中的位置。（vector里面的iterator）
    void loc();

    //重定位
    //用块索引表顶端元素的值恢复栈顶指针top，完成重定位操作。（pop 直到 *top==curitem）
    //有效地清除刚刚被编译完的块在栈式符号表中的所有记录。
    void reloc();

    //引用符号
    int ref(const string &name, int refrow);

    int refFunc(const string &name, const vector<string> &params, int refrow);

    node searchSymbol(const string &name);
};

#endif //PARSER_SYMBOL_TABLE_H
