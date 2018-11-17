
typedef struct node NODE;
typedef struct graph GRAPH;
 
struct node
{
  int vertex;
  NODE * next;
};

struct graph
{
  int numVertices;
  int * visited;
  NODE ** adjLists;
};

NODE * createNode(int );
void destroy_node(NODE *);
GRAPH * createGraph(int);
GRAPH * reverse(GRAPH *);
void destroy_graph(GRAPH *);
void addEdge(GRAPH *,  int, int);
void printGraph(GRAPH *);
void DFS(GRAPH *, int);
int number(GRAPH *);
int isConnected(GRAPH *);
