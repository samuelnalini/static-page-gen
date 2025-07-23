#include "headers/generator.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: gen <file>\n";
        return 1;
    }

    std::filesystem::path inputPath(argv[1]);

    if (!std::filesystem::exists(inputPath))
    {
        std::cerr << "Invalid input file\n";
        return 1;
    }

    if (inputPath.extension() != ".html")
    {
        std::cerr << "Invalid file extension\n";
        return 1;
    }

    std::ifstream inputFile(inputPath, std::ios::in);

    if (!inputFile.is_open())
    {
        std::cerr << "Failed to open input file!\n";
        return 1;
    }

    class Generator generator;
    generator.readFile(inputFile);
}
