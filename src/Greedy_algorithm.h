#pragma once

#include "CFLP.h"
#include "simple_algorithm.h"

class Greedy_algorithm : public simple_algorithm
{
private:
    CFLP _Solu;
public:
    Greedy_algorithm() {};
    ~Greedy_algorithm() {};

    void getSolution() override;
    void load_file(string) override;
    string to_string() const override;
    size_t getFitness() const override;
};

void Greedy_algorithm::getSolution()
{
    _Solu.greedy_init();
}

void Greedy_algorithm::load_file(string file) 
{
    CFLP::load_file(file);
}

inline string Greedy_algorithm::to_string() const
{
    return _Solu.to_string();
}

inline size_t Greedy_algorithm::getFitness() const
{
    return _Solu.getFitness();
}