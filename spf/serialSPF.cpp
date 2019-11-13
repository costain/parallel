 const int INFINITY=9999;

#include <iostream>
#include <mpi.h>
#include <string.h>
//using namespace std;
#include <stdio.h> 
// let INFINITY be a very big number, bigger than any other

#include <stdlib.h> 
// G[][] is the adjacency matrix
// D[] is the distance array
// path[] is indicating where from we came to that node
const int SIZE = 6; //maximum size of the graph
int G[SIZE][SIZE]; //adjacency matrix
int startNode = 0;
bool visited[SIZE]; //nodes done
int D[SIZE]; //distance
int previous[SIZE]; //we came to this node from

#define buffer_size 2024
const char *filename = "edge.txt";

void Print_paths(int global_pred[], int n) {
    int v, w, count, i;
    int path [SIZE];

    printf("  v     Path %d->v\n",startNode);
    printf("----    ---------\n");
    for (v = startNode; v < n; v++) {
        printf("%3d:    ", v);
        count = 0;
        w = v;
        while (w != 0) {
            path[count] = w;
            count++;
            w = global_pred[w];
        }
        //printf("0 ");
        for (i = count-1; i >= 0; i--)
            printf("%d ", path[i]);
        printf("\n");
    }
//
    //free(paths);
}

void Print_dists(int global_dist[], int n) {
    int v;

    printf("  v    dist %d->v\n",startNode);
    printf("----   ---------\n");

    for (v = 1; v < n; v++) {
        if (global_dist[v] == INFINITY) {
            printf("%3d       %5s\n", v, "inf");
        }
        else
            printf("%3d       %4d\n", v, global_dist[v]);
        }
    printf("\n");
}


int Read_G_Adjacency_Matrix(){
//actual G[][] adjacency matrix read in
 
int u,v = 0;

for ( u = 0; u < SIZE; u++){
        for(int v = 0; v < SIZE; v++){

            if( u==v){
                G[u][v]=0;
            }
            else {
                G[u][v] = INFINITY;
            }
        }

    }

    int nodeU = -1;
    int  nodeV= -1 ;
    int  nodeZ =-1;

    const char *delimiter_characters = " ";
    FILE *input_file = fopen( filename, "r" );
    char buffer[ buffer_size ];
    char *last_token;
    int nodes = SIZE;

    if( input_file == NULL ){

        fprintf( stderr, "Unable to open file %s\n", filename );

    }else {


        while (fgets(buffer, buffer_size, input_file) != NULL) {


            last_token = strtok(buffer, delimiter_characters);

            while (last_token != NULL) {
                nodeU = atoi(last_token);
                nodeV = atoi(strtok(NULL, delimiter_characters));
                nodeZ = atoi(strtok(NULL, delimiter_characters));

                G[nodeU][nodeV] = nodeZ;

                printf("Adding: (%d,%d, %d)\n", nodeU, nodeV,nodeZ);
                last_token = strtok(NULL, delimiter_characters);
            }

        }

        if (ferror(input_file)) {
            perror("The following error occurred");
        }

        fclose(input_file);
   return 0;
    }
}

void dijkstra(int s){
  int i,j;
  int tmp, x;

  //initial values:
  for(i=0;i<SIZE;i++){
    D[i]=G[s][i];
   visited[i]=false;
    previous[i]=s;
  } 
  
  /*Print_dists(D,SIZE);
  printf("\n");  
  Print_paths(path,SIZE);  
*/
  visited[s]=true;
  previous[s]=-1;

  //there are N-1 steps in the greedy algorithm:
  for(j=1;j<SIZE;j++){
   printf("\n");
    //finding maimimum of D[]:
    
     printf("***********We are in round J: %d***********\n",j) ; 
    tmp=INFINITY;
    for(i=0;i<SIZE;i++){
//	printf("OK:%d -- D[i]%d\n",OK[i], D[i]);
	 if(!visited[i] && D[i]<tmp){
        x=i;
        tmp=D[i];
	printf("Tmp: %d\n",tmp);
      } /*else {printf("FAILED\n");}*/
    }
    visited[x]=true;

    //alternating paths:
    for(i=0;i<SIZE;i++){
      if(!visited[i] && D[i]>D[x]+G[x][i]){
   
	D[i]=D[x]+G[x][i];
        printf("D[i]: %d\nD[x]: %d\nG[x][i]: %d\n",D[i],D[x],G[x][i]);    
        previous[i]=x;
	
        printf("X= Previous[%d]: %d\n",i,x) ; 
      } 
    }
   /*	printf("OK:%d **********\n\n\n",OK[0]);
   	printf("OK:%d **********\n\n\n",OK[1]);
   	printf("OK:%d **********\n\n\n",OK[2]);
   	printf("OK:%d **********\n\n\n",OK[3]);
 */ }
}
 

main(int argc, char** argv){

 Read_G_Adjacency_Matrix();
 dijkstra(0);

  Print_dists(D,SIZE);
  printf("\n");  
  Print_paths(previous,SIZE);  

}



