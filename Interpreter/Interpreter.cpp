#include "Interpreter.h"

#include <cmath>

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
    const double result = Computer(expression);
    cout << result << endl;
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
    getline(cin, input);

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
        else if (element == ")") {
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
        else if (element == ",") {
            while (!operators.empty() && operators.top() != "(") {
                expression.push(operators.top());
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

    return expression;
}

double Interpreter::Computer(queue<string>& expression) const
{

    stack<double> numbers;

    while (!expression.empty()) {
        string token = expression.front();
        expression.pop();

        if (isNumber(token)) {
            numbers.push(std::stod(token));
        } else if (isValidFunc(token)) {
            if (numbers.empty()) throw std::runtime_error("Invalid expression");
            const double num1 = numbers.top();
            numbers.pop();
            double result = 0;

            if (token == "abs") {
                result = std::abs(num1);
            } else if (token == "min") {
                const double num2 = numbers.top();
                numbers.pop();
                result = std::min(num1, num2);
            } else if (token == "max") {
                const double num2 = numbers.top();
                numbers.pop();
                result = std::max(num1, num2);
            } else if (token == "pow") {
                const double num2 = numbers.top();
                numbers.pop();
                result = std::pow(num2, num1);
            }
            else {
                throw std::out_of_range("Unsupported function");
            }
            numbers.push(result);
        } else if (isOperator(token[0])) {
            if (numbers.size() < 2) throw std::runtime_error("Invalid expression");
            const double num2 = numbers.top();
            numbers.pop();
            const double num1 = numbers.top();
            numbers.pop();

            double interimResult = 0;
            if (token == "+") {
                interimResult = num1 + num2;
            } else if (token == "-") {
                interimResult = num1 - num2;
            } else if (token == "*") {
                interimResult = num1 * num2;
            } else if (token == "/") {
                if (num2 == 0) throw std::invalid_argument("Division by zero");
                interimResult = num1 / num2;
            } else {
                throw std::out_of_range("Unsupported operator");
            }
            numbers.push(interimResult);
        }
    }
    if (numbers.empty()) throw std::runtime_error("Invalid expression");
    return numbers.top();
}
