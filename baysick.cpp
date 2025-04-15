#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <filesystem>
#include <functional>
using std::cin, std::cout, std::string, std::map, std::vector;

void Hello()
{
    cout << "Hello World";
}
string SearchCommand(string &input)
{
    // size_t = always non-negative/usesd to represent numeric values that cannot be negative: size,indexes, etc.
    size_t begin, end;
    string command;
    string valChar = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    begin = input.find_first_of(valChar);
    if (begin != string::npos)
    {
        input = input.substr(begin, string::npos); // string::npos = used to declare "until the end of the string"

        begin = input.find_first_of(valChar);
        end = input.find_first_of(" ");

        command = input.substr(begin, end);
        input.erase(begin, end);
    }
    else
    {
        input.clear();
    }

    return command;
}
void SearchAtt(vector<string> &att, string &input)
{
    // size_t = always non-negative/used to represent numeric values that cannot be negative: size,indexes, etc.
    for (size_t i = 1; i < 5; i++)
    {
        if (!input.empty())
        {
            att.push_back(SearchCommand(input));
        }
    }
}
void FuncExists(map<string, std::function<void()>> func, string action)
{
    auto it = func.find(action);
    if (it != func.end())
    {
        func.at(action)();
    }
    else
    {
        cout << "Error: Function does not exist";
    }
}
void InitCommand(vector<string> &att, string &input, map<string, std::function<void()>> func)
{
    string action = SearchCommand(input);
    SearchAtt(att, input);
    FuncExists(func, action);
}
int main()
{
    map<string, std::function<void()>> func;
    func["Hello"] = Hello;

    vector<string> att;
    string sym = "!~~ ";
    string input;

    cout << sym;
    getline(cin, input);
    InitCommand(att, input, func);

    cout << '\n';
    for (string a : att)
    {
        cout << a << '\n';
    }

    return 0;
}
