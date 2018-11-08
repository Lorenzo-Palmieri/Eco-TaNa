#include "functions.h"
#include "species_class.h"

void Species_Class :: calculate_fitness(int Ptot , int index , array<int,L> &market_strategy, vector <Species_Class>& species, array < vector < double>, genomes > &J){
    double a1 = 2.0, a2 = 2.0, a3=0.000001, a4=0.2;
    double Jij;
    int d;
    
    Ht = 0.;
    T1=0;
    T2=0;
    T3=0;

    int i  = species[index].strategy;
    for(int k = 0 ; k < species.size() ; k++ ) {
        int j  = species[k].strategy;
        d = Hamming(species[index].binary_code, species[k].binary_code);
            T1 += species[k].pt * a1 * J[i][j]/ Ptot;
            T2 += a3 * species[k].pt * (1 + a4*cos(pi*double(d)/L)) ;
    }
    
    dm = Hamming(market_strategy,species[index].binary_code);
    T3 = a2 * cos(pi*double(dm)/L);
    Ht += T1 - T2 + T3;
    Pgrow = exp(Ht) / (1 + exp(Ht));
    return;
}


void Species_Class :: mutations(){
    for (int i = 0 ;  i < L ; i++){
        if ( uniform() <= pmut){
            if (binary_code[i] == 1)
                binary_code[i] = 0;
            else
                binary_code[i]=1;
        }
    }
    loosing_steps = 0 ;
    strategy = ConvertToDec(binary_code);
    return ;
}
