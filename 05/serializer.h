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
    Error operator()(ArgsT... args) {
        return process(args...);
    }

    template <class T>
    Error process(T&& val) {
        std::type_index ti(typeid(val));
        if (ti == std::type_index(typeid(uint64_t))) {
            out_ << val << Separator;
        } else if (ti == std::type_index(typeid(bool))) {
            if (val) {
                out_ << "true" << Separator;
            } else {
                out_ << "false" << Separator;
            }
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        std::type_index ti(typeid(val));
        if (ti == std::type_index(typeid(uint64_t))) {
            out_ << val << Separator;
        } else if (ti == std::type_index(typeid(bool))) {
            if (val) {
                out_ << "true" << Separator;
            } else {
                out_ << "false" << Separator;
            }
        } else {
            return Error::CorruptedArchive;
        }
        return process(std::forward<Args>(args)...);
    }
    
private:
    std::ostream& out_;
};

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

    template <class T>
    Error process(T&& val) {
        std::string temp;
        in_ >> temp;
        if (temp == "true") {
            val = true;
        } else if (temp == "false") {
            val = false;
        } else {
            try {
                val = std::stoi(temp);
            } catch (std::invalid_argument& err) {
                return Error::CorruptedArchive;
            }
        }
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        std::string temp;
        in_ >> temp;
        if (temp == "true") {
            val = true;
        } else if (temp == "false") {
            val = false;
        } else {
            try {
                val = std::stoi(temp);
            } catch (std::invalid_argument& err) {
                return Error::CorruptedArchive;
            }
        }
        return process(std::forward<Args>(args)...);
    }

private:
    std::istream& in_;
};
