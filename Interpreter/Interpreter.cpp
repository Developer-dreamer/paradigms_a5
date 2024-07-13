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

void Interpreter::Evaluate() const
{
    queue<string> expression = ShuntingYard();

}

void Interpreter::getInput() const
{
    for (const auto& element : mInput)
    {
        cout << element << " ";
    }
    cout << '\n' << endl;
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

    // adding unary minus

    string validatedInput;
    int j = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == ' ')
        {
            continue;
        }
        if (input[i] == '-' && (i == 0 || validatedInput[j-1] == '('))
        {
            validatedInput.push_back('0');
            validatedInput.push_back('-');
            j+=2;
        }
        else
        {
            validatedInput.push_back(input[i]);
            j++;
        }
    }


    for (char token : validatedInput)
    {
        if (token == '(' || token == ')')
        {
            if (!number.str().empty())
            {
                if (isValidFunc(number.str()) || isNumber(number.str()))
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
        else
        {
            cout << "Invalid character: " << token << endl;
            exit(-1);
        }
    }
    pushToken(number);
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

queue<string> Interpreter::ShuntingYard() const
{
    queue<string> expression;
    stack<string> operators;
    for (const auto& element : mInput) {
        if (element == "(")
            operators.push(element);
        else if (element == ")" || element == ",") {
            while (!operators.empty()) {
                if (operators.top() == "(") {
                    operators.pop();
                    if (operators.top() == "abs") {
                        expression.push(operators.top());
                        operators.pop();
                    }
                    break;
                }
                expression.push(operators.top());
                operators.pop();

            }

            if (!operators.empty() && operators.top() == "(") {
                operators.pop();
            }
        }
        else if (isNumber(element)) {
            expression.push(element);
        }
        else if (isValidFunc(element)) {
            operators.push(element);
        }
        else if (isOperator(element[0])) {
            while (!operators.empty() && comparePriority(operators, element)) {
                expression.push(operators.top());
                operators.pop();
            }
            operators.push(element);
        }
    }

    while (!operators.empty()) {
        if (operators.top() == "(") {
            operators.pop();
        } else {
            expression.push(operators.top());
            operators.pop();
        }
    }

    const int size = expression.size();
    for (int i = 0; i < size; i++) {
        cout << expression.front() << " ";
        expression.pop();
    }

    return expression;
}
