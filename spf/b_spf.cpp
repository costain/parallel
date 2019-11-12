#include <iostream>
#include <mpi.h>
using namespace std;
  
const int SIZE=30000; //maximum size of the graph
char G[SIZE][SIZE]; //adjacency matrix
bool OK[SIZE]; //nodes done
int D[SIZE]; //distance
int path[SIZE]; //we came to this node from
const int INFINITY=9999; //big enough number, bigger than any possible path
 
int nproc, id;
MPI_Status status;
int N;

int Read_G_Adjacency_Matrix(){
//actual G[][] adjacency matrix read in


} 
void dijk(int s){ 

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
    
    cout<<"time elapsed: "<<(t2-t1)<<endl;
  }

  MPI_Finalize();
}
