#include "Helper.h"
#include <iostream>
#include <stack>
#include <unordered_set>

using namespace std;

bool isOperator(const char& c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool comparePriority(const std::stack<std::string>& operators, const std::string& element) {
    return !operators.empty() && isOperator(operators.top()[0]);
}

bool isNumber(const char& c)
{
    return c >= '0' && c <= '9';
}

bool isNumber(const string& s)
{
    try {
        stod(s);
        return true;
    }
    catch (const invalid_argument& _)
    {
        return false;
    }
}

bool isLetter(const char& c)
{
    const bool isMaxOrMin = c == 'm' || c == 'a' || c == 'x' || c == 'i' || c == 'n';
    const bool isPow = c == 'p' || c == 'o' || c == 'w';
    const bool isAbs = c == 'b' || c == 's';
    return isMaxOrMin || isPow || isAbs;
}

bool isValidFunc(const string& func)
{
    static unordered_set<string> functions = {"min", "max", "abs", "pow"};
    return functions.find(func) != functions.end();
}