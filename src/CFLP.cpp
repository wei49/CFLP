#include <fstream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include "CFLP.h"

vector<CFLP::facility> CFLP::facilities = vector<CFLP::facility>();
vector<size_t> CFLP::demands = vector<size_t>();
vector<size_t> CFLP::assign_cost = vector<size_t>();


void CFLP::load_file(string file)
{
    fstream datain (file);

    int factory_num, consumer_num;
    datain >> factory_num >> consumer_num;

    facilities.clear();
    demands.clear();
    assign_cost.clear();

    facilities.reserve(factory_num);
    demands.reserve(consumer_num);
    assign_cost.reserve(consumer_num * factory_num);

    double temp, temp2;
    for (int i = 0; i < factory_num; i++) {
        datain >> temp >> temp2;
        facilities.emplace_back(facility(temp, temp2));
        // cout << facilities[i].capacity << "-" << facilities[i].cost << " ";
        // if (i % 10 == 9) cout << endl;
    }

    for (int i = 0; i < consumer_num; i++) {
        datain >> temp;
        demands.emplace_back(temp);
        // cout << demands[i] << " ";
        // if (i % 10 == 9) cout << endl;
    }

    for (int i = 0; i < consumer_num * factory_num; i++) {
        datain >> temp;
        assign_cost.emplace_back(temp);
        // cout << assign_cost[i] << " ";
        // if (i % 10 == 9) cout << endl;
    }
}

size_t CFLP::get_cost(size_t _facility, size_t _consumer)
{
    return assign_cost[_facility * demands.size() + _consumer];
}

string CFLP::to_string() const
{
    ostringstream out;
    for (auto i : custumer_assign)
        out << i << " ";
    return out.str();
}

void CFLP::greedy_init()
{
    facility_occpy.resize(facilities.size(), 0);
    for (auto &i : facility_occpy) i = 0;
    custumer_assign.resize(demands.size());

    for (int i = 0; i < demands.size(); i++) {
        size_t mincost = std::numeric_limits<size_t>::max();
        int min_index;
        //寻找能容纳其需求的且Assignment cost最小的工厂
        for (int j = 0; j < facilities.size(); j++) {
            if (get_cost(j, i) < mincost && demands[i] <= facilities[j].capacity - facility_occpy[j]) {
                min_index = j;
                mincost = get_cost(j, i);
            }
        }
        facility_occpy[min_index] += demands[i];
        custumer_assign[i] = min_index;
    }
    
    calFitness();
    // isFeasible();
}

void CFLP::calFitness()
{
    _fitness = 0;
    for (int i = 0; i < demands.size(); i++)
        _fitness += get_cost(custumer_assign[i], i);
    for (int i = 0; i < facilities.size(); i++)
        if (facility_occpy[i] != 0)
            _fitness += facilities[i].cost;
}

void CFLP::random_init() {};

void CFLP::swapCustomer()
{
    for (int i = 0; i < demands.size()-1; i++) {
        for (int j = i+1; j < demands.size(); j++) {
            int diff = demands[i] - demands[j];
            int fitness = get_cost(custumer_assign[j], i) - get_cost(custumer_assign[i], i)
                    + get_cost(custumer_assign[i], j) - get_cost(custumer_assign[j], j);

            if (fitness < 0 && ((diff >= 0 && diff <= facilities[custumer_assign[j]].capacity - facility_occpy[custumer_assign[j]])
                || (diff < 0 && -diff <= facilities[custumer_assign[i]].capacity - facility_occpy[custumer_assign[i]]))) {
                facility_occpy[custumer_assign[i]] -= diff;
                facility_occpy[custumer_assign[j]] += diff;

                _fitness += fitness;

                swap(custumer_assign[i], custumer_assign[j]);
            }
        }
    }
    // isFeasible();
}

void CFLP::moveCustomer()
{
    for (int i = 0; i < demands.size(); i++) {
        int index_f = custumer_assign[i];
        size_t fitness1 = get_cost(index_f, i);
        if (demands[i] == facility_occpy[index_f]) fitness1 += facilities[index_f].cost;

        int minfacility = index_f;
        int minfitness = fitness1;
        for (int j = 0; j < facilities.size(); j++) {
            if (j == index_f) continue;
            if (demands[i] <= facilities[j].capacity - facility_occpy[j]) {
                size_t newfitness = get_cost(j, i);
                if (facility_occpy[j] == 0) newfitness += facilities[j].cost;

                if (newfitness < minfitness) {
                    minfacility = j;
                    minfitness = newfitness; 
                }
            }
        }

        if (minfacility != index_f) {
            facility_occpy[minfacility] += demands[i];
            facility_occpy[index_f] -= demands[i];
            custumer_assign[i] = minfacility;
            _fitness -= (fitness1 - minfitness);

        }
    }

    // isFeasible();
}

