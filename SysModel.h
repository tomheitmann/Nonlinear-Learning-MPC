#include "Track.h"

#ifndef SYS_MODEL
#define SYS_MODEL

void DynModel(double* x_global_next, double* x_curv_next, double x[6], double x_glob[6], double u[2], double dt, Map * map);


#endif 
