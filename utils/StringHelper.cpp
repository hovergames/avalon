//
//  StringHelper.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#include "StringHelper.h"
#include <vector>
#include <string>
#include <sstream>

namespace StringHelper
{
    void replace(std::string& str, const std::string& from, const std::string& to) {
        if (from.empty())
            return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    void trim(std::string &str)
    {
        int pos = str.find_first_not_of(" ");
        if (pos >= 0)
            str.erase(0, pos);

        pos = str.find_last_not_of(" ");
        if (pos >= 0)
            str.erase(pos + 1);
    }

    std::vector<std::string> split(const std::string &text, char sep)
    {
        std::vector<std::string> tokens;
        int start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
    }
}