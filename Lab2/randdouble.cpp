//
// Created by hrkec on 10/12/2019.
//

#include "randdouble.h"
#include <stdlib.h>
#include <time.h>
double rand_double (){
    double value = rand() / double(RAND_MAX);
    return value;
};
