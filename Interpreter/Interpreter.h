#pragma once
#include <queue>
#include <list>
#include <sstream>
#include <string>

using namespace std;

#ifndef INTERPRETER_H
#define INTERPRETER_H
class Interpreter
{
public:
    Interpreter();
    void Evaluate();
private:
    double mIntermediateResult{};
    list<string> mInput;
    vector<vector<string>> customFunctions;

    void Parse();
    void FunctionParser(string &input);



    void pushToken(std::ostringstream& num);
};
#endif //INTERPRETER_H