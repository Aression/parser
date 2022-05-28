cmake_minimum_required(VERSION 3.15.3)
project(parser)

set(CMAKE_CXX_STANDARD 11)

add_executable(parser main.cpp token.h token.cpp reminder.h file_reader.h file_reader.cpp reminder.cpp lexer.h lexer.cpp parser.h parser.cpp symbol_table.h symbol_table.cpp errkinds.h errkinds.cpp)
