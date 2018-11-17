 
# include "input.h"

int randomIntervalo(int a,int b)
{
  return a + rand() % (b + 1 - a);
}

// Utility function to find ceiling of r in arr[l..h]
int findCeil(int arr[], int r, int l, int h)
{
    int mid;
    while (l < h)  {
      mid = l + ((h - l) >> 1);  // Same as mid = (l+h)/2
      (r > arr[mid]) ? (l = mid + 1) : (h = mid);
    }
    return (arr[l] >= r) ? l : -1;
}

// The main function that returns a random number from arr[] according to
// distribution array defined by freq[]. n is size of arrays.
int myRand( int freq[], int n)
{
    // Create and fill prefix array
  int prefix[n],i,r;
    prefix[0] = freq[0];
    for (i = 1; i < n; ++i)
        prefix[i] = prefix[i - 1] + freq[i];

    // prefix[n-1] is sum of all frequencies. Generate a random number
    // with value from 1 to this sum
    r = (rand() % prefix[n - 1]) + 1;

    // Find index of ceiling of r in prefix arrat
    return findCeil(prefix, r, 0, n - 1);
}


int VerifyINPUT(int* row, int i, INPUT * dados,int ngenes)
{
  int s, j, sum, x, nulo, N, A;
  N = ngenes;
  A = dados->atractores;
   // Do not consider rows which auto-activation
  if (row[i] == 1) return 0;

  // Do not consider row with all NULL entries and just 1 non-NULL entry and all n
  for (s=sum=0,nulo=1; s<N; s++) {
    if (row[s] != 0) nulo=0;
    if (row[s] != 0) sum++;
  }
  if (nulo) return 0;
  if (sum == N || sum == 1 ) return 0;
  
  for (s = 0; s < A; s++) {
    sum = 0;
    for (j = 0; j < N; j++) {
      sum += row[j] * dados -> Matriz[s][j];
    }
    if (sum > 0) {
      x = 1;
    } else if (sum < 0) {
      x = 0;
    } else {
      x = dados->Matriz[s][i];
    }
    // Compare with the same state
    if (x != dados->Matriz[s][i]) {
      return 0;
    }
  }
  return 1;
}

int Next(int *mask, int n)
{
  int i;
  for (i = n-1; i >= 0; i--) {
    if (mask[i] == -1) {
      mask[i] = 0;
      break;
    } else if (mask[i] == 0) {
      mask[i] = 1;
      break;
    } else if (mask[i] == 1) {
      mask[i] = -1;
    }
  }
  return (i >= 0)? 1 : 0;
}
 
void destroyINFERENCE(struct inference * in,int N){
  int i,j;
  
  for(i = 0; i < N; i++){
    for(j = 0; j < in -> size[i]; j++)
      free(in -> rows[i][j]);
    free(in -> rows[i]);
  }
  free(in -> rows);
  free(in -> size);
  free(in);
  in=NULL;
}

void printINFERENCE(INPUT * data,struct inference * in){
  int i,j,k,l,N;
  N = data -> ngenes;
  printINPUT(data);
  /* for (l = 0; l < N; l++)  */
    /* printf("[%d]-",in -> size[l]); */
  /* printf("\n"); */
  for(i = 0; i < N; i++){
    printf ("Gene (Row): %2d [%d Solutions]\n", i, in->size[i]);
    for(j = 0; j < in -> size[i]; j++){
      for(k = 0; k < N; k++)
	printf("%3d",in -> rows[i][j][k]);
      printf("\n");
    }
    printf("\n");
  }
}

void inference(INPUT * in, struct inference * out){
  struct inference aux;
  int * row,i,j,k,N,A,count;
    
  N = in->ngenes;
  aux.size = (int *) malloc (N * sizeof (int));
  aux.rows = (int ***) malloc (N * sizeof (int **));  
  
  for (i = 0; i < N; i++) {
    row = (int *) malloc (N * sizeof (int));
    for (j = 0; j < N; j++) row[j] = -1;
        
    count = 0;
    do {
      if (VerifyINPUT(row,i,in,N) == 1) count++;   
    } while(Next(row, N) == 1);
    aux.size[i] = count;

    free(row);
    row = (int *) malloc (N * sizeof (int));
    for (j = 0; j < N; j++) row[j] = -1;
    
    aux.rows[i] = (int **) malloc (count * sizeof (int *));
    for (k = 0; k < count; k++)
      aux.rows[i][k] = (int *) malloc (N * sizeof (int));

    k=0;
    do {
      if (VerifyINPUT(row,i,in,N) == 1){
	for (j = 0; j < N; j++)
	  aux.rows[i][k][j] = row[j];
	k++;
      }  
    } while(Next(row, N) == 1);
    
    free(row);
  }
  out -> rows = aux.rows;
  out -> size = aux.size; 
}  

