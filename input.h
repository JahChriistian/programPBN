#ifndef _INPUTH_
#define _INPUTH_

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <ctype.h>
# include <math.h>
# include <time.h>
# include <assert.h>


typedef struct inference INFERENCE;
typedef struct input INPUT;

//-- input entrada.txt
struct input {
  int atractores; //-- number attractors started
  int ngenes; //-- number of genes
  double * fr; //-- list frecuency
  int ** Matriz;
  int * state; //-- attractors input
};

struct inference{
  int * size;
  int *** rows;
};
  
INPUT * ReadInput(const char *);
void destroyINPUT(INPUT *);
void printINPUT(INPUT *);
void generateINPUT(INPUT *,INPUT *);
void inference(INPUT *,struct inference *);
void destroyINFERENCE(struct inference *,int);
void printINFERENCE(INPUT *,struct inference *);
int ** generatedM(INPUT *,struct inference *);
void destroyM(int **,int);

int randomIntervalo(int,int);
int findCeil(int [], int, int, int);
int myRand( int [], int );
int Next(int *, int );


#endif
