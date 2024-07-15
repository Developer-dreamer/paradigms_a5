#pragma once
#include <list>
#include <queue>
#include <queue>
#include <vector>

#include "Helper.h"

using namespace std;
#ifndef CUSTOMFUNCTION_H
#define CUSTOMFUNCTION_H



class CustomFunction {
public:
   CustomFunction() = default;
   explicit CustomFunction(const string &input);
   ~CustomFunction() = default;

   string getExpression(const vector<double> &args) const;
   static string setArgsToBody(string input, vector<tuple<string, double>> args);
private:
   string mName;
   vector<string> mArgs;
   list<string> mRpnBody;

   void parseFunction(string input);
   list<string> ParseWithArgs(string &body);
   void ShuntingYardWithArgs(const list<string> &parsedInput);

   static void PushToken(list<string> parsedInput, std::ostringstream& num);

   string setArgsToBody(const vector<double> &args) const;
   bool isArg(const string &token) const;
   bool isValidLetter(char letter);

   friend class Interpreter;
};



#endif //CUSTOMFUNCTION_H
