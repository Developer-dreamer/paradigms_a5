#include <iostream>

#include "Interpreter/Interpreter.h"

int main()
{
    const Interpreter interpreter;
    interpreter.getInput();
    interpreter.Evaluate();
    return 0;
}