INPUT * ReadInput(const char *samplefile) {

  int i,j,co,input_number_attractors,input_number_genes,pot2;
  double t;
  FILE *file;
  
  INPUT * data = malloc(sizeof(struct input));
  
  file = fopen (samplefile, "r");
  if (file == NULL) return 0;

  fscanf (file, "%d", &input_number_attractors);
  fscanf (file, "%d", &input_number_genes);

  data->atractores = input_number_attractors;
  data->ngenes = input_number_genes;

    
  data->fr = (double *)malloc(input_number_attractors * sizeof (double));
  data->Matriz = (int **) malloc ( input_number_attractors * sizeof (int *));
  for(i = 0; i < input_number_attractors; i++) {
    data->Matriz[i] = (int *) malloc (input_number_genes * sizeof(int));
  }

  data->state = (int *) malloc (input_number_genes * sizeof(int));
  
  for(i = 0; i < input_number_attractors; i++){
    for(j = 0; j < input_number_genes; j++){
      fscanf(file,"%d",&co);
      data -> Matriz[i][j] = co;
    }
    
    fscanf(file,"%lf",&t);
    data->fr[i]=t;
  }
  
  
  for(i = 0; i < input_number_attractors; i++){
    data->state[i] = 0; pot2 = 1;
    for(j = input_number_genes - 1; j >= 0; j--) {
      data -> state[i] += data -> Matriz[i][j] * pot2;
      pot2 *= 2;
    }
  }
  
  fclose(file);
  return data;
}

void generateINPUT(INPUT * inputData,INPUT * ptr) {

  int i,j,ram,r,pot2;
  double t,sum=0.0;
  int * visited;
  INPUT aux;
  
  visited = (int *) malloc (inputData->atractores * sizeof (int));
  int index[inputData -> atractores];
  for(i=0;i<inputData -> atractores;i++){
    visited[i]=0;
    index[i]=(int) (100.0 * inputData -> fr[i]);
  }
  
  ram = randomIntervalo(6,9);
  ram++; /* devido ao atrator 0 */
  
  aux.ngenes = inputData -> ngenes;
  aux.atractores = ram; 
  
  aux.state = (int *)malloc(ram * sizeof (int));
  aux.fr = (double *)malloc(ram * sizeof (double));
  aux.Matriz = (int **) malloc (ram * sizeof (int *));
  
  for(i = 0; i < ram; i++)
    aux.Matriz[i] = (int *) malloc (inputData -> ngenes * sizeof(int));
  
  i = 0;
  while( i < ram-1) {
    r = myRand(index,inputData -> atractores - 1);
        
    if(visited[r] != 1){
      visited[r] = 1;
      aux.fr[i] = inputData -> fr[r];
      for(j = 0; j < inputData -> ngenes ; j++) {
	aux.Matriz[i][j] = inputData -> Matriz[r][j];
      }
           
      aux.state[i] = 0; pot2 = 1;
      for(j = inputData -> ngenes - 1; j >= 0; j--) {
	aux.state[i] += inputData -> Matriz[r][j] * pot2;
	pot2 *= 2;
      } 
     
      i++;
    }
  }
  /* atractor 0 */
  aux.state[ram-1] = 0;
  aux.fr[ram-1] = 0.0;
  for (j=0; j<inputData -> ngenes; j++)
    aux.Matriz[ram-1][j] = 0;

  for(i = 0; i < aux.atractores; i++) sum += aux.fr[i];
  for(i = 0; i < aux.atractores; i++) aux.fr[i] = aux.fr[i]/sum;
  
  ptr -> state = aux.state;
  ptr -> atractores = aux.atractores;
  ptr -> ngenes = aux.ngenes;
  ptr -> Matriz = aux.Matriz;
  ptr -> fr = aux.fr;
 
  free(visited);
}
 
int ** generatedM(INPUT * ptr,struct inference * inf)
{
  int ** aux,i,j,r;
  aux = (int **) malloc (ptr -> ngenes * sizeof (int*));
  for(j=0;j< ptr->ngenes ;j++)
    aux[j] = (int*) malloc (ptr -> ngenes * sizeof (int));
  for(i = 0; i < ptr->ngenes; i++){
    r = randomIntervalo(0,inf -> size[i] - 1);
    for (j = 0; j < ptr->ngenes; j++)
      aux[i][j] = inf -> rows[i][r][j];
  }  
  return aux;
}

void destroyM(int ** M,int n){
  int i;
  for(i = 0; i < n; i++)
    free(M[i]);
  free(M);
}

void destroyINPUT(INPUT * input){
  int i,j;
  for(i = 0; i < input -> atractores; i++)
    free(input -> Matriz[i]);
  free(input -> Matriz);
  free(input -> fr);
  free(input -> state );
  free(input);
  input=NULL;
}

void printINPUT(INPUT * input){
  int i,j;
  
  printf(" Attractor Set with %d attractors:\n",input->atractores);
  for(i=0;i<input->atractores;i++) {
    for(j=0;j<input->ngenes;j++)
      printf("%2d",input->Matriz[i][j]);
    printf(" - %03d - ",input -> state[i]);
    printf("%2lf \n",input->fr[i]);
  
  }
  printf("\n");
}
