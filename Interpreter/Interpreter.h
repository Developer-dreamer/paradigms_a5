#pragma once
#include "CustomFunction.h"
#include <queue>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

#ifndef INTERPRETER_H
#define INTERPRETER_H
class Interpreter
{
public:
    Interpreter();
    void Evaluate();
    void setInput();
    void getInput() const;
private:
    int evaluationOption = 0;
    CustomFunction functionToEvaluate;
    string variableToEvaluate;
    string mInput;
    unordered_map<string, double> variables;
    list<string> mInputParsed;
    vector<CustomFunction> mCustomFunctions;

    void Parse();

    void VariableParser(const string &input);

    queue<string> ShuntingYard() const;

    double Compute(queue<string> &expression) const;
    double ComputeWithVariable();


    static string validateInput(const string &input);
    void pushToken(std::ostringstream& num);
};
#endif //INTERPRETER_H
