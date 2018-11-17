#include "states.h"

void printStates(int ** Mx,int ngenes){
  int i,j;
  for(i=0; i < ngenes; i++){
    for(j=0; j < ngenes; j++){
     printf("%2d ", Mx[i][j]);}
   printf("\n");}
  printf("\n");
}

char * bin2str (char * str, int v[], int ngenes) {
  int i;
  char * ch = str;
  for (i=0; i<ngenes; i++) {
    *str++ = '0' + v[i];
  }
  *str = 0;
  return ch;
}

int dec2bin (int v[], int ngenes, unsigned long state) {
  int i;
  for (i=0; i<ngenes; i++) v[i] = 0;
  i = ngenes;
  while (state) {
    v[--i] = (state & 1);
    state = state >> 1;
  }
  return 0;
}

char * dec2str (char *str, int ngenes, unsigned long state) {
  int * v = (int *) malloc (sizeof (int) * ngenes);
  char * ch = str;

  dec2bin (v, ngenes, state);
  str = bin2str (str, v, ngenes);

  free (v);
  return ch;
}

int nextState (int u[], int ** M, int v[], int ngenes) {
  int i, j, soma;
  for (i=0; i<ngenes; i++) {
    soma = 0;
    for (j=0; j<ngenes; j++) {
      soma += M[i][j] * v[j];
    }
    if (soma > 0) u[i] = 1;
    if (soma == 0) u[i] = v[i];
    if (soma < 0) u[i] = 0;
  }
  return 0;
}

unsigned long bin2dec (int v[], int ngenes) {
  int i; 
  unsigned long state = 0;
  for (i=0; i<ngenes; i++) {
    state += v[i] << (ngenes - i - 1);
  }
  return state;
}


int generatedDiagram (int ** M, int ngenes, unsigned long graph []) {
  unsigned long start, end;
  int * v = (int *) malloc (sizeof(int) * ngenes);
  int * u = (int *) malloc (sizeof(int) * ngenes);
  unsigned long number_of_states = 1 << ngenes;

  for (start = 0; start < number_of_states; start++) {
    dec2bin (v, ngenes, start);
    nextState (u, M, v, ngenes);
    end = bin2dec (u, ngenes);
    graph[start] = end;
  }

  free (v);
  free (u);

  return 0;
}

int find_basins (unsigned long node, unsigned long graph [], int visited []) {

  if (visited[node] != -1) return visited[node];

  number_visited++;

  top = 0;
  stack[top++] = node; visited[node] = basin_number;
  tam_basin[basin_number]++;
  do {
    node = graph[node];
    if (visited[node] == -1) {
      /* node is not visited */
      stack[top++] = node;
      visited[node] = basin_number;
      tam_basin[basin_number]++;
      number_visited++;
    }
    else {
      if (visited[node] != basin_number) {
        /* node is not in the stack and already visited at other time */
        /* belongs to an existing attractor */
        while (top > 0) { 
          visited[stack[--top]] = visited[node];
          tam_basin[basin_number]--;
          tam_basin[visited[node]]++;
        }
        return visited[node];
      }
      else {
        /* in this case, visited[node] == basin_number */
        /* node is in the stack */
        /* belongs to a new attractor */
        /* printf ("Attractor %d:\n", basin_number); */
        attractor[basin_number] = node; tam_attractor[basin_number] = 1; attractor_node[node] = 1;
        while (top > 0 && stack[top-1] != node) {
          tam_attractor[basin_number]++; 
          top--; attractor_node[stack[top]] = 1;
        }
        top = 0;
        basin_number++;
        return basin_number-1;
      }
    }
  }  while (1);
}