void CFLP::closeFacility()
{
    int index_f = rand() % facilities.size();
    while (facility_occpy[index_f] == 0)
        index_f = (index_f == facilities.size()-1) ? 0 : index_f+1;

    auto facility_t = facility_occpy;
    auto custumer_t = custumer_assign;
    auto fitness_t = _fitness - facilities[index_f].cost;
    facility_t[index_f] = 0;
    size_t i = 0;
    for (; i < demands.size(); i++) {
        if (custumer_t[i] == index_f) {
            int newfacility = index_f + 1;
            for (int j = 0; j < facilities.size()-1; j++) {
                if (newfacility == facilities.size()) newfacility = 0;
                if (facility_t[newfacility] != 0 && demands[i] <= facilities[newfacility].capacity - facility_t[newfacility]) break;
                newfacility++;
            }
            if (newfacility == facilities.size()) newfacility = 0;
            if (newfacility != index_f) {
                facility_t[newfacility] += demands[i];
                custumer_t[i] = newfacility;
                fitness_t += get_cost(newfacility, i) - get_cost(index_f, i);
            }
            else break;
        }
    }

    if (i == demands.size()) {
        facility_occpy = move(facility_t);
        custumer_assign = move(custumer_t);
        _fitness = move(fitness_t);
    }
    // isFeasible();
}

void CFLP::openFacility()
{
    int index_f = rand() % facilities.size();
    int index = 0;
    for (; index < facilities.size(); index++) {
        if (facility_occpy[index_f] == 0) break;
        index_f = (index_f == facilities.size() - 1) ? 0 : index_f+1;
    }

    if (index != facilities.size()) {
        if (facility_occpy[index_f] != 0) throw 1;
        auto facility_t = facility_occpy;
        auto custumer_t = custumer_assign;
        auto fitness_t = _fitness + facilities[index_f].cost;

        bool hasmove = false;
        for (int i = 0; i < demands.size(); i++) {
            if (demands[i] <= facilities[index_f].capacity - facility_t[index_f]) {
                size_t fitness = get_cost(custumer_assign[i], i);
                if (demands[i] == facility_t[custumer_assign[i]]) fitness += facilities[custumer_assign[i]].cost;

                if (fitness > get_cost(index_f, i)) {
                    fitness_t -= fitness - get_cost(index_f, i);
                    facility_t[custumer_assign[i]] -= demands[i];
                    facility_t[index_f] += demands[i];
                    custumer_t[i] = index_f;
                    hasmove = true;
                }
            }
        }

        if (hasmove) {
            facility_occpy = move(facility_t);
            custumer_assign = move(custumer_t);
            _fitness = fitness_t;
        }

        // isFeasible();
    }
}

void CFLP::swapFacility()
{
    int fir = rand() % facilities.size();
    int sec = rand() % facilities.size();
    while (sec == fir) sec = rand() % facilities.size();

    vector<size_t> assign1, assign2;
    size_t demand1 = 0, demand2 = 0;
    for (int i = 0; i < demands.size(); i++) {
        if (custumer_assign[i] == fir) {
            assign1.emplace_back(i);
            demand1 +=  demands[i];
        }
        else if (custumer_assign[i] == sec) {
            assign2.emplace_back(i);
            demand2 +=  demands[i];
        }
    }

    int diff = demand1 - demand2;

    if ((diff >= 0 && diff <= facilities[sec].capacity - facility_occpy[sec]) ||
        (diff < 0 && -diff <= facilities[fir].capacity - facility_occpy[fir])) {

        facility_occpy[fir] -= diff;
        facility_occpy[sec] += diff;

        if (facility_occpy[fir] == 0 && facility_occpy[sec] != 0)
            _fitness += (facilities[sec].cost - facilities[fir].cost);
        else if (facility_occpy[fir] != 0 && facility_occpy[sec] == 0)
            _fitness += (facilities[fir].cost - facilities[sec].cost);

        for (auto i : assign1) {
            _fitness += (get_cost(sec, i) - get_cost(fir, i));
            custumer_assign[i] = sec;
        }
        for (auto i : assign2) {
            _fitness += (get_cost(fir, i) - get_cost(sec, i));
            custumer_assign[i] = fir;
        }

    }
    // isFeasible();
}

void CFLP::isFeasible()
{
    auto fitness = _fitness;
    auto facility_t = move(facility_occpy);

    facility_occpy = vector<size_t> (facilities.size(), 0);
    
    for (int i = 0; i < custumer_assign.size(); i++) {
        facility_occpy[custumer_assign[i]] += demands[i];
    }

    for (int i = 0; i < facilities.size(); i++) {
        if (facility_t[i] != facility_occpy[i] || facility_occpy[i] > facilities[i].capacity)
            throw 1;
    }

    calFitness();
    if (fitness != _fitness) throw "sdf";
}
