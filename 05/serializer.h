#pragma once

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <cstddef>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
            : out_(out) {}
    
    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

    template <class T, class... ArgsT>
    Error process(T&& val, ArgsT&&... args) {
        Error err = process(val);
        if (err == Error::NoError) {
            return process(std::forward<ArgsT>(args)...);
        }
        return Error::CorruptedArchive;
    }

    Error process(bool& val) {
        if (val) {
            out_ << "true" << Separator;
        } else {
            out_ << "false" << Separator;
        }
        return Error::NoError;
    }

    Error process(uint64_t& val) {
        out_ << val << Separator;
        return Error::NoError;
    }

    template <class T>
    Error process(T& val) {
        return Error::CorruptedArchive;
    }
    
private:
    std::ostream& out_;
};