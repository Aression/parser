//
// Created by 吕迪 on 2022/5/27.
//

#ifndef PARSER_ERRKINDS_H
#define PARSER_ERRKINDS_H

enum errs{
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h,
    i,
    j,
    k,
    l,
    m,
    n,
    o,
    p
};

class ERR{
public:
    static std::string meaning(int errKind);
};

#endif //PARSER_ERRKINDS_H
