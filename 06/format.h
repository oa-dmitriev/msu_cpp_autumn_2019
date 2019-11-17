#pragma once

#include <vector>
#include <sstream>

class Parser {
    static constexpr char Separator = ' ';
    std::vector<std::string> params_;
    std::stringstream ss_;
    std::string s_;
public:
    template <class... ArgsT>
    Parser(const std::string& s, ArgsT&&... args) : s_(s) {
        pushParamsToStream(std::forward<ArgsT>(args)...);
    }

    template <class T>
    void pushParamsToStream(T&& val) {
        ss_ << val;
    }

    template <class T, class... ArgsT>
    void pushParamsToStream(T&& val, ArgsT&&... args) {
        ss_ << val << Separator;
        pushParamsToStream(std::forward<ArgsT>(args)...);
    }

    void extractParamsFromStream();
    int getPos(const std::string& s);
    std::string getFormattedString();
};

template <class... ArgsT>
std::string format(const std::string& s, ArgsT&&... args) {
    Parser p(s, std::forward<ArgsT>(args)...);
    return p.getFormattedString();
}

inline std::string format(const std::string& s) {
    return s;
}