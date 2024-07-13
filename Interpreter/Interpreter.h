#pragma once
#include <queue>
#include <sstream>
#include <stack>
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
    queue<string> mInput;
    vector<vector<string>> customFunctions;

    void Parse();
    void FunctionParser(string &input);

    static bool isOperator(const char &c);
    static bool isNumber(const char &c);
    static bool isNumber(const string& s);
    static bool isLetter(const char &c);
    static bool isValidFunc(const string& func);
    void pushToken(std::ostringstream& num);
};
#endif //INTERPRETER_H
