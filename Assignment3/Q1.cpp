#include <omp.h>
void main ()
{     
    int i;       
    double ZZ, func(), res=0.0;

    #pragma omp parallel for reduction(+:res) private(ZZ) 
    for (i=0; i< 1000; i++){
        ZZ = func(I);
        res = res + ZZ; 
    }
}