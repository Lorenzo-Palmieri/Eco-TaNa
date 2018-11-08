#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<math.h>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<array>
#include "functions.h"


#ifndef __Class
#define __Class

using namespace std;

class Species_Class{
public:
    double pt ; // price time t
    double Ht ; // fitness time t
    double Pgrow ; // Probability of growth.
    int strategy ; // strategy adopted by the species
    int loosing_steps ; // number of "loosing" steps
    array <int,L> binary_code; // binary code
    int label;
    // 3 terms of the Hamiltonian
    double T1,T2,T3;
    int dm; //distance from the market.
    
    void calculate_fitness(int, int, array<int,L> &, vector <Species_Class> &, array < vector < double>, genomes > &);
    void mutations();
};
#endif /* defined(__Class) */
