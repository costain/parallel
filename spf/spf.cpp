#include <iostream>
#include <mpi.h>
#include <string.h>
//using namespace std;
#include <stdio.h> 
#include <stdlib.h> 
const int SIZE = 70; //maximum size of the graph
int G[SIZE][SIZE]; //adjacency matrix
int startNode = 1;
bool OK[SIZE]; //nodes done
int D[SIZE]; //distance
int path[SIZE]; //we came to this node from
const int INFINITY=1000000; //big enough number, bigger than any possible path
#define buffer_size 2024
const char *filename = "school.txt";
 
void Print_dists(int global_dist[], int n);
int nproc, id;
MPI_Status status;
int N;

void Print_paths(int global_pred[], int n);
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
void dijk(int s){
  int i,j;
  int tmp, x;
  int pair[2];
  int tmp_pair[2];
  for(i=id;i<N;i+=nproc){
    D[i]=G[s][i];
    OK[i]=false;
    path[i]=s;
  }
  OK[s]=true;
  path[s]=-1;
  for(j=1;j<N;j++){

    tmp=INFINITY;
    for(i=id;i<N;i+=nproc){
      if(!OK[i] && D[i]<tmp){
	x=i;
	tmp=D[i];
      }
    }

    pair[0]=x;
    pair[1]=tmp;

    if(id!=0){ //Slave
      MPI_Send(pair,2,MPI_INT,0,1,MPI_COMM_WORLD);
    }else{ // id==0, Master
      for(i=1;i<nproc;++i){
	MPI_Recv(tmp_pair,2,MPI_INT,i,1,MPI_COMM_WORLD, &status);
	if(tmp_pair[1]<pair[1]){
	  pair[0]=tmp_pair[0];
	  pair[1]=tmp_pair[1];
	}
      }
    }
    MPI_Bcast(pair,2,MPI_INT,0,MPI_COMM_WORLD);
    x=pair[0];
    D[x]=pair[1];
    OK[x]=true;
    for(i=id;i<N;i+=nproc){
      if(!OK[i] && D[i]>D[x]+G[x][i]){
	D[i]=D[x]+G[x][i];
	path[i]=x;
      }
    }
  }
}



main(int argc, char** argv){
  double t1, t2;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc); // get totalnodes
  MPI_Comm_rank(MPI_COMM_WORLD, &id);     // get mynode

  N=Read_G_Adjacency_Matrix();
  N= SIZE; 
 // printf("%d",N);
  //read in the G[][]
  //set actual size
  printf("We are here");
  if(id==0){
    t1=MPI_Wtime();
  }

  dijk(startNode);
  //call the algorithm with the choosen node
 

  if(id==0){
    t2=MPI_Wtime();

    //check the results with some output from G[][] and D[]
    
    printf("time elapsed:%d \n",(t2-t1));
  }

  MPI_Finalize();
  int p =0;
  int r =0;
/*  printf("D[] ARRAY\n");
  for(p = 0; p < SIZE; p++){
   	printf("%d ",D[p]);
	}
  printf("\n\n PATH\n");

  for(p = 0; p < SIZE; p++){
   	printf("%d %d\n ",p,path[p]);
	}
  printf("\n\n");
  printf("G[][] ARRAY\n");
  
  for(p = 0; p < SIZE; p++){
  for(r = 0; r < SIZE; r++){
   	printf("%d ",G[p][r]);
	} printf("\n");
}*/

  Print_dists(D,SIZE);
  printf("\n");  
  Print_paths(path,SIZE);  
}

void Print_paths(int global_pred[], int n) {
    int v, w, count, i;
    int path [SIZE];

    printf("  v     Path 0->v\n");
    printf("----    ---------\n");
    for (v = 0; v < n; v++) {
        printf("%3d:    ", v);
        count = 0;
        w = v;
        while (w != 0) {
            path[count] = w;
            count++;
            w = global_pred[w];
        }
        printf("0 ");
        for (i = count-1; i >= 0; i--)
            printf("%d ", path[i]);
        printf("\n");
    }
//
    //free(paths);
}

void Print_dists(int global_dist[], int n) {
    int v;

    printf("  v    dist 0->v\n");
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

