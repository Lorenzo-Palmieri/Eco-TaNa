#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<math.h>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include <algorithm>


#include "functions.h"
#include "species_class.h"

int main(){
    
    int N = No,cs, lab;
    int dim = pow(2,L);
    vector <Species_Class> species(N) ;
    vector<int> strategies;
    int Ptot=0, Ptot_old=0;
    double Dtot=0;
    double similarity;
    array<int,L> market_strategy={} ;
    array< array<int,L> ,genomes> table;
    double dm, w;
    int i,j,t,Ndead,l, rand, Market, k;
    double Z;
    string filename;
    stringstream convert;
    vector<ofstream> streams;
    array <int, genomes> B;
    array <double, genomes> A;
    array < vector < double>, genomes > J;// In this way there is no problem with the initial stack, because it just stores an array of size genomes, that will be filled with pushback in a second moment.

    
    // Create table
    for (i=0; i<genomes; i++) {
        array<int,L> vec={} ;
        ConvertToBinary(i, L, vec);
        table[i]=vec;
    }
    
    
    // Initialize vectors A and B for correlations.
    for (i=0; i < genomes; i++) {
        A[i] = gaussian();
        if(uniform() <= theta){
            B[i] = 1;
        }else{
            B[i] = 0;
        }
    }
    
    for(i = 0 ; i < genomes; i++ ) {
        for (j=0; j<genomes; j++) {
            J[i].push_back(compute_interactions(i, j, A, B, table));
        }
    }

    


    for(i = 0 ; i < N ; i++ ){
        rand = choose_in_range(0,pow(2,L-1));
        species[i].strategy = rand;
        species[i].loosing_steps = 0 ;
        species[i].binary_code = table[rand];
        species[i].pt = 100 ; // I initialise all the prices
        species[i].label=i; // assign label to species
        lab=i;
    }
    
    Market = choose_in_range(0,pow(2,L-1));
    market_strategy = table[Market];
    
 
    ofstream fPtot("Ptot.txt");
    ofstream fvar("variation.txt");
    ofstream fm("market.txt");
    ofstream fsim("similarity.txt");
    ofstream fsum("summary.txt");
    ofstream fn("N.txt");
    
    
    // Dynamics //
    for(t = 0; t < simulation_length; t++){
        // Total amount of Money in the system //
        Ptot=0 ;
        for(j = 0 ; j < N ; j ++) {
            Ptot += species[j].pt ;
        }
        
       // Ndead=0;
        for(i = 0 ; i < N ; i ++){ // cambiare prezzo di un double.
            species[i].calculate_fitness(Ptot, i, market_strategy, species, J);
            if(uniform() < species[i].Pgrow){
                species[i].pt += 1 ;
                //species[i].pt += species[i].pt/10000;
                //species[i].pt += 1 + species[i].Pgrow*gaussian(); // random variable with sigma=Pgrow.
                if(species[i].loosing_steps>0){species[i].loosing_steps -= 1;}
            }else{
                species[i].pt -= 1 ;
                //species[i].pt -= species[i].pt/10000;
                //species[i].pt -= 1 + species[i].Pgrow*gaussian(); // random variable with sigma=Pgrow.
                species[i].loosing_steps += 1;
                if (species[i].pt<=0) {
                    //close the file and remove the company.
                    convert.str("");
                    filename.clear();
                    l = species[i].label;
                    convert << l;
                    filename = convert.str();
                    filename+=".txt";
                    streams[l].close();
                    // remove the company from species.
                    species.erase(species.begin()+i);
                    i--;
                    N--;
                    //Ndead++;
                }
            }
        }
        
        //N -= Ndead;

        // Mutations
        for(j = 0 ; j < N ; j ++){
            if(species[j].loosing_steps >  critical_step){species[j].mutations();}
        }
        // Market changes.
        if(t % market_step == 0 ) {
            Market = change_market(market_strategy);
            Market = ConvertToDec(market_strategy);
        }
        
        // Create new companies.
        if(t % born_step == 0 ) {
            //if(uniform() <= similarity){
                N++;
                Species_Class s;
                s.strategy = choose_in_range(0,pow(2,L-1));
                s.binary_code = table[s.strategy];
                s.loosing_steps = 0;
                s.pt = Ptot/1000;
                lab++; //assign a label to the new species;
                s.label=lab;
                species.push_back(s);
                // next line is just for printing (Ptot has changed)
                species[i].calculate_fitness(Ptot + s.pt, i, market_strategy, species, J);
           // }
        }
        
        // PRINT COMMANDS.
        if (t>=0) {
            //if(t == simulation_length-1){
            //fsum.open("summary.txt");
            //fsum << (lab+1) << "\t" << t << endl;
            //fsum.close();
            //}
            fsum << lab << "\t" << t << endl;

                strategies.clear();
                for(j = 0 ; j < N ; j ++) {
                    convert.str("");
                    filename.clear();
                    l = species[j].label;
                    convert << l;
                    filename = convert.str();
                    filename+=".txt";
                    
                    // if the file doesn't exist, it is created.
                    if(fileExists(filename)==0){
                        streams.emplace_back(ofstream(filename));
                        //streams[l].open(filename); NO. Si apre automaticamente.
                    }

                    streams[l] << t << "\t" << species[j].pt << "\t" << species[j].strategy << "\t"<< species[j].dm << "\t"<<species[j].Ht << "\t" << species[j].T1<< "\t" << species[j].T2 << "\t" << species[j].T3 << endl;
                }
            
                // Compute Ptot
                Ptot = 0;
                for(j = 0 ; j < N ; j ++) {Ptot += species[j].pt ;}
                // Compute similarity
                similarity=0;
                for(j = 0 ; j < N ; j ++) {
                        dm = double( Hamming(market_strategy,species[j].binary_code))/L;
                        w = dm * double(species[j].pt)/Ptot;
                        similarity += w;
                }
                if (Ptot_old!=0) { // avoid first point.
                    Dtot = double((Ptot - Ptot_old))/Ptot_old;
                    fvar << Dtot << endl;
                }
                fPtot << Ptot << endl;
                fm << Market << endl;
                fsim << similarity << endl;
                Ptot_old=Ptot;
                fn << N << endl;
            }// END OF PRINT COMMANDS
        // Print steps.
        if(t % 100000 == 0 ) {
            cout << t << "..."<<endl;
        }// End of print steps.
    }
    fPtot.close();
    fvar.close();
    fm.close();
    fsim.close();
    fsum.close();
    fn.close();
    return 0 ;
}



