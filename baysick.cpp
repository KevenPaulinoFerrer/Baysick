#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <filesystem>
#include <functional>

using std::cin, std::cout, std::string, std::map, std::vector;

// Colors
// Pedro Arraiza
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

// Function declarations
void NewDirectory(vector<string> att);
void DeleteDirectory(vector<string> att);
void RenameDirectory(vector<string> att);
void MoveDirectory(vector<string> att);
void Hello(vector<string> att);
void CreateFile(vector<string> att);
void ReadFile(vector<string> att);
void UpdateFile(vector<string> att);
void DeleteFile(vector<string> att);
void ClearScreen(vector<string> att);
void Readme(vector<string> att);
void License(vector<string> att);
void ListCommands(vector<string> att);
string Help(string action);

// Utility functions
// Keven Paulino
string SearchCommand(string &input)
{
    size_t begin = input.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    if (begin == string::npos)
    {
        input.clear();
        return "";
    }

    input = input.substr(begin);
    size_t end = input.find_first_of(" ");
    if (end == string::npos)
    {
        end = input.length();
    }

    string command = input.substr(0, end);
    input.erase(0, end + 1);
    return command;
}

// Keven Paulino
void SearchAtt(vector<string> &att, string &input)
{
    do
    {
        if (!input.empty())
        {
            att.push_back(SearchCommand(input));
        }

    } while (!input.empty());
}

// Keven Paulino
bool FuncExists(map<string, std::function<void(vector<string>)>> func1, string action)
{
    auto it = func1.find(action);
    if (it == func1.end())
    {
        cout << RED << "Error: Function does not exist\n"
             << RESET;
        return false;
    }
    return true;
}

// Keven Paulino
bool PathExists(std::filesystem::path path)
{
    path = path.make_preferred();
    return std::filesystem::directory_entry{path}.exists();
}

// Keven Paulino
bool RootExists(std::filesystem::path root)
{
    root = root.root_path();
    return PathExists(root);
}

// Keven Paulino
bool InitCommand(vector<string> &att, string &input, map<string, std::function<void(vector<string>)>> func)
{
    string action = SearchCommand(input);
    if (action == "close")
    {
        return true;
    }
    bool funcStat = FuncExists(func, action);
    SearchAtt(att, input);

    if (funcStat && !att.empty() && att.at(0) == "help")
    {
        cout << Help(action);
    }
    else if (funcStat)
    {
        func.at(action)(att);
    }

    att.clear();
    return false;
}

// Main
int main()
{
    map<string, std::function<void(vector<string>)>> func;
    func["hello"] = Hello;
    func["Ndirec"] = NewDirectory;
    func["Ddirec"] = DeleteDirectory;
    func["Rdirec"] = RenameDirectory;
    func["Mdirec"] = MoveDirectory;
    func["Cfile"] = CreateFile;
    func["Rfile"] = ReadFile;
    func["Ufile"] = UpdateFile;
    func["Dfile"] = DeleteFile;
    func["clear"] = ClearScreen;
    func["readme"] = Readme;
    func["license"] = License;
    func["list"] = ListCommands;

    vector<string> att;
    string sym = "!~~ ";
    string input;

    cout << sym;
    Readme(att);
    do
    {
        cout << sym;
        getline(cin, input);

    } while (!InitCommand(att, input, func));

    return 0;
}
// Keven Paulino
void Readme(vector<string> att)
{
    att.push_back("README.md");
    ReadFile(att);
}

// Keven Paulino
void License(vector<string> att)
{
    att.push_back("LICENSE");
    ReadFile(att);
}

// Help text
// Keven Paulino
string Help(string action)
{
    map<string, string> help;
    help["hello"] = "* Function name *  No parameters\n";
    help["Ndirec"] = "* Function name *  directory path ... \n";
    help["Ddirec"] = "* Function name *  directory path ... \n";
    help["Mdirec"] = "* Function name *  from path , to path ...\n";
    help["Rdirec"] = "* Function name *  directory path , new name ...\n";
    help["Cfile"] = "* Function name *  file path ...\n";
    help["Rfile"] = "* Function name *  file path ...\n";
    help["Ufile"] = "* Function name *  file path ...\n";
    help["Dfile"] = "* Function name *  file path ...\n";
    help["clear"] = "* Function name *  No parameters\n";
    help["readme"] = "* Function name *  No parameters\n";
    help["license"] = "* Function name *  No parameters\n";
    help["list"] = "* Function name *  No parameters.\n";

    return help.at(action);
}

// Pedro Arraiza
void ListCommands(vector<string> att)
{
    cout << BLUE << "Available commands:\n"
         << RESET;

    vector<std::pair<string, string>> commands = {
        {"hello", " - print 'Hello World'"},
        {"Ndirec", " - create directory"},
        {"Ddirec", " - delete directory"},
        {"Rdirec", " - rename directory"},
        {"Mdirec", " - move directory"},
        {"Cfile", " - create file"},
        {"Rfile", " - read file"},
        {"Ufile", " - update file - if it doesnt exists creates it"},
        {"Dfile", " - delete file"},
        {"clear", " - clear the screen"},
        {"readme", " - show README.md"},
        {"license", " - show LICENSE"},
        {"list", " - list all commands"},
        {"close", " - exit the program"}};

    for (int i = 0; i < commands.size(); i++)
    {
        cout << GREEN << " - " << commands[i].first << RESET << commands[i].second << "\n";
    }
}
// Keven Paulino
void Hello(vector<string> att)
{
    cout << GREEN << "Hello World\n"
         << RESET;
}

// Directory functions
// Keven Paulino
void NewDirectory(vector<string> att)
{
    for (std::filesystem::path path : att)
    {
        path = path.make_preferred();
        if (!PathExists(path) && RootExists(path))
        {
            std::filesystem::create_directories(path);
            cout << GREEN << "Created directory: " << path << RESET << "\n";
        }
        else if (PathExists(path))
        {
            cout << YELLOW << "ERROR: Directory already exists: " << path << RESET << "\n";
        }
        else
        {
            cout << RED << "ERROR: Root path does not exist: " << path.root_path() << RESET << "\n";
        }
    }
}

// Keven Paulino
void DeleteDirectory(vector<string> att)
{
    for (std::filesystem::path path : att)
    {
        path = path.make_preferred();
        if (PathExists(path))
        {
            if (std::filesystem::remove_all(path))
            {
                cout << GREEN << "Deleted directory: " << path << RESET << "\n";
            }
            else
            {
                cout << RED << " ERROR: Directory cannot be removed: " << path << RESET << "\n";
            }
        }
        else
        {
            cout << RED << "ERROR: Directory does not exist: " << path << RESET << "\n";
        }
    }
}

// Keven Paulino
void RenameDirectory(vector<string> att)
{
    std::filesystem::path oName;
    std::filesystem::path nName;
    if (att.size() % 2 == 1)
    {
        cout << RED << " ERROR: Function needs an even number of arguments\n"
             << RESET;
    }
    else
    {
        for (size_t i = 0; i < att.size(); i += 2)
        {
            oName = att.at(i);
            nName = oName.parent_path() / att.at(i + 1);
            oName = oName.make_preferred();
            nName = nName.make_preferred();

            if (PathExists(oName) && !PathExists(nName))
            {
                try
                {
                    std::filesystem::rename(oName, nName);
                    cout << GREEN << "Renamed to: " << nName << RESET << "\n";
                }
                catch (const std::exception &e)
                {
                    cout << RED << "ERROR: Invalid name: " << att.at(i + 1) << RESET << "\n";
                }
            }
            else
            {
                cout << RED << "ERROR: Original not found or new name already exists\n"
                     << RESET;
            }
        }
    }
}

// Keven Paulino
void MoveDirectory(vector<string> att)
{
    std::filesystem::path fromPath;
    std::filesystem::path toPath;

    if (att.size() % 2 == 1)
    {
        cout << RED << "ERROR: Function needs an even number of arguments\n"
             << RESET;
    }
    else
    {
        for (size_t i = 0; i < att.size(); i += 2)
        {
            fromPath = att.at(i);
            toPath = att.at(i + 1) + "/" + fromPath.filename().string();
            fromPath = fromPath.make_preferred();
            toPath = toPath.make_preferred();

            if (PathExists(fromPath) && !PathExists(toPath))
            {
                try
                {
                    std::filesystem::copy(fromPath, toPath, std::filesystem::copy_options::recursive);
                    if (std::filesystem::remove_all(fromPath))
                    {
                        cout << GREEN << "Deleted directory: " << fromPath << RESET << "\n";
                    }
                    else
                    {
                        cout << RED << " ERROR: Directory cannot be removed: " << fromPath << RESET << "\n";
                    }
                    cout << GREEN << "Moved directory to: " << toPath << RESET << "\n";
                }
                catch (...)
                {
                    cout << RED << "ERROR: Failed to move\n"
                         << RESET;
                }
            }
            else
            {
                cout << RED << "ERROR: Invalid from or to path\n"
                     << RESET;
            }
        }
    }
}

// File functions
// Pedro Arraiza
void CreateFile(vector<string> att)
{
    for (const auto &path : att)
    {
        std::ofstream file(path);
        if (file)
        {
            cout << GREEN << "Created file: " << path << RESET << "\n";
        }
        else
        {
            cout << RED << "ERROR: Failed to create file: " << path << RESET << "\n";
        }
    }
}

// Pedro Arraiza
void ReadFile(vector<string> att)
{
    for (const auto &path : att)
    {
        std::ifstream file(path);
        if (file)
        {
            cout << YELLOW << "Reading file: " << path << RESET << "\n";
            string line;
            while (getline(file, line))
            {
                cout << line << "\n";
            }
        }
        else
        {
            cout << RED << "ERROR: Failed to read file: " << path << RESET << "\n";
        }
    }
}

// Pedro Arraiza
void UpdateFile(vector<string> att)
{
    string text;
    std::ofstream file(att[0], std::ios::app);
    if (file)
    {
        cout << "Enter text:\n";
        getline(cin, text);
        file << text << "\n";
        cout << GREEN << "Updated file: " << att[0] << RESET << "\n";
    }
    else
    {
        cout << RED << "ERROR: Failed to update file: " << att[0] << RESET << "\n";
    }
}

// Pedro Arraiza
void DeleteFile(vector<string> att)
{
    for (const auto &path : att)
    {
        if (std::filesystem::remove(path))
        {
            cout << GREEN << "Deleted file: " << path << RESET << "\n";
        }
        else
        {
            cout << RED << "ERROR: Failed to delete file: " << path << RESET << "\n";
        }
    }
}

// Clear screen
// Pedro Arraiza
void ClearScreen(vector<string> att)
{
// the compiler detects you're on a Windows system
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
