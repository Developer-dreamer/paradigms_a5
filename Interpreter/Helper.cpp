#include "Helper.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, int> Operators =
{
    {"(", 0},
    {")", 0},
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
};
unordered_map<string, int> Functions =
{
        {"pow", 3},
        {"min", 3},
        {"max", 3},
        {"abs", 3},
};

bool isOperator(const char& c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool comparePriority(const stack<string>& stack, const string& token) {
    if (stack.empty()) return false;

    const string topToken = stack.top();
    const bool topIsFunc = isValidFunc(topToken);
    const bool currentIsFunc = isValidFunc(token);

    if (topIsFunc != currentIsFunc) {
        if (topIsFunc) {
            // Assuming token is a single character operator when not a function
            return Functions[topToken] >= Operators[std::string(1, token[0])];
        } else {
            // Assuming topToken is a single character operator when not a function
            return Operators[std::string(1, topToken[0])] >= Functions[token];
        }
    }

    if (topIsFunc) {
        return Functions[topToken] >= Functions[token];
    } else {
        // Assuming both tokens are single character operators when not functions
        return Operators[std::string(1, topToken[0])] >= Operators[std::string(1, token[0])];
    }
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

vector<string> split(const string& input, const char& delimiter)
{
    vector<string> elements;
    stringstream ss(input);
    string item;
    while (getline(ss, item, delimiter)) {
        elements.push_back(item);
    }
    return elements;
}