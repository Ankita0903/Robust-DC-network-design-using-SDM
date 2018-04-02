//include files
#include <ilcplex/cplex.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


//function prototype
int refreshCPLEXEnvironment();

// global variables

double *value;
int status;
CPXENVptr env;
CPXLPptr lp;

// start to define your variables and function prototypes here

#define N 12 //no of nodes including 0 as virtual
#define M 9999 //99 // a large number
#define R 30 // no of request
#define B 48 //bandwidth requested (should be read from input file)
#define source 0 //0= virtual node
#define C 4 //set of core per fiber
#define MF 3 //no of modulation formats
#define D 1 // disaster