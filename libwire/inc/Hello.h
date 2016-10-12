#ifndef HELLO_H
#define HELLO_H

#include <string>

class Hello {
public:
    Hello() = default;
    ~Hello() = default;

    std::string yo() const;
};

#endif // HELLO_H
