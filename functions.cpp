#include "functions.h"
//random_device rd; // obtain a random number from hardware.
//mt19937 eng(rd()); // seed the generator.

mt19937 eng(12345); // seed the generator. //Seed usato per tunare algoritmo di ricerca transizioni.



int change_market(array<int, L> &market_strategy){
    for (int i = 0 ;  i < L ; i++){
        if ( uniform() <= pmark){
            if (market_strategy[i] == 1)
                market_strategy[i] = 0;
            else
                market_strategy[i]=1;
        }
    }
    return(ConvertToDec(market_strategy)) ;
}


#include <sys/stat.h>
// Function: fileExists
/* Check if a file exists
 @param[in] filename - the name of the file to check
 @return    true if the file exists, else false
 */
bool fileExists(const string& filename){
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1){
        return true;
    }
    return false;
}


int Hamming(array<int,L> &v1, array<int,L> &v2){ 
    int i, distance=0;
    for (i=0; i<L; i++) {
        if (v1[i] != v2[i]) {
            distance++;
        }
    }
    return distance;
}


void Bin_recursive(int n, int j, array<int,L> &vec){ // It must be called setting j=L.
    j--;
    if (n / 2 != 0) {
        Bin_recursive(n/2, j, vec);
    }
    vec[j] = n % 2;
}

void ConvertToBinary(int n, int j,array<int,L> &vec){
    int i;
    if(n >= pow(2,L)){
        cout << "the number cannot be represented" << endl;
        exit (EXIT_FAILURE);
    }
    for (i=0; i<L; i++) {
        vec[i]=0;
    }
    Bin_recursive(n, j, vec);
}


int ConvertToDec(array<int,L> &arr){
    int i, dec=0;
    for (i=0; i<L;i++) {
        if(arr[L-1-i] == 1){
            dec += pow(2,i);
        }
    }
    return dec;
}


double compute_interactions(int x, int y, array<double,genomes> &A, array <int, genomes> &B,     array< array<int,L> ,genomes> &table){
    double w1=0, w2=0, w3=0;
    int z,i,j;
    int z1[K][L]={},  z2[K][L]= {}, z3[K][L]= {};
    int n1[K], n2[K], n3[K];
    array<int, L> v1,v2,v3,r;
    double interaction;
    
    z=x^y; // XOR
    // convert to binary.
    v1 = table[x];
    v2 = table[y];
    v3 = table[z];
    // split to create correlations.
    for (j=0; j<K; j++) {
        for (i=0; i<L/K; i++) {
            z1[j][i + j*L/K] = v1[i + j*L/K];
            z2[j][i + j*L/K] = v2[i + j*L/K];
            z3[j][i + j*L/K] = v3[i + j*L/K];
        }
        // convert to decimal.
        get_row(z1,j,r);
        n1[j] = ConvertToDec(r);
        get_row(z2,j,r);
        n2[j] = ConvertToDec(r);
        get_row(z3,j,r);
        n3[j] = ConvertToDec(r);
        
    }
    if (B[z]==0 || x==y) {
        interaction = 0;
    } else{
        for (j=0; j<K; j++) {
            w1 +=  A[n1[j]]/sqrt(double(K));
            w2 +=  A[n2[j]]/sqrt(double(K));
            w3 +=  A[n3[j]]/sqrt(double(K));
        }
        interaction = w1 * w3;
    }
    return(interaction);
}


double gaussian(void){
    double u1=0, u2=0;
    double gauss_var;
    // exclude the zero values.
    while (u1 == 0 || u2 == 0) {
        u1 = uniform();
        u2 = uniform();
    }
    gauss_var = sqrt( -2.0*log(u1)) * cos(2.0*pi*u2);
    return(gauss_var);
}

double powerlaw(int x0,int x1, int n){//generates a power law distribution between [x0,x1] with exponent n.
    double y, r;
    y= uniform();
    r=pow((pow(x1,n+1) - pow(x0,n+1))*y + pow(x0,n+1),1./(n+1));
    return(r);
}

double uniform(void){
    uniform_real_distribution<> rand(0, 1); // define the range [a,b], extremes included.
    return rand(eng);
}

int choose_in_range(int a, int b){
    uniform_int_distribution<> choose(a, b); // define the range [a,b], extremes included.
    return choose(eng);
}


void get_row(int (&m)[K][L], int row, array<int, L> &r) {
    int i;
    for (i=0; i<L; i++) {
        r[i] = m[row][i];
    }
}
