#include "headers/generator.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <iomanip>

void Generator::readFile(std::ifstream& file)
{
    std::ostringstream ss;
    ss << file.rdbuf();
    source = ss.str();

    while (!isAtEOF())
    {
        switch (source[fileIdx])
        {
            case '@':
                {
                    if (peek() == '{')
                    {
                        advance(); // consume the @ 
                        advance(); // consume the {
                        declareVar();
                    }
                    break;
                }
            case '{':
                {
                    advance();
                    readVar();
                    break;
                }
        }

        advance();
    }

    generateStaticPage();
}

void Generator::generateStaticPage()
{
    std::regex varPattern(R"(\{(\w+)\})");
    std::string result;
    auto searchStart{ source.cbegin() };
    std::smatch match;

    while (std::regex_search(searchStart, source.cend(), match, varPattern))
    {
        result.append(searchStart, match.prefix().second);
        
        std::string varName = match[1].str();

        auto it = declVariables.find(varName);
        if (it != declVariables.end())
        {
            result.append(it->second);
        } else
        {
            result.append(match[0].str());
        }

        searchStart = match.suffix().first;
    }

    result.append(searchStart, source.cend());

    std::ofstream outputFile("static.html");
    outputFile << result;
    std::cout << "Generated static.html\n";
}

void Generator::readVar()
{
    int startingIdx{fileIdx};
    std::string varName;
    while (source[fileIdx] != '}')
    {
        if (isAtEOF())
        {
            std::cerr << "Invalid statement at " << lineNum << '\n';
            exit(1);
        }

        varName += source[fileIdx];
        advance();
    }

    advance(); // Consume the '}'

    auto mapKey{ declVariables.find(varName) };
    if (mapKey == declVariables.end())
    {
        std::cerr << "Attempted to use undeclared variable " << varName << " on line " << lineNum << '\n';
        exit(1);
    }

    VarToken var(lineNum, startingIdx, (fileIdx - startingIdx),  varName);

    readVariables.emplace_back(var);
}

void Generator::declareVar()
{
    int startingIdx{ fileIdx + 1 }; // Starts at '{' so I need to increment it by 1
    int startingLine{ lineNum };
    std::string varName;

    while (source[fileIdx] != '}') {
        if (isAtEOF())
        {
            std::cerr << "Invalid declaration on line " << startingLine << '\n';
            exit(1);
        }
        varName += source[fileIdx];
        advance();
    } 
    advance(); // consumes the '}'
               
    //varName = source.substr(startingIdx, ((fileIdx) - startingIdx));
    std::cout << "Declaring variable " << varName << ' ';

    while (source[fileIdx] != '=')
    {
        if (isAtEOF())
        {
            std::cerr << "Invalid declaration on line " << startingLine << '\n';
            exit(1);
        }
        advance();
    }

    advance(); // Consume the =
    std::cout << "as ";


    while (source[fileIdx] != '"') // Start quote
    {
        if (isAtEOF())
        {
            std::cerr << "Invalid declaration on line " << startingLine << '\n';
            exit(1);
        }
        advance();
    }

    advance(); // Consumes the quote

    int quotes{1}; // Includes the starting quote
    bool exempt{false};
    std::string varVal;
    startingIdx = fileIdx;

    //std::cout << "Starting at " << source[startingIdx] << '\n';

     while (source[fileIdx] != ';') // Declaration end
    {
        if (isAtEOF())
        {
            std::cerr << "Unfinished declaration on line " << startingLine << '\n';
            exit(1);
        }
        
        if (source[fileIdx] == '\\') {
            exempt = true;
            advance();
        }

        if (source[fileIdx] == '"') {
            if (!exempt) {
                ++quotes;
                //std::cout << " " << quotes << " ";
            }
            else {
                exempt = false;
                //std::cout << " exempt off ";
            }
        }

        if (quotes < 2) {
            std::cout << source[fileIdx];
            varVal += source[fileIdx];
        }
        else if (quotes > 2) {
            std::cerr << "Invalid declaration on line " << startingLine << ": " << "too many quotes -> " << quotes << '\n';
            exit(1);
        }

        advance();
    }

    advance(); // Consume the ';'
    std::cout << '\n';
    declVariables[varName] = varVal;
}

void Generator::advance()
{
    fileIdx++;
    if (source[fileIdx] == '\n') lineNum++;
}

char Generator::peek()
{
    if (source[fileIdx + 1] >= source.length()) return '\0';

    char c {source[fileIdx + 1]}; 
    return c;
}

bool Generator::isAtEOF()
{
    return (fileIdx >= source.length());
}
