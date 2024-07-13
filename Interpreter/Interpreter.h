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
    void Evaluate() const;
    void getInput() const;
private:
    list<string> mInput;
    vector<vector<string>> customFunctions;

    void Parse();
    void FunctionParser(string &input);

    queue<string> ShuntingYard() const;

    double Computer(queue<string> &expression) const;

    void pushToken(std::ostringstream& num);
};
#endif //INTERPRETER_H
