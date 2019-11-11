#include <iostream>
#include <mpi.h>
#include <string.h>
//using namespace std;
#include <stdio.h> 
#include <stdlib.h> 
const int SIZE = 6; //maximum size of the graph
char G[SIZE][SIZE]; //adjacency matrix
bool OK[SIZE]; //nodes done
int D[SIZE]; //distance
int path[SIZE]; //we came to this node from
const int INFINITY=9999; //big enough number, bigger than any possible path
#define buffer_size 2024
const char *filename = "edge.txt";
 
int nproc, id;
MPI_Status status;
int N;

int Read_G_Adjacency_Matrix(){
//actual G[][] adjacency matrix read in
 
int u,v = 0;

for ( u = 0; u < SIZE; u++){
        for(int v = 0; v < SIZE; v++){

            if( u==v){
                G[u][v]=0;
            }
            else {
                G[u][v] = 'i';
            }
        }

    }

    char nodeU = 'a';
    char nodeV= 'a' ;
    char nodeZ ='a';

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
                nodeU = last_token[0];
                nodeV = strtok(NULL, delimiter_characters)[0];
                nodeZ = strtok(NULL, delimiter_characters)[0];

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
  //read in the G[][]
  //set actual size

  if(id==0){
    t1=MPI_Wtime();
  }

  dijk(200);
  //call the algorithm with the choosen node

  if(id==0){
    t2=MPI_Wtime();

    //check the results with some output from G[][] and D[]
    
    printf("time elapsed:%d ",(t2-t1));
  }

  MPI_Finalize();
}
