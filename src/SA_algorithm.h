#pragma once
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "CFLP.h"
#include "simple_algorithm.h"

class SA_algorithm : public simple_algorithm
{
private:
    CFLP _Solu;
public:
    SA_algorithm() {};
    ~SA_algorithm() {};

    void getSolution() override;
    void load_file(string) override;
    string to_string() const override;
    size_t getFitness() const override;
};

void SA_algorithm::getSolution()
{
    _Solu.greedy_init();

    /* 
    auto bestSolu = _Solu;
    int num = 0;
    srand(time(0));
    while(num < 1000) {
        int randnum = rand() % 6;
        if (randnum == 0)
            _Solu.openFacility();
        else if (randnum < 3)
            _Solu.closeFacility();
        else
            _Solu.swapFacility();

        
        _Solu.swapCustomer();
        _Solu.moveCustomer();
        if (_Solu.getFitness() < bestSolu.getFitness()) {
            num = 0;
            bestSolu = _Solu;
        }
        else num++;
            
    }
    _Solu = move(bestSolu);
    */

   auto bestSolu = _Solu;
   auto temp = _Solu;
   srand(time(0));
   int outnum = 0;
   double temperture = 0.001;
   //外循环终止条件：2次内循环最优解都没有改变且温度小于0.0001
   while (outnum < 2 || temperture > 0.0001) {
       int innum = 0;
       size_t bestFitness = bestSolu.getFitness();
       //外循环终止条件：4次循环最优解都没有改变
       while (innum < 4) {
           int randnum = rand() % 6;
            if (randnum == 0)
                temp.openFacility();
            else if (randnum < 3)
                temp.closeFacility();
            else
                temp.swapFacility();

            temp.swapCustomer();
            temp.moveCustomer();

            double diff = double((int)temp.getFitness() - (int)_Solu.getFitness()) / _Solu.getFitness();
            if (diff < 0) {
                innum = 0;
                _Solu = temp;
                if (temp.getFitness() < bestSolu.getFitness())
                    bestSolu = _Solu;
            } else if (double(rand() % 1000) / 1000 < exp(-diff / temperture)) {
                _Solu = temp;
                innum++;
            } else {
                temp = _Solu;
                innum++;
            }
       }
       if (bestFitness == bestSolu.getFitness()) outnum++;
       else outnum = 0;
       temperture = temperture * 0.99;
   }
}

void SA_algorithm::load_file(string file) 
{
    CFLP::load_file(file);
}

inline string SA_algorithm::to_string() const
{
    return _Solu.to_string();
}

inline size_t SA_algorithm::getFitness() const
{
    return _Solu.getFitness();
}