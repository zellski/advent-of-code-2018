#pragma once

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

inline std::vector<std::string> str_file_contents(std::string file_name)
{
    std::ifstream in_stream{file_name};
    std::vector<std::string> result;
    std::string line;
    while (std::getline(in_stream, line))
    {
        result.emplace_back(line);
    }
    return result;
}

inline std::vector<int> int_file_contents(std::string file_name)
{
    std::ifstream in_stream{file_name};
    std::vector<int> result;
    std::string line;
    while (std::getline(in_stream, line))
    {
        result.emplace_back(std::stoi(line));
    }
    return result;
}
