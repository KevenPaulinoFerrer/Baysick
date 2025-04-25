#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <filesystem>
#include <functional>
using std::cin, std::cout, std::string, std::map, std::vector;

void NewDirectory(vector<string> att);
void DeleteDirectory(vector<string> att);
void RenameDirectory(vector<string> att);
void MoveDirectory(vector<string> att);
void Hello(vector<string> att);
string Help(string action);

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

bool FuncExists(map<string, std::function<void(vector<string>)>> func1, string action)
{
    bool found = true;

    auto it = func1.find(action);
    if (it == func1.end())
    {
        found = false;
        cout << "Error: Function does not exist\n";
    }

    return found;
}

bool PathExists(std::filesystem::path path)
{
    path = path.make_preferred();
    bool exists = false;

    std::filesystem::directory_entry entry{path};
    if (entry.exists())
    {
        exists = true;
    }

    return exists;
}

bool RootExists(std::filesystem::path root)
{
    root = root.root_path();
    return PathExists(root) == true ? true : false;
}

bool InitCommand(vector<string> &att, string &input, map<string, std::function<void(vector<string>)>> func)
{
    bool close = false;
    string action = SearchCommand(input);
    bool funcStat = FuncExists(func, action);
    if (action != "close")
    {
        SearchAtt(att, input);
        if (funcStat != false && !att.empty() && att.at(0) == "help")
        {
            cout << Help(action);
        }
        else if (funcStat != false)
        {
            func.at(action)(att);
        }
    }
    else
    {
        close = true;
    }
    att.clear();
    return close;
}

int main()
{

    map<string, std::function<void(vector<string>)>> func;
    func["hello"] = Hello;
    func["Ndirec"] = NewDirectory;
    func["Ddirec"] = DeleteDirectory;
    func["Rdirec"] = RenameDirectory;
    func["Mdirec"] = Hello;

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

    } while (InitCommand(att, input, func) == false);

    return 0;
}

string Help(string action)
{
    map<string, string> help;
    help["hello"] = " * Function name *  No parameters\n";
    help["Ndirec"] = " * Function name *  existing path ...\n";
    help["Ddirec"] = " * Function name *  existing path ...\n";
    help["Mdirec"] = " * Function name *  from path / to path ...\n";
    help["Rdirec"] = " * Function name *  existing path / new name\n";
    help["list"] = " * Function name *  No parameters\n";
    cout << "/ - represents spaces\n";

    return help.at(action);
}
void Hello(vector<string> att)
{
    cout << "Hello World\n";
}
void NewDirectory(vector<string> att)
{
    // only path to new directory
    for (std::filesystem::path path : att)
    {
        path = path.make_preferred();
        if (PathExists(path) == false && RootExists(path) == true)
        {
            std::filesystem::create_directories(path);
        }
        else if (PathExists(path) == true)
        {
            cout << "The Directory: " << path << " already exists";
        }
        else
        {
            cout << "The root path: " << path.root_path() << " doesnt exists";
        }
    }
}
void DeleteDirectory(vector<string> att)
{ // only path to existing directory
    for (std::filesystem::path path : att)
    {
        path = path.make_preferred();
        if (PathExists(path) == true)
        {
            std::filesystem::remove_all(path);
        }
        else
        {
            cout << "The Directory: " << path << "doesnt exists ";
        }
    }
    cout << '\n';
}
void RenameDirectory(vector<string> att)
{
    // path to existing directory & new name
    std::filesystem::path oName;
    std::filesystem::path nName;
    if (att.size() % 2 == 1)
    {
        cout << "Function needs an even number of arguments";
    }
    else
    {
        for (size_t i = 0; i < att.size(); i += 2)
        {
            oName = att.at(i);
            nName = oName.parent_path();
            nName += att.at(i + 1);
            oName = oName.make_preferred();
            nName = nName.make_preferred();
            bool pathStat = (PathExists(oName) == true && PathExists(nName) == false);

            if (pathStat == true)
            {
                try
                {
                    std::filesystem::rename(oName, nName);
                    cout << "Success ";
                }
                catch (const std::exception &e)
                {
                    cout << "ERROR: The name " << att.at(i + 1) << " is invalid" << '\n';
                }
            }
            else if (pathStat == false)
            {
                cout << "ERROR";
            }
        }
    }
}
void MoveDirectory(vector<string> att)
{
    // path to existing directory & new name
    std::filesystem::path fromPath;
    std::filesystem::path toPath;
    if (att.size() % 2 == 1)
    {
        cout << "Function needs an even number of arguments";
    }
    else
    {
        for (size_t i = 0; i < att.size(); i += 2)
        {
            fromPath = att.at(i);
            toPath = att.at(i + 1) + "/";
            toPath += fromPath.filename();
            fromPath = fromPath.make_preferred();
            toPath = toPath.make_preferred();
            bool pathStat = (PathExists(fromPath) == true && PathExists(toPath) == false);

            if (pathStat == true)
            {
                try
                {
                    std::filesystem::copy(fromPath, toPath, std::filesystem::copy_options::recursive);
                    std::filesystem::remove_all(fromPath);
                    cout << "Successfully moved the directory";
                }
                catch (const std::exception &e)
                {
                    cout << "ERROR " << att.at(i + 1) << " doesnt exist" << '\n';
                }
            }
            else if (pathStat == false)
            {
                if (toPath == fromPath)
                {
                    cout << "ERROR - already in directory";
                }
                else
                {
                    cout << "ERROR - Invalid path " << (PathExists(fromPath) == true ? att.at(i + 1) : att.at(i));
                }
            }
        }
    }
}