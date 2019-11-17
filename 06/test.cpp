#include <iostream>
#include <cassert>
#include "format.h"

#define checkThrow(x) \
    do { \
        try { (x); } \
        catch (const std::runtime_error&) { break; } \
        catch (...) {} \
        std::cout << "expected runtime_error at line " << __LINE__ << '\n'; \
    } while(0)

struct Test {};

std::ostream& operator<<(std::ostream& out, const Test&) {
    out << "test";
    return out;
}

int main() {
    assert(format("") == "");
    assert(format("0") == "0");
    assert(format("{0}", 1) == "1");
    assert(format("{1}", 1, 2) == "2");
    assert(format("{0}{0}", "a") == "aa");
    assert(format("{0}", Test()) == "test");
    const Test test;
    assert(format("{0}", test) == "test");
    assert(format("{0}", const_cast<Test&>(test)) == "test");
    assert(format("{1} aaa {0}{2} {1}", 1, test, "kek") == "test aaa 1kek test");
    
    checkThrow(format("{", 1));
    checkThrow(format("{0", 1));
    checkThrow(format("}", 1));
    checkThrow(format("{1}", 1));
    checkThrow(format("{0}{1}{3}", 1, 2));
    checkThrow(format("{0{}", 1));
    checkThrow(format("{0a}", 1));
    checkThrow(format("0}", 1));
    checkThrow(format("{{0}", 1));
    return 0;
}