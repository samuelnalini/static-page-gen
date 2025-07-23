#include "headers/generator.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

void Generator::readFile(std::ifstream& file)
{
    std::ostringstream ss;
    ss << file.rdbuf();
    source = ss.str();

    while (!isAtEOF())
    {
        switch (peek())
        {
            case '@':
                {
                    if (peek() == '{')
                    {
                        declareVar();
                    }
                    break;
                }
            case '{':
                {
                    readVar();
                    break;
                }
        }
    }

    generateStaticPage();
}

void Generator::generateStaticPage()
{

    std::sort(readVariables.begin(), readVariables.end(), [](const VarToken& a, const VarToken& b) {
            return a.startIdx > b.startIdx;
            });

    for (VarToken var : readVariables)
    {
        std::string replStr{ declVariables[var.varName] };
        source.replace(var.startIdx - 1, var.nameLen + 2, replStr);
    }

    std::ofstream outputFile("static.html");
    outputFile << source;
    std::cout << "Generated static.html\n";
}

void Generator::readVar()
{
    int startingIdx{ fileIdx };
    while (peek() != '}')
    {
        if (isAtEOF())
        {
            std::cerr << "Invalid statement at " << lineNum << '\n';
            exit(1);
        }
    }

    std::string varName{ source.substr(startingIdx, (fileIdx - startingIdx - 1)) };

    auto mapKey{ declVariables.find(varName) };
    if (mapKey == declVariables.end())
    {
        std::cerr << "Attempted to use undeclared variable " << varName << " on line " << lineNum << '\n';
        exit(1);
    }

    VarToken var(lineNum, startingIdx, (fileIdx - startingIdx - 1),  varName);

    readVariables.emplace_back(var);
    //std::cout << "\nUSED VARIABLE: " << varName << "\n";
    //std::cout << "EOF: " << (fileIdx == isAtEOF()) << "\nstartingIdx: " << startingIdx << "\nfileIdx: " << fileIdx << "\nlineNum: " << lineNum << "\nnameLen: " << (fileIdx - startingIdx - 1) << '\n';

}

void Generator::declareVar()
{
    int startingIdx{ fileIdx };
    int startingLine{ lineNum };
    std::string varName;
    while (peek() != '}')
    {
        if (isAtEOF())
        {
            std::cerr << "Invalid declaration on line " << startingLine << '\n';
            exit(1);
        }
    }

    varName = source.substr(startingIdx, (fileIdx - startingIdx - 1));

    while (peek() != '=')
    {
        if (isAtEOF())
        {
            std::cerr << "Invalid declaration on line " << startingLine << '\n';
            exit(1);
        }
    }


    while (peek() != '"') // First quote
    {
        if (isAtEOF())
        {
            std::cerr << "Invalid declaration on line " << startingLine << '\n';
            exit(1);
        }
    }

    std::string varVal;
    startingIdx = fileIdx;

     while (peek() != '"') // End quote
    {
        if (isAtEOF())
        {
            std::cerr << "Invalid declaration on line " << startingLine << '\n';
            exit(1);
        }
    }   

    if (peek() != ';')
    {
        std::cerr << "Unfinished declaration on line " << startingLine << '\n';
        exit(1);
    }


    varVal = source.substr(startingIdx, fileIdx - startingIdx - 2);
    declVariables[varName] = varVal;

    //std::cout << "DECLARED VARIABLE: " << varName << "\nValue: " << varVal << "\nlineNum: " << lineNum << "\nstartingIdx: " << startingIdx << "\nfileIdx: " << fileIdx << "\nnameLen: " << (fileIdx - startingIdx - 2) << '\n';
}

void Generator::advance()
{
    fileIdx++;
}

char Generator::peek()
{
    char c {source[fileIdx]};
    if (c == '\n') lineNum++;
    advance();
    return c;
}

bool Generator::isAtEOF()
{
    return (fileIdx >= source.length());
}
