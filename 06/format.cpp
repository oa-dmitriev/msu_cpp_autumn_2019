#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "format.h"

void Parser::extractParamsFromStream() {
    std::string s;
    while (ss_ >> s) {
        params_.push_back(s);
    }
}

int Parser::getPos(const std::string& s) {
    if (std::all_of(s.begin(), s.end(), isdigit)) {
        int argPos;
        try {
            argPos = std::stoi(s);
        } catch (std::out_of_range& err) {
            throw std::runtime_error("Key Error: " + s);
        }
        if (argPos >= params_.size() || argPos < 0) {
            throw std::runtime_error("Index Error: out of range");
        }
        return argPos;
    }
    throw std::runtime_error("Key Error: " + s);
}

std::string Parser::getFormattedString() {
    extractParamsFromStream();
    std::string formatted = "";
    for (int i = 0; i < s_.length(); ++i) {
        if (s_[i] == '{') {
            ++i;
            std::string arg = "";
            while (i < s_.length() && s_[i] != '}') {
                arg.push_back(s_[i]);
                ++i;
            }
            if (i == s_.length()) {
                throw std::runtime_error(
                    "Value Error: expected '}' before end of string");
            }
            int argPos = getPos(arg);
            formatted.append(params_[argPos]);
        } else if (s_[i] == '}') {
            throw std::runtime_error(
                "Value Error: Single '}' encountered in format string");
        } else {
            formatted.push_back(s_[i]);
        }
    }
    return formatted;
}