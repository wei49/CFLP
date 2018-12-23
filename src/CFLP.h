#ifndef _CFLP_H
#define _CFLP_H

#include <vector>
#include <iostream>
#include <string>
#include <list>

using namespace std;

//Capacitated Facility Location Problem
class CFLP
{
    struct facility {
        size_t capacity, cost;
        facility(size_t _capacity, size_t _cost) : capacity{_capacity}, cost{_cost} {};
        facility() {};
    };

private:
    static vector<facility> facilities;
    static vector<size_t> demands;
    static vector<size_t> assign_cost;
    
    size_t get_cost(size_t, size_t);

    void isFeasible();

    //vector<list<size_t>> genes;
    vector<size_t> facility_occpy;
    vector<size_t> custumer_assign;
    size_t _fitness;

public:
    static void load_file(string);
    void greedy_init();
    void random_init();
    void calFitness();
    void swapCustomer();
    void moveCustomer();

    void closeFacility();
    void swapFacility();
    void openFacility();

    size_t getFitness() const { return _fitness; };

    string to_string() const ;



    CFLP() {};
};


#endif