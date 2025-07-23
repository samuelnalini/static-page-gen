#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

typedef struct varToken {
    std::string varName;
    int startIdx;
    int nameLen;
    int lineNum;

    varToken(int lineNum, int startIdx, int nameLen, std::string& varName)
        : lineNum(lineNum)
        , startIdx(startIdx)
        , nameLen(nameLen)
        , varName(varName)
    {}
} VarToken;

class Generator  
{
public:
    Generator() {};
    void readFile(std::ifstream& file);
private:
    void advance();
    char peek();
    bool isAtEOF();
    void readVar();
    void declareVar();
    void generateStaticPage();
private:
    std::string source;
    int lineNum = 1;
    int fileIdx;
    std::vector<VarToken> readVariables;
    std::unordered_map<std::string, std::string> declVariables;
};
