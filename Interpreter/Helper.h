#pragma once

#include <stack>
#include <string>


#ifndef HELPER_H
#define HELPER_H
bool isOperator(const char& c);
bool comparePriority(const std::stack<std::string>& operators, const std::string& element);
bool isNumber(const char& c);
bool isNumber(const std::string& s);
bool isLetter(const char& c);
bool isValidFunc(const std::string& func);
#endif //HELPER_H
