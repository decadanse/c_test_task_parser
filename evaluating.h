#pragma once
#ifndef POSTFIXEVALUATION_H
#define POSTFIXEVALUATION_H
#include "shunting-yard.h"

class PostfixEvaluation
{
public:
    PostfixEvaluation();
    ~PostfixEvaluation();

    float ast_evaluation(Node* ast);

private:
    bool is_number(const std::string& s);
};
#endif
