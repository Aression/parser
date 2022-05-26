//
// Created by Administrator on 2022/5/15 0015.
//

#include "file_reader.h"

unsigned int FileReader::getLineno() const {
    return lineno;
}

unsigned int FileReader::getColno() const {
    return colno;
}

FileReader::FileReader(std::ifstream &ifstream) {
    if (ifstream.is_open()) {
        std::stringstream buffer;
        buffer << ifstream.rdbuf();
        ifstream.close();
        content = std::string(buffer.str());
        content_length = content.size();
    } else {
        std::cerr << "Cannot open file" << std::endl;
        exit(-1);
    }
}

char FileReader::get_char() {
    if (cur_index < content_length) {
        char c = content[cur_index++];
        //std::cout << "cur_index: " << cur_index << std::endl;
        colno++;
        if (c == '\n') {
            lineno++;
            colno = 0;
        }
        return c;
    }
    return EOF;
}

char FileReader::seek() {
    if (cur_index < content_length)
        return content[cur_index];
    return EOF;
}
