#pragma once

#include "serializer.h"

class Deserializer {
public:
    explicit Deserializer(std::istream& in)
            : in_(in) {}
    
    template <class T>
    Error load(T& object) {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        Error err = process(val);
        if (err == Error::NoError) {
            return process(std::forward<Args>(args)...);
        }
        return Error::CorruptedArchive;
    }

    template <class T>
    Error process(T&& val) {
        return process(val);
    }

    Error process(bool& val) {
        std::string temp;
        in_ >> temp;
        if (temp == "true") {
            val = true;
        } else if (temp == "false") {
            val = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(uint64_t& val) {
        std::string temp;
        in_ >> temp;
        try {
            val = std::stoi(temp);
        } catch (std::invalid_argument& err) {
            return Error::CorruptedArchive;
        } catch (std::out_of_range& err) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template <class T>
    Error process(T& val) {
        return Error::CorruptedArchive;
    }
private:
    std::istream& in_;
};