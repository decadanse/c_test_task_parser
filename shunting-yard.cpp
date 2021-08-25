#include "shunting-yard.h"

ShuntingYard::ShuntingYard(std::string line)
{
    this->line = line;
}

ShuntingYard::~ShuntingYard() {}

Node* ShuntingYard::get_ast()
{
    return this->ast_stack.top();
}

std::stack<std::string> ShuntingYard::get_rpn()
{
    return this->rpn;
}

std::string ShuntingYard::clear_line(const std::string s)
{
    // Small regex modification to the line
    std::string tmp, ff;

    // Remove double `-` for one `+`
    tmp = std::regex_replace(s, std::regex("\\-\\-"), "+");

    // Remove multiple `+` to just one
    tmp = std::regex_replace(tmp, std::regex("\\+\\++"), "+");

    // Remove multiple `-+` to '-`
    tmp = std::regex_replace(tmp, std::regex("\\-\\+"), "-");

    // Remove multiple `+-` to '-`
    tmp = std::regex_replace(tmp, std::regex("\\+\\-"), "-");

    // We will remove plus symbols after `*`
    tmp = std::regex_replace(tmp, std::regex("\\*\\+"), "*");

    // We will remove plus symbols after `/`
    tmp = std::regex_replace(tmp, std::regex("\\/\\+"), "/");

    // Removing whitespaces
    tmp = std::regex_replace(tmp, std::regex("\\ "), "");

    // We will enclose all the negative numbers in parenthesis,
    // to handle negative numbers properly
    std::regex_replace(std::back_inserter(ff), tmp.begin(), tmp.end(),
        std::regex("-\\s?(\\d+)"), "+\(-$1)");

    // We will remove plus symbols at the beginning of the line
    ff = std::regex_replace(ff, std::regex("^\\+"), "");

    // Check for invalid elements
    if (ff.find_first_not_of("0123456789()-+/*") != std::string::npos)
        throw std::invalid_argument("Invalid token: An invalid element was entered");

    return ff;
}

void ShuntingYard::parsing()
{
    std::istringstream input(clear_line(line));
    bool prev_number = false;
    bool prev_par = false;
    bool prev_minus = false;

    std::string op;
    std::string check = line.substr(0, 1);
    int numbercheck = 0; /*We need this variable to distinguish between valid 
                           negative numbers(when it is necessary to subtract) and 
                           unacceptable ones(when the number is in the first place and 
                           because of this is not single - character)*/

    while (input >> cc)
    {
        std::string c(1, cc);

        // Checking if the current element is a number
        if (p.check_number(c))
        {
            numbercheck++; //counting actual numbers

            // No more than one digit
            if (prev_number)
            {             
                if (!rpn.empty())
                {
                    throw std::invalid_argument("Invalid AST: in 1 digits only 1 literal allowed");
                }
            }
            else
            {
                if (prev_minus)
                { 
                   c = "-" + c;
                   prev_minus = false;
                }

                rpn.push(c);
                ast_stack.push(new Node(c));
                prev_number = true;
            }
            prev_par = false;
            prev_number = true;
        }

        // Checking operators
        else if (p.check_if_in(c, p.valid_operators))
        {
            prev_number = false;
            // Checking for negative numbers
            if (prev_par && c == "-")
            {
                //If until then wasn't any actual numbers then it's an error of input
                if (numbercheck == 0) { 
                    std::cerr << "Error: first literal can't be negative444" << std::endl;
                    exit(0);
                }

                prev_minus = true;
                continue;
            }
            else if (prev_par && c == "+")
            {
                continue;
            }
            prev_par = false;

            // Verifying precedence
            while (!p.operators.empty() &&
                p.check_precedence(p.operators.top(), c))
            {
                op = p.operators.top();
                rpn.push(op);
                p.operators.pop();

                Node* op2 = ast_stack.top();
                ast_stack.pop();
                Node* op1 = ast_stack.top();
                ast_stack.pop();

                ast_stack.push(new Node(op, op1, op2));

            }
            p.operators.push(c);
        }

        // Checking left-bracket
        else if (p.check_if_in(c, p.left_brackets))
        {
            prev_number = false;
            prev_par = true;
            p.operators.push(c);
        }

        // Checking right-bracket
        else if (p.check_if_in(c, p.right_brackets))
        {
            prev_number = false;
            while (!p.operators.empty() &&
                !p.check_if_in(p.operators.top(), p.left_brackets))
            {
                op = p.operators.top();
                rpn.push(op);
                p.operators.pop();

                Node* op2 = ast_stack.top();
                ast_stack.pop();
                Node* op1 = ast_stack.top();
                ast_stack.pop();

                ast_stack.push(new Node(op, op1, op2));
            }

            // Verify missing left brackets
            if (p.operators.empty())
            {
                throw std::invalid_argument("Mismatched parentheses: Missing left bracket");
            }

            p.operators.pop();
        }
    }

    while (!p.operators.empty())
    {
        // Remaining elements cannot be parentheses
        std::string t = p.operators.top();
        if (p.check_if_in(t, p.left_brackets) ||
            p.check_if_in(t, p.right_brackets))
        {
            throw std::invalid_argument("Mismatched parentheses: Extra bracket");
        }

        op = p.operators.top();
        rpn.push(op);
        p.operators.pop();

        Node* op2 = ast_stack.top();
        ast_stack.pop();
        Node* op1 = ast_stack.top();
        ast_stack.pop();

        ast_stack.push(new Node(op, op1, op2));
    }

    // reversing stack
    std::stack<std::string> oo;
    while (!rpn.empty())
    {
        oo.push(rpn.top());
        rpn.pop();
    }
    rpn = oo;
}

void ShuntingYard::print_result()
{
    std::stack<std::string> tmp = rpn;

    while (!tmp.empty())
    {
        std::cout << tmp.top() << " ";
        tmp.pop();
    }
    std::cout << std::endl;
}