#include "Interpreter/Interpreter.h"

int main()
{
    Interpreter interpreter;
    while (true)
    {
        interpreter.setInput();
        interpreter.Evaluate();
    }
    return 0;
}
