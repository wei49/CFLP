#pragma once

#include <string>
using std::string;
class simple_algorithm
{
public:
    virtual ~simple_algorithm() noexcept{};
    virtual void getSolution() = 0;
    virtual void load_file(string) = 0;
    virtual string to_string() const = 0;
    virtual size_t getFitness() const = 0;
};
