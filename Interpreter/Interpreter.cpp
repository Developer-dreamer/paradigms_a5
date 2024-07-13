#include "Interpreter.h"
#include <iostream>
#include <sstream>
#include <unordered_set>


using namespace std;

Interpreter::Interpreter()
{
    Parse();
}

void Interpreter::Parse()
{
    string input;
    cout << "Enter an expression: "; getline(cin, input);

    int i  = 0;
    while (input[i] == ' ') { i++; }
    string def = input.substr(i, 3);
    if (def == "def")
    {
        FunctionParser();
        return;
    }

    std::ostringstream number;

    if (input[0] == '-')
    {
        mInput.emplace("0");
    }

    for (char token : input)
    {

        if (token == '(' || token == ')')
        {
            if (!number.str().empty())
            {
                if ((isValidFunc(number.str()) || isNumber(number.str())))
                {
                    pushToken(number);
                }
                else
                {
                    throw invalid_argument("Invalid arguments: " + number.str());
                }
            }
            mInput.emplace(1, token);
        }
        else if (isNumber(token) || isLetter(token) || token == '.')
            number << token;
        else if (token == ',')
        {
            pushToken(number);
            mInput.emplace(1, ',');
        }
        else if (isOperator(token))
        {
            pushToken(number);
            mInput.emplace(1, token);
        }
        else if (token == ' ')
        {
            pushToken(number);
        }
        else
        {
            cout << "Invalid character: " << token << endl;
            exit(-1);
        }
    }
    mInput.push(number.str());

    while (!mInput.empty())
    {
        cout << mInput.front() << " ";
        mInput.pop();
    }
}

void Interpreter::FunctionParser(string &input)
{
    string name;
    vector<string> args;
    queue<string> body;
}

bool Interpreter::isOperator(const char& c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool Interpreter::isNumber(const char& c)
{
    return c >= '0' && c <= '9';
}

bool Interpreter::isNumber(const string& s)
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

bool Interpreter::isLetter(const char& c)
{
    const bool isMaxOrMin = c == 'm' || c == 'a' || c == 'x' || c == 'i' || c == 'n';
    const bool isPow = c == 'p' || c == 'o' || c == 'w';
    const bool isAbs = c == 'b' || c == 's';
    return isMaxOrMin || isPow || isAbs;
}

bool Interpreter::isValidFunc(const string& func)
{
    static unordered_set<string> functions = {"min", "max", "abs", "pow"};
    return functions.find(func) != functions.end();
}

void Interpreter::pushToken(std::ostringstream &num)
{
    if (!num.str().empty())
    {
        mInput.push(num.str());
        num.str("");
    }
}