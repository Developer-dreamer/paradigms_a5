#include "Interpreter.h"
#include "Helper.h"
#include <list>
#include <iostream>
#include <sstream>
#include <stack>


using namespace std;

Interpreter::Interpreter()
{
    Parse();
}

void Interpreter::Evaluate()
{

}


void Interpreter::Parse()
{
    string input;
    cout << "Enter an expression: "; getline(cin, input);

    int i = 0;
    while (input[i] == ' ') { i++; }

    if (const string def = input.substr(i, 3); def == "def")
    {
        FunctionParser(input);
        return;
    }

    std::ostringstream number;

    if (input[0] == '-')
    {
        mInput.emplace_back("0");
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
            mInput.emplace_back(1, token);
        }
        else if (isNumber(token) || isLetter(token) || token == '.')
            number << token;
        else if (token == ',')
        {
            pushToken(number);
            mInput.emplace_back(1, ',');
        }
        else if (isOperator(token))
        {
            pushToken(number);
            mInput.emplace_back(1, token);
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
    pushToken(number); // Ensure the last token is pushed if not empty
}

void Interpreter::pushToken(std::ostringstream &num)
{
    if (!num.str().empty())
    {
        mInput.push_back(num.str());
        num.str("");
    }
}

void Interpreter::FunctionParser(string &input)
{
    string name;
    vector<string> args;
    queue<string> body;
}