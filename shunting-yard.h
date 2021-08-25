#pragma once
#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H
#include "parsing.h"

class ShuntingYard
{
public:
    ShuntingYard(std::string line);
    ~ShuntingYard();
    void parsing();
    void print_result();
    Node* get_ast();
    std::stack<std::string> get_rpn();

private:
    Parser p;
    char cc;
    std::string s;
    std::string line;
    std::stack<std::string> rpn;
    std::stack<Node*> ast_stack;
    std::string clear_line(const std::string s);
};
#endif