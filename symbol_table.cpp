//
// Created by 吕迪 on 2022/5/19.
//

#include "symbol_table.h"
#include <utility>

SymbolTable::SymbolTable() {
    // 定位到全局表
    loc();
}

SymbolTable::~SymbolTable() {
    // 清除全局变量
    reloc();
}

//检查子表中是否有重名变量
//无，新记录压入栈顶, return 1
//有，报告错误, return 0
int SymbolTable::insert(const node& newnode){
    // 如果是空表则直接插入
    if(symbols.empty()){
        symbols.push_back(newnode);
        return 1;
    }
    // 检查重名
    if(checkMultipleName(newnode) == 0){
        // 正常，没有重复命名
        symbols.push_back(newnode);
    }else{
        return 0;
    }
    return 1;
}

//从栈顶到栈底线性检索;实现了最近嵌套作用域原则
//在当前子表中找到，局部变量, return 1
//在其他子表中找到，非局部变量, return 块深度
//没找到，return -1
int SymbolTable::islocal(const string& name, const string& type){
    node tmpnode = node(name,type);
    auto blockCheck = blockIndexTabs.back();
    for(auto iter = symbols.begin();iter!=symbols.end();iter++){
        //如果在块外
        if(iter - (symbols.begin()+blockCheck) < 0){
            if(*iter == tmpnode){
                return (*iter).blkn;
            }
        }else{
            //块内
            if(*iter == tmpnode){
                return 1;
            }
        }
    }
    return 0;
}

//定位操作，在块开始时调用
//将栈顶指针top的值压入块索引表顶端。
//块索引表的元素是指针，指向相应块的子表中第一个记录在栈中的位置。（vector里面的iterator）
void SymbolTable::loc(){
    //将栈顶指针压入块索引表顶端
    blockIndexTabs.push_back(symbols.size());
}

//重定位，在块结束时调用
//用块索引表顶端元素的值恢复栈顶指针top，有效地清除刚刚被编译完的块在栈式符号表中的所有记录。
//pop last item 直到 *top==curitem
void SymbolTable::reloc(){
    int size = symbols.size();
    int blockTopIndex = blockIndexTabs.back();
    for (int i = 0; i < (size - blockTopIndex); ++i) {
        symbols.pop_back();
    }
    blockIndexTabs.pop_back();
}

void SymbolTable::check() {
    printf("[INFO]checking symbol table..\n");
    for (int i = 0; i < symbols.size(); ++i) {
        for (int j = 0; j < symbols[i].blkn; ++j) {
            printf("\t");
        }
        printf("index=%d, name=%s, type=%s, blkn=%d, declarRow=%d\n",i,symbols[i].name.c_str(),symbols[i].type.c_str(),symbols[i].blkn,symbols[i].declarRow);
    }
    printf("-------------------------------\n");
}

void SymbolTable::ref(const string &name, const string &type, const int refrow) {
    for(auto & symbol : symbols){
        if(symbol==node(name,type)){
            symbol.refRows.push_back(refrow);
        }
    }
}

//void SymbolTable::refsymbol(const string& name, const string& type, const int refrow){
//    // 如果块深度为0则为全局变量，否则为局部变量。
//    node tmpnode = node(name,type);
//    auto blockCheck = blockIndexTabs.back();
//    for(auto iter = symbols.begin();iter!=symbols.end();iter++){
//        //如果在块外
//        if(iter - (symbols.begin()+blockCheck) < 0){
//            if(*iter == tmpnode){
//                if ((*iter).blkn == 0){
//                    //如果是全局变量，允许引用
//                    (*iter).refRows.push_back(refrow);
//                }else{
//                    //否则报错，不能引用其他块内的局部变量
//                    continue;
//                }
//            }
//        }else{
//            //块内
//            if(*iter == tmpnode){
//                (*iter).refRows.push_back(refrow);
//            }
//        }
//    }
//}
