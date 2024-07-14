#pragma once
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

   double Evaluate(const vector<double>& args) const;

   static string setArgsToBody(string input, vector<tuple<string, double>> args);
private:
   string mBody;
   string mName;
   vector<string> mArgs;
   queue<string> mRpnBody;

   void parseFunction(string input);

   friend class Interpreter;
};



#endif //CUSTOMFUNCTION_H
