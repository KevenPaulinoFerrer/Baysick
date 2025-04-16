#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <filesystem>
#include <functional>
using std::cin, std::cout, std::string, std::map, std::vector;

void Hello(vector<string> att)
{

    if (!att.empty() && att.at(0) == "help")
    {
        cout << "*Function name* - No parameters";
    }
    else
    {
        cout << "Hello World\n";
    }
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

int FuncExists(map<string, std::function<void(vector<string>)>> func1, string action)
{
    int found;

    auto it = func1.find(action);
    if (it != func1.end())
    {
        found = 0;
    }
    else
    {
        found = 1;
        cout << "Error: Function does not exist\n";
    }
    return found;
}

bool InitCommand(vector<string> &att, string &input, map<string, std::function<void(vector<string>)>> func1)
{
    bool close = true;
    string action = SearchCommand(input);
    if (action != "close")
    {
        SearchAtt(att, input);
        if (FuncExists(func1, action) != 1)
        {
            func1.at(action)(att);
        }
    }
    else
    {
        close = false;
    }
    return close;
}

int main()
{

    map<string, std::function<void(vector<string>)>> func;
    func["hello"] = Hello;

    vector<string> att;
    string sym = "!~~ ";
    string input;

    do
    {
        cout << sym;
        getline(cin, input);
        if (input.find("close") == string::npos)
        {
            cout << sym;
        }

    } while (InitCommand(att, input, func) == true);

    return 0;
}
