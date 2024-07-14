#include "Interpreter.h"

#include <cmath>

#include "Helper.h"
#include "CustomFunction.h"
#include <list>
#include <iostream>
#include <sstream>
#include <stack>


using namespace std;

Interpreter::Interpreter() = default;

void Interpreter::Evaluate()
{
    switch (evaluationOption)
    {
    case 0:
        {
            queue<string> expression = ShuntingYard();
            const double result = Compute(expression);
            cout << result << endl;
            mInputParsed.clear();
            break;
        }
    case 1:
        {
            queue<string> expression = ShuntingYard();
            const double value = Compute(expression);
            variables[variableToEvaluate] = value;
            mInputParsed.clear();
            break;
        }
    case 2:
        {
            cout << "Function: " << functionToEvaluate.mName << " added successfully" << endl;
            break;
        }
    default:
        {
            cout << "Invalid evaluation option" << endl;
            break;
        }
    }
}

void Interpreter::setInput()
{
    string input;
    getline(cin, input);
    input = validateInput(input);

    if (input.find('=') != string::npos)
    {
        VariableParser(input);
        evaluationOption = 1;
        return;
    }
    {
        bool parseVar = false;
        vector<tuple<string, double>> args;
        for (const auto& element : variables)
        {
            if (input.find(element.first) != string::npos)
            {
                parseVar = true;
                args.emplace_back(element.first, element.second);
            }
        }
        if (parseVar)
        {
            evaluationOption = 0;
            input = CustomFunction::setArgsToBody(input, args);
            mInput = input;
            Parse();
            return;
        }
    }

    if (const string def = input.substr(0, 3); def == "def")
    {
        evaluationOption = 2;
        CustomFunction customFunction(input);
        mInput = customFunction.mBody;
        Parse();
        queue<string> rpnBody = ShuntingYard();
        customFunction.mRpnBody = rpnBody;
        return;
    }

    {
        bool parseFunc = false;
        for (const auto& element : mCustomFunctions)
        {
            if (input.find(element.mName) != string::npos)
            {
                parseFunc = true;
                functionToEvaluate = element;
                evaluationOption = 2;
                return;
            }
            cout << "Function not found" << endl;
            exit(-1);
        }
        if (parseFunc)
        {
            evaluationOption = 0;
            Parse();
            return;
        }
    }

    mInput = input;
    Parse();
}

string Interpreter::validateInput(const string& input)
{
    string validatedInput;
    int j = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == ' ')
        {
            continue;
        }
        if (input[i] == '-' && (i == 0 || validatedInput[j - 1] == '('))
        {
            validatedInput.push_back('0');
            validatedInput.push_back('-');
            j += 2;
        }
        else
        {
            validatedInput.push_back(input[i]);
            j++;
        }
    }

    return validatedInput;
}

void Interpreter::getInput() const
{
    for (const auto& element : mInputParsed)
    {
        cout << element << " ";
    }
    cout << '\n' << endl;
}

void Interpreter::Parse()
{
    std::ostringstream number;

    for (char token : mInput)
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
            mInputParsed.emplace_back(1, token);
        }
        else if (isNumber(token) || isLetter(token) || token == '.')
            number << token;
        else if (token == ',')
        {
            pushToken(number);
            mInputParsed.emplace_back(1, ',');
        }
        else if (isOperator(token))
        {
            pushToken(number);
            mInputParsed.emplace_back(1, token);
        }
        else
        {
            cout << "Invalid character: " << token << endl;
            exit(-1);
        }
    }
    pushToken(number);
}

void Interpreter::pushToken(std::ostringstream& num)
{
    if (!num.str().empty())
    {
        mInputParsed.push_back(num.str());
        num.str("");
    }
}

void Interpreter::VariableParser(const string& input)
{
    int i = 0;
    const string name = input.substr(0, input.find('='));

    mInput = input.substr(input.find('=') + 1);
    variableToEvaluate = name;
    Parse();
}

queue<string> Interpreter::ShuntingYard() const
{
    queue<string> expression;
    stack<string> operators;
    for (const auto& element : mInputParsed)
    {
        if (element == "(")
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
                        expression.push(operators.top());
                        operators.pop();
                    }
                    break;
                }
                expression.push(operators.top());
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
                expression.push(operators.top());
                operators.pop();
            }
        }
        else if (isNumber(element))
        {
            expression.push(element);
        }
        else if (isValidFunc(element))
        {
            operators.push(element);
        }
        else if (isOperator(element[0]))
        {
            while (!operators.empty() && comparePriority(operators, element))
            {
                expression.push(operators.top());
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
            expression.push(operators.top());
            operators.pop();
        }
    }

    return expression;
}

double Interpreter::Compute(queue<string>& expression) const
{
    stack<double> numbers;

    while (!expression.empty())
    {
        string token = expression.front();
        expression.pop();

        if (isNumber(token))
        {
            numbers.push(std::stod(token));
        }
        else if (isValidFunc(token))
        {
            if (numbers.empty()) throw std::runtime_error("Invalid expression");
            const double num1 = numbers.top();
            numbers.pop();
            double result = 0;

            if (token == "abs")
            {
                result = std::abs(num1);
            }
            else if (token == "min")
            {
                const double num2 = numbers.top();
                numbers.pop();
                result = std::min(num1, num2);
            }
            else if (token == "max")
            {
                const double num2 = numbers.top();
                numbers.pop();
                result = std::max(num1, num2);
            }
            else if (token == "pow")
            {
                const double num2 = numbers.top();
                numbers.pop();
                result = std::pow(num2, num1);
            }
            else
            {
                throw std::out_of_range("Unsupported function");
            }
            numbers.push(result);
        }
        else if (isOperator(token[0]))
        {
            if (numbers.size() < 2) throw std::runtime_error("Invalid expression");
            const double num2 = numbers.top();
            numbers.pop();
            const double num1 = numbers.top();
            numbers.pop();

            double interimResult = 0;
            if (token == "+")
            {
                interimResult = num1 + num2;
            }
            else if (token == "-")
            {
                interimResult = num1 - num2;
            }
            else if (token == "*")
            {
                interimResult = num1 * num2;
            }
            else if (token == "/")
            {
                if (num2 == 0) throw std::invalid_argument("Division by zero");
                interimResult = num1 / num2;
            }
            else
            {
                throw std::out_of_range("Unsupported operator");
            }
            numbers.push(interimResult);
        }
    }
    if (numbers.empty()) throw std::runtime_error("Invalid expression");
    return numbers.top();
}
