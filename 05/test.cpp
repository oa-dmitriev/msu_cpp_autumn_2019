#include <iostream>
#include "serializer.h"
#include "deserializer.h"
#include <sstream>
#include <cassert>

struct Data {
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer) {
        return deserializer(a, b, c);
    }
};

struct BadData {
    double a;
    int b;
    short c;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer) {
        return deserializer(a, b, c);
    }
};

int main() {
    Data x {1, true, 2};

    std::stringstream stream;

    Serializer serializer(stream);
    serializer.save(x);

    Data y {0, false, 0};

    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);
    assert(err == Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
    stream.str(std::string());

    x.a = 100;
    x.b = false;
    serializer.save(x);
    deserializer.load(y);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);

    stream.str(std::string());
    BadData z {3.14, 2, 3};
    serializer.save(z);
    
    const Error err1 = deserializer.load(y);
    assert(err1 == Error::CorruptedArchive);

    return 0;
}