void generateBasin (int ** M,int ngenes, struct states * aux){
  
  double * frecuency_tam_basin;
  unsigned long *graph, number_of_states, node, *ap1;
  int i,j,*ap2;
  char * ap3;
  
  number_of_states = 1 << ngenes;

  graph = (unsigned long *) malloc (sizeof (unsigned long) * number_of_states);
  if (!graph) {printf ("It was not possible to allocate memory\n");}

  ap1 = tam_basin; ap2 = visited; ap3 = attractor_node;
  for (i=0; i<number_of_states; i++) {
    *ap1++ = 0;*ap2++ = -1; *ap3++ = 0; 
    } 

  generatedDiagram (M, ngenes, graph); 
 
  number_visited = basin_number = 0;
  for (node = 0; number_visited < number_of_states && node < number_of_states; node ++) {
    if (visited[node] == -1) find_basins (node, graph, visited); 
  }

  aux->M = (int **) malloc (ngenes * sizeof (int *));
  for (i=0; i < ngenes; i++)
    aux->M[i] = (int *) malloc (ngenes * sizeof (int));
  aux->atractors = (unsigned long *) malloc (sizeof (unsigned long) * basin_number);
  aux->size_atractors = (unsigned long *) malloc (sizeof (unsigned long) * basin_number);
  aux->frecuency = (double *) malloc (sizeof (double) * basin_number);  
  
  aux->nro_atractors = basin_number;

  for(i=0; i<basin_number; i++){
    aux->atractors[i]=attractor[i];
    aux->size_atractors[i]=tam_basin[i];
    aux->frecuency[i]=(double)tam_basin[i]/number_of_states;    
  }

  for (i=0; i < ngenes; i++)
    for (j=0; j < ngenes; j++)
      aux -> M[i][j] = M[i][j];
  
  free(graph);
  clear(stack,MAX1);
  clear(attractor,MAX1);
  clear(tam_attractor,MAX1);
  clear(tam_basin,MAX1);
}

void destroySTATES(struct states * st,int n){
  int i;
  free(st -> atractors);
  free(st -> size_atractors);
  free(st -> frecuency);
  for(i = 0; i < n; i++) free(st -> M[i]);free(st -> M);
  free(st);
  st = NULL;
}

void destroyLIST_STATES(struct list_states * list_state,int n){
  int i,j;
  
  for(i = 1; i < list_state -> popSize; i++){
    free(list_state -> list[i].size_atractors);
    free(list_state -> list[i].frecuency);
    free(list_state -> list[i].atractors);
    for(j = 0; j < n; j++)
      free(list_state -> list[i].M[j]);
    free(list_state -> list[i].M);
  }
  free(list_state -> list);
  free(list_state);
  list_state = NULL;
}

void clear(unsigned long l [],int t)
{
  int i;
  for(i=0; i<t; i++)
    l[i] = 0;
}

void printLIST_STATES(struct list_states * list_state,int n){
  int i;
  for(i = 0; i < list_state -> popSize; i++){
    printSTATES(&list_state -> list[i],n);
  }
}

void printLISTNET(struct listNETWORK * Network){
  int i,j,k;
  for(i = 0; i < Network->nlist; i++){
    printINPUT(Network -> listNet[i].subAtractors);
    for(j = 0; j < Network->listNet[i].nNetwork; j++){  
      printf("Deviation: %f\n",Network->listNet[i].list[j].deviation);
      printf("Number Atractors: %lu\n\n",Network->listNet[i].list[j].nro_atractors);
      for(k=0; k < Network->listNet[i].list[j].nro_atractors; k++){  
	printf("Attractor: %04lu Size basin: %04lu Frecuency: %4f \n",
	       Network->listNet[i].list[j].atractors[k],
	       Network->listNet[i].list[j].size_atractors[k],
	       Network->listNet[i].list[j].frecuency[k]);
      }
      
    }
  }
}

void printSTATES(struct states * state,int ngenes){
  int i,j;
  printf("  GRN generated \n");
  for(i = 0; i < ngenes; i++){
    for (j = 0; j < ngenes; j++)
      printf("%3d",state -> M[i][j]);
    printf("\n");
  }
  printf("\n  Atractors:%lu Deviation: %4f\n",state -> nro_atractors,state -> deviation);
  printf("\n");
  for(i=0; i < state -> nro_atractors; i++){  
    printf("  Attractor: %04lu Size basin: %04lu Frecuency: %4f \n",
	   state -> atractors[i],
	   state -> size_atractors[i],
	   state -> frecuency[i]);
  }
  printf("\n");
}
