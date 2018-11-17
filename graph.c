#include <stdlib.h>
#include <stdio.h>

#include "graph.h"


NODE * createNode(int v)
{
  NODE * newNode = malloc(sizeof(NODE));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

GRAPH * createGraph(int vertices)
{
  int i;
  GRAPH * Graph = malloc(sizeof(GRAPH));
  Graph->numVertices = vertices;
  
  Graph->adjLists = malloc(vertices * sizeof(NODE *));  
  Graph->visited = malloc(vertices * sizeof(int));
  
  for (i = 0; i < vertices; i++) {
    Graph->adjLists[i] = NULL;
    Graph->visited[i] = 0;
  }
  return Graph;
}

void destroy_graph(GRAPH * graph){
  int v;
  if(graph){
    if(graph->adjLists){
      for (v = 0; v < graph->numVertices; v++)	{
	NODE * temp = graph->adjLists[v];
	while (temp) {
	  NODE * tmp = temp;
	  temp = temp->next;
	  free(tmp);
	}
	free(temp);
      }
      free(graph->adjLists);
    }
    free(graph->visited);
    free(graph);
    graph=NULL;
  }
}

void DFS(GRAPH * Graph, int vertex) {
  NODE * temp = Graph->adjLists[vertex];
  Graph->visited[vertex] = 1;
  while(temp) {
    if(Graph->visited[temp->vertex] == 0) {
      DFS(Graph, temp->vertex);
    }
    temp = temp->next;
  }
  free(temp);
}

GRAPH * reverse(GRAPH * Graph){
  GRAPH * g = createGraph(Graph->numVertices);
  int v;
  for (v = 0; v < Graph->numVertices; v++){
    NODE * temp = Graph->adjLists[v];
    while (temp){
      addEdge(g,temp->vertex,v);
      temp = temp->next;
    }
    free(temp);
  }
  return g;
}

void addEdge(GRAPH * Graph, int src, int dest)
{
  NODE * newNode = createNode(dest);
  newNode->next = Graph->adjLists[src];
  Graph->adjLists[src] = newNode;
 
}

int number(GRAPH * Graph){
  int v,j=0;
  for (v = 0; v < Graph->numVertices; v++) {
    NODE * temp = Graph->adjLists[v];
    while (temp){
      j++;
      temp = temp->next;
    }
    free(temp);
 }
 return j;
}

void printGraph(GRAPH * Graph)
{
  int v;
  for (v = 0; v < Graph->numVertices; v++)
    {
      NODE * temp = Graph->adjLists[v];
      printf("vertex [%d] ", v);
      while (temp){
	printf(" -> %d", temp->vertex);
	temp = temp->next;
      }
      free(temp);
      printf("\n");
    }
}

int isConnected(GRAPH * Graph){
  int j;
  GRAPH * graphr;
  graphr=reverse(Graph);
  DFS(Graph, 0);
  for(j=0;j<Graph->numVertices;j++)
    if(Graph->visited[j]==0)return 0;
  
  DFS(graphr, 0);
  for(j=0;j<graphr->numVertices;j++)
    if(graphr->visited[j]==0)return 0;
  
  if(graphr)destroy_graph(graphr);
  return 1;
}

