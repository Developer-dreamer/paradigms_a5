#include "CustomFunction.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <unordered_map>
#include <pstl/execution_defs.h>

#include "Helper.h"

using namespace std;

CustomFunction::CustomFunction(const string &input)
{
    parseFunction(input);
}

string CustomFunction::getExpression(const vector<double>& args) const
{
    string funcExpression = setArgsToBody(args);
    return funcExpression;
}

string CustomFunction::setArgsToBody(string input, vector<tuple<string, double>> args)
{
    for (auto & i : args)
    {
        const string arg = get<0>(i);
        const string argToReplaceWith = to_string(get<1>(i));
        size_t pos = 0;
        while ((pos = input.find(arg, pos)) != string::npos)
        {
            input.replace(pos, arg.length(), argToReplaceWith);
            pos += argToReplaceWith.length();
        }
    }

    return input;
}

void CustomFunction::parseFunction(string input)
{
    input = input.substr(3, input.length());
    const string name = input.substr(0, input.find('('));
    const string args = input.substr(input.find('(') + 1, input.find(')') - input.find('(') - 1);
    string body = input.substr(input.find('{') + 1, input.find('}') - input.find('{') - 1);

    mName = name;
    mArgs = split(args,',');

    list<string> parsedBody = ParseWithArgs(body);
    ShuntingYardWithArgs(parsedBody);
}

list<string> CustomFunction::ParseWithArgs(string &body)
{
    list<string> parsedInput;
    std::ostringstream number;

    for (char token : body)
    {
        if (token == '(' || token == ')')
        {
            if (!number.str().empty())
            {
                if (isValidFunc(number.str()) || isNumber(number.str()))
                {
                    PushToken(parsedInput, number);
                }
                else
                {
                    throw invalid_argument("Invalid arguments: " + number.str());
                }
            }
            parsedInput.emplace_back(1, token);
        }
        else if (isNumber(token) || token == '.' || isValidLetter(token))
            number << token;
        else if (token == ',')
        {
            PushToken(parsedInput, number);
            parsedInput.emplace_back(1, ',');
        }
        else if (isOperator(token))
        {
            PushToken(parsedInput, number);
            parsedInput.emplace_back(1, token);
        }
        else
        {
            cout << "Invalid character: " << token << endl;
            exit(-1);
        }
    }
    PushToken(parsedInput, number);

    return parsedInput;
}

void CustomFunction::ShuntingYardWithArgs(const list<string> &parsedInput)
{
    list<string> expression;
    stack<string> operators;
    for (const auto& element : parsedInput)
    {
        if (element == "(" || isValidFunc(element))
        {
            operators.push(element);
        }
        else if (element == ")")
        {
            while (!operators.empty())
            {
                if (operators.top() == "(")
                {
                    operators.pop();
                    if (operators.top() == "abs")
                    {
                        expression.push_back(operators.top());
                        operators.pop();
                    }
                    break;
                }
                expression.push_back(operators.top());
                operators.pop();
            }

            if (!operators.empty() && operators.top() == "(")
            {
                operators.pop();
            }
        }
        else if (element == ",")
        {
            while (!operators.empty() && operators.top() != "(")
            {
                expression.push_back(operators.top());
                operators.pop();
            }
        }
        else if (isNumber(element) || isArg(element))
        {
            expression.push_back(element);
        }
        else if (isOperator(element[0]))
        {
            while (!operators.empty() && comparePriority(operators, element))
            {
                expression.push_back(operators.top());
                operators.pop();
            }
            operators.push(element);
        }
    }

    while (!operators.empty())
    {
        if (operators.top() == "(")
        {
            operators.pop();
        }
        else
        {
            expression.push_back(operators.top());
            operators.pop();
        }
    }

    mRpnBody = expression;
}

void CustomFunction::PushToken(list<string> parsedInput, std::ostringstream& num)
{
    if (!num.str().empty())
    {
        parsedInput.push_back(num.str());
        num.str("");
    }
}

string CustomFunction::setArgsToBody(const vector<double>& args) const
{
    stringstream prepBody;
    for (const auto &element : mRpnBody)
    {
        if (isArg(element))
        {
            const auto it = find(mArgs.begin(), mArgs.end(), element);
            const int index = distance(mArgs.begin(), it);
            prepBody << args[index];
        }
        else
        {
            prepBody << element;
        }
    }

    return prepBody.str();
}

bool CustomFunction::isArg(const string &token) const {
    const auto it = find(mArgs.begin(), mArgs.end(), token);
    return it != mArgs.end();
}

bool CustomFunction::isValidLetter(const char letter)
{
    return any_of(mArgs.begin(), mArgs.end(), [letter](const string& arg) {
        return arg.find(letter) != string::npos;
    });
}
