#include <fstream>
#include <iostream>
#include <ctime>

#include "CFLP.h"
#include "Greedy_algorithm.h"
#include "SA_algorithm.h"
using namespace std;

int main(int argc, char const *argv[])
{
    
    size_t total = 0;
    fstream outfile;
    int input;

    cout << "input 0 for Greedy algorithm, 1 for SA algorithm" << endl;
    cin >> input;

    if (input == 0) {
        auto totalstart = clock();
        outfile.open("../result/Greedy_result", ios::out);
        Greedy_algorithm greedy;
        for (int i = 1; i <= 71; i++) {
            auto start = clock();
            greedy.load_file("../Instances/p" + to_string(i));
            greedy.getSolution();
            outfile << "p" + to_string(i) + " fitness: " << greedy.getFitness()
                << " time: " << double(clock() - start) / 1000 << "s" << endl;
            outfile << "solution: " << greedy.to_string() << endl << endl; 
            total += greedy.getFitness();

            cout << "p" << to_string(i) << "solve" << endl;
        }

        outfile << "total fitness: " << total << endl;
        outfile << "total time: " << double(clock() - totalstart) / 1000 << "s" << endl;;
    }

    else if (input == 1) {
        auto totalstart = clock();
        outfile.open("../result/SA_result", ios::out);
        SA_algorithm sa;
        for (int i = 1; i <= 71; i++) {
            auto start = clock();
            sa.load_file("../Instances/p" + to_string(i));
            sa.getSolution();
            outfile << "p" + to_string(i) + " fitness: " << sa.getFitness()
                << " time: " << double(clock() - start) / 1000 << "s" << endl;
            outfile << "solution: " << sa.to_string() << endl << endl; 
            total += sa.getFitness();

            cout << "p" << to_string(i) << "solve" << endl;
        }

        outfile << "total fitness: " << total << endl;
        outfile << "total time: " << double(clock() - totalstart) / 1000 << "s" << endl;;
    } else exit(0);

    outfile.close();
}
