#include "parsing.h"

// Empty constructor and destructor
Parser::Parser() {}
Parser::~Parser() {}

 
bool Parser::check_number(std::string s)
{
    int n;
    
    try
    {
        n = std::stof(s);
    }
    catch (std::invalid_argument& ia)
    {
        return false;
    }

    return true;
}

bool Parser::check_if_in(std::string s, const std::vector<std::string> v)
{
    auto r = std::find(std::begin(v), std::end(v), s);
    if (r != std::end(v))
    {
        return true;
    }
    return false;
}

// Check if `a` precedes `b`
bool Parser::check_precedence(std::string a, std::string b)
{
    if (a == "/" || a == "*")
    {
        return true;
    }
    return false;
}