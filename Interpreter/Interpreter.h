#pragma once
#include <list>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include "CustomFunction.h"

using namespace std;

#ifndef INTERPRETER_H
#define INTERPRETER_H
class Interpreter
{
public:
    Interpreter() = default;
    ~Interpreter() = default;

    void Evaluate();

    void setInput();
    void getInput() const;

private:
    int mEvaluationOption = 0;

    string mInput;
    list<string> mInputParsed;

    string mVariableToEvaluate;
    unordered_map<string, double> mVariables;

    CustomFunction mFunctionToEvaluate;
    vector<CustomFunction> mCustomFunctions;

    void Parse();
    void VariableParser(const string &input);

    queue<string> ShuntingYard() const;

    static double Compute(queue<string> &expression);

    static string validateInput(const string &input);
    void pushToken(std::ostringstream& num);
};
#endif //INTERPRETER_H
