
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <math.h>
# include "input.h"  

//# define MAX1 0xFFFFFFFFU
# define MAX1 0xFFFFFFF0
# define MAX_GEN 100

int visited [MAX1];
unsigned long stack[MAX1], top;
unsigned long basin_number;
unsigned long tam_basin[MAX1];
unsigned long number_visited;

unsigned long attractor[MAX1];
unsigned long tam_attractor[MAX1];
char attractor_node[MAX1];
unsigned long *state_data;
int nsamples;

typedef struct states STATE;


struct list_states {
  int popSize;
  struct states{
    unsigned long nro_atractors;
    unsigned long * atractors;
    unsigned long * size_atractors;
    double * frecuency;
    double deviation;
    int ** M;
  }* list;
};

struct listNETWORK{
  int nlist;
  struct NETWORK{
    int nNetwork;
    INPUT * subAtractors;
    struct states * list;
  } * listNet;
};

struct listPBTN{
  int popsize;
  struct PBTN{
    double MSE;
    int * indice;
    double * prob_selection;
  } * list;
};


char * bin2str (char *, int [], int);
int dec2bin (int [], int, unsigned long);
char * dec2str (char *, int, unsigned long);
unsigned long bin2dec(int [], int);
int find_basins(unsigned long, unsigned long [], int[]);

void printStates(int **,int);
int nextState(int[],int**,int[],int);
int generatedDiagram(int **,int,unsigned long[]);
void generateBasin(int **,int,struct states *);
void clear(unsigned long[],int);
void destroySTATES(struct states *,int);
void printSTATES(struct states *,int);
void printLIST_STATES(struct list_states *,int);
void destroyLIST_STATES(struct list_states *,int);
void printLISTNET(struct listNETWORK *);
