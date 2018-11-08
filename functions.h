#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<math.h>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include <array>
#include <random>

#define No 100
#define L 12 // we could simulate up to L=18, but the initialization is quite slow.
#define K 3
#define genomes 4096
#define theta 0.5
#define pmut 0.1
#define pmark 0.7
#define critical_step 100 // va visto come un parametro (nel caso RW prob di andare giù di fila è 0.5^100)
#define born_step 1000 // ogni mese circa può nascere una compagnia
#define market_step 400000 //possiamo fare che 10^4 sono 1 anno.
//#define market_step 1500000 // -->set to infinity
#define pi 3.14159265358979323846
#define simulation_length 1500000


using namespace std;

bool fileExists(const string& filename);

double powerlaw(int, int, int);

int Hamming(array<int,L> &, array<int,L> &);

void Bin_recursive(int , int , array<int, L> &);

void ConvertToBinary(int , int , array<int, L> &);

int ConvertToDec(array<int, L> &);

int choose_in_range(int , int );

double compute_interactions(int , int , array<double,genomes> &, array <int, genomes> &, array< array<int,L> ,genomes> &);

void get_row(int (&m)[K][L], int row, array<int, L> &r);

double gaussian(void);

double uniform(void);

int change_market(array<int, L> &);

