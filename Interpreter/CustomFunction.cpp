#include "CustomFunction.h"
#include "Helper.h"

#include <sstream>
#include <unordered_map>

using namespace std;
CustomFunction::CustomFunction() = default;

CustomFunction::CustomFunction(const string &input)
{
    parseFunction(input);
}

void CustomFunction::parseFunction(string input)
{
    input = input.substr(3, input.length());
    const string name = input.substr(0, input.find('('));
    const string args = input.substr(input.find('(') + 1, input.find(')') - input.find('(') - 1);
    const string body = input.substr(input.find('{') + 1, input.find('}') - input.find('{') - 1);

    mName = name;
    mArgs = split(args,',');
    mBody = body;
}

string CustomFunction::setArgsToBody(string input, vector<tuple<string, double>> args)
{
    for (auto & i : args)
    {
        const string arg = get<0>(i);
        const string argToReplaceWith = to_string(get<1>(i));
        size_t pos = 0;
        while ((pos = input.find(arg, pos)) != string::npos)
        {
            input.replace(pos, arg.length(), argToReplaceWith);
            pos += argToReplaceWith.length();
        }
    }

    return input;
}
