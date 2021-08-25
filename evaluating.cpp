#include "evaluating.h"

PostfixEvaluation::PostfixEvaluation() {}
PostfixEvaluation::~PostfixEvaluation() {}

bool PostfixEvaluation::is_number(const std::string& s)
{
    std::string tmp = s;

    // Check if string format is a number
    if (std::regex_match(tmp, std::regex("[-+]?\\d+\\.?\\d*")))
    {
        return true;
    }
    return false;
}

// Recursive evaluation of the stack AST
float PostfixEvaluation::ast_evaluation(Node* ast)
{
    // Check null
    if (ast == nullptr)
    {
        throw std::invalid_argument("Invalid AST: nullptr");
    }

    // Evaluation
    std::string s = ast->value;

    if (is_number(s))
    {
        return std::stof(s);
    }
    else
    {
        float op1 = ast_evaluation(ast->left);
        float op2 = ast_evaluation(ast->right);

        if (s == "+") { return op1 + op2; }
        else if (s == "-") { return op1 - op2; }
        else if (s == "*") { return op1 * op2; }
        else if (s == "/") { return op1 / op2; }
    }
}