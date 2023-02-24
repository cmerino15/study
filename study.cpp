#include <iostream>
#include <string>     // to convert args to strings
#include <windows.h>  // for www and for c_str() I believe, and for setconsoletextattribute
#include <shellapi.h> // for www
#include <fstream>    // for quizmaker

void help_menu(int argc, char *argv[]);
void quiz_maker();
void create_quiz();
void take_quiz();
void question_in_out(std::string quiz_name);
int main(int argc, char *argv[])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);

    if (argc == 1)
    {
        std::cout << "No args entered\n";
        std::cout << "Try a.exe help\n";
    }
    else if (argc == 2 || argc == 3)
    {

        std::string argv1(argv[1]); // need to convert

        if (argv1 == "help" || argv1 == "h" || argv1 == "HELP" || argv1 == "--h" || argv1 == "/h" || argv1 == "-h") // need to convert
        {
            help_menu(argc, argv);
        }
        else if (argv1 == "translate")
        {
            std::string engword(argv[2]);
            std::string site = "https://translate.google.com/#view=home&op=translate&sl=en&tl=fr&text=" + engword;
            // Works -- see www version
            ShellExecute(0, 0, site.c_str(), 0, 0, SW_SHOW); // note the precompiler error of LPCWSTR vs Compiler error of LPCSTR
        }
        else if (argv1 == "www")
        {
            std::string host(argv[2]);
            std::string site = "http://www." + host;
            // Works
            // ShellExecute(0, 0, "http://www.google.com", 0, 0, SW_SHOW);
            ShellExecute(0, 0, site.c_str(), 0, 0, SW_SHOW); // note the precompiler error of LPCWSTR vs Compiler error of LPCSTR

            // Still trying to figure out how to get rid of precompiler error...may be an error itself...
            // const wchar_t* site = host;
            // std::cout << host;
            // std::cout << argv[2];
            // int url_length = site.length();
            // char url[100]; // can't use url_length here because it's not constant...:(
            // std::cout << site;
            // std::string url = site.c_str());
            // ShellExecute(0, 0, site.c_str(), 0, 0, SW_SHOW);
            // ShellExecute(0,0, L"site",0,0,SW_SHOW);
            // std::wstring site = L"http://www.google.com";
            // ShellExecute(0, 0, site.c_str(), 0, 0, SW_SHOW);
            // std::wstring site = L"http://www.yahoo.com";
        }
        else if (argv1 == "cmd")
        {
            system("cmd");
        }
        else if (argv1 == "quizmaker")
        {
            quiz_maker();
        }
        else
        {
            std::cout << "Not a valid command\nTry a.exe help\n";
        }
    }
    else
    {
        std::cout << "Too many arguments\n";
        std::cout << "Try a.exe help\n";
    }

    SetConsoleTextAttribute(hConsole, 7);
    return 0;
}

void help_menu(int argc, char *argv[])
{

    std::cout << "HELP MENU\n";
    std::cout << "---------\n";
    std::cout << "1. Using the www arg \n";
    std::cout << "2. using the translate arg \n";
    std::cout << "3. Exit the program\n";
    std::cout << "4. Arg info \n";
    std::cout << "5. Using the cmd arg\n";
    std::cout << "6. How to open the camera\n";
    std::cout << "7. Using the quizmaker arg \n";
    std::cout << "Enter a choice (1 -7), the press enter";
    int choice = 0;
    std::cin >> choice;
    std::cout << "\n";
    if (choice == 1)
    {
        std::cout << "To go to a website enter a.exe www hostname\n";
        std::cout << "For example a.exe www google.com or a.exe www MSDN.net\n";
    }
    else if (choice == 2)
    {
        std::cout << "To translate a word to french use translate yourword\n";
        std::cout << "Alternatively you can use quotes to translate an entire phrase\n";
    }
    else if (choice == 3)
    {
        exit(0);
    }
    else if (choice == 4)
    {
        std::cout << "The number of args entered was " << argc << std::endl;
        std::cout << "And they were:   ";
        for (int i = 0; i < argc; i++)
        {
            std::cout << argv[i] << "   ";
        }
    }
    else if (choice == 5)
    {
        std::cout << "To open command line try cmd\n";
        std::cout << "Then `help` for help\n";
    }
    else if (choice == 6)
    {
        std::cout << "To open camera. Open cmd line and try `start microsoft.windows.camera:`\n";
    }
    else if (choice == 7)
    {
        std::cout << "To start quizmaker try a.exe quizmaker\n";
    }
}

void quiz_maker()
{
    std::cout << "1. Create a New Quiz\n";
    std::cout << "2. Take a  Quiz\n";
    std::cout << "3. Modify a Quiz\n";
    std::cout << "Enter a value...\n";
    int quiz_choice;
    std::cin >> quiz_choice;
    switch (quiz_choice)
    {
    case 1:
        create_quiz();
        break;

    case 2:
        take_quiz();
        break;
    default:
        std::cout << "Invalid Choice Entered\n";
        break;
    }
}

void create_quiz()
{
    std::ofstream fout;
    std::ifstream fin;

    std::cout << "What would you like to name the quiz?\n";
    std::cout << "The quiz will be saved as name.txt\n";
    std::string name, dummystring;
    std::cin.ignore(1, '\n');
    getline(std::cin, name);

    fout.open("quizlog.txt", std::ios::app); // just to create for the first time
    fout.close();

    // checks for duplicates
    std::string dummy;
    bool dupl = 0;
    fin.open("quizlog.txt");
    do
    {
        getline(fin, dummy);
        if (dummy == name)
        {
            dupl = 1;
        }

    } while (fin);
    fin.close();

    if (dupl == 1)
    {
        std::cout << "Error: That quiz already exists\n";
    }
    else
    {
        fout.open("quizlog.txt", std::ios::app);
        fout << name << std::endl;
        fout.close();
        question_in_out(name);
    }
}
void take_quiz()
{
    std::ifstream fin;
    std::string quiz_name, question, answer, dummy;
    int score = 0, total = 0;
    std::cout << "Enter the name of the quiz you want to take\n";
    std::cin.ignore(1, '\n');
    getline(std::cin, quiz_name);
    fin.open(quiz_name + ".txt");
    if (fin)
    {
        do
        {
            getline(fin, question);
            getline(fin, answer);
            std::cout << question << std::endl;
            getline(std::cin, dummy);
            if (dummy == answer)
            {
                std::cout << "Correct!\n";
                score++;
            }
            else
            {
                std::cout << "Incorrect!\n";
            }
            total++;
        } while (fin);
        fin.close();
        std::cout << "You got " << score << " out of " << total << std::endl;
    }
    else
    {
        std::cout << "Error: That quiz does not exist\n";
    }
}
void question_in_out(std::string quiz_name)
{
    std::string question, answer;
    char choice;
    do
    {
        std::ofstream fout;
        fout.open(quiz_name + ".txt", std::ios::app);
        std::cout << "Enter a question and hit enter\n";
        getline(std::cin, question);
        fout << question << std::endl;
        std::cout << "Enter an answer and hit enter\n";
        getline(std::cin, answer);
        fout << answer << std::endl;

        std::cout << "Enter more questions? Enter y for yes or anything else for no\n";
        std::cin >> choice;
        std::cin.ignore(1, '\n');
    } while (choice == 'y');
}
