#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <scanner.hpp>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;

namespace Global
{
    fs::path filePath {};
    std::vector<fs::path> includePaths {};
    std::ifstream file {};
    std::vector<scanner::Token> tokens;
    bool outputTokens {false};
    std::filesystem::path tokenOutputPath;
    std::ofstream tokenOutputFile;
    
    const std::string helpMessage {
        "TODO HELP MESSAGE"
    };
}

void helpCommand()
{
    std::cout << Global::helpMessage << std::endl;
    std::exit(EXIT_SUCCESS);
}

void includeCommand(int& argIndex, int argc, char** argv)
{
    for(argIndex += 1; argIndex < argc and argv[argIndex][0] != '-'; argIndex += 1)
    {
        fs::path includePath {argv[argIndex]};
        if(not fs::is_directory(includePath))
        {
            std::cout << "The path \"" << includePath.string() << "\" is not a valid directory." << std::endl;
            std::exit(EXIT_FAILURE);
        }
        Global::includePaths.push_back(includePath);
    }
    if(Global::includePaths.empty())
    {
        std::cout << "Please provide one or more paths after the \"--include\" flag" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    argIndex -= 1;
}

void outputTokensCommand(int& argIndex, int argc, char** argv)
{
    Global::outputTokens = true;
    argIndex += 1;
    if(argIndex >= argc)
    {
        std::cout << "Please provide a path to the token output file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    Global::tokenOutputPath = fs::path(argv[argIndex]);
    if(not fs::exists(Global::tokenOutputPath)) {
        std::cout << "The path provided to output the tokens is invalid" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if(fs::is_directory(Global::tokenOutputPath))
    {
        Global::tokenOutputPath /= "tokens.txt";
        Global::tokenOutputFile.open(Global::tokenOutputPath);
        if(not Global::tokenOutputFile.is_open())
        {
            std::cout << "Error creating token.txt in the provided path" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    else if(Global::tokenOutputPath.extension() == ".txt")
    {
        Global::tokenOutputFile.open(Global::tokenOutputPath);
        if(not Global::tokenOutputFile.is_open())
        {
            std::cout << "Error opening the file provided to be the token output" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    else
    {
        std::cout << "Please provide a file with the .txt extension to be a token output" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void openCodeFile(char* codeFilePath)
{
    Global::filePath = fs::path(codeFilePath);
    Global::file.open(Global::filePath);
    if(not Global::file)
    {
        std::cout << "The path \"" << Global::filePath << "\" is invalid" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void parseCommandLine(int argc, char** argv)
{
    if(argc == 1)
    {
        std::cout << "Please provide arguments to the compiler, if you need help, type \"futz --help\"" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if(std::string {argv[1]} == "--help")
        helpCommand();
    for(int argumentIndex { 2 }; argumentIndex < argc; argumentIndex += 1)
    {
        std::string argument { argv[argumentIndex] };
        if(argument == "--include" or argument == "-i")
        {
            includeCommand(argumentIndex, argc, argv);
            continue;
        }
        else if(argument == "--output_tokens" or argument == "-t")
        {
            outputTokensCommand(argumentIndex, argc, argv);
            continue;
        }
        else
        {
            std::cout << "Invalid compile flag \"" << argument << "\", type \"futz --help\" for instructions" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    openCodeFile(argv[1]);
}

void finish()
{
    if(Global::outputTokens)
        Global::tokenOutputFile.close();
    Global::file.close();
}

int main(int argc, char** argv)
{
    parseCommandLine(argc,argv);
    std::size_t lineNumber {0};
    std::stringstream buffer;
    buffer << Global::file.rdbuf();
    std::string code {buffer.str()};
    Global::tokens = scanner::tokenize(code);
    if(Global::outputTokens)
        for(const auto& token : Global::tokens)
            Global::tokenOutputFile << token << '\n';
    finish();
    std::cout << "Compilation success" << std::endl;
    return EXIT_SUCCESS;
}