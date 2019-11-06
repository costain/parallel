#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "graph.h"
#include "queue.h"
#include "list.h"
#define SIZE 9877
#define BUFFER_SIZE 1024
const char *filename = "ca_read.txt";

struct AdjListNode { 
    int dest; 
    struct AdjListNode* next; 
}; 

int getStartNode(char* line);
int getEndNode(char* line);
void bfs(MPI_Comm com, int rank, int world_size, struct Graph* graph, int graph_size);

/* README COMMENTS!!!
 * This comment block is meant for discussing the speed ups over the serial
 * implementation.
 * The serial BFS works by searching the breadth of options instead of drilling to the leafs of a tree like in depth first.
 * My parallel implementation does this by starting a BFS at the same time across different starting points of the graph.
 * Ideally, these starting points should be optimized.
 * I like to think of it like a voronoi diagram. 
 * I want to pick starting points so that the resulting area searched by each resembles a voronoi diagram.
 * I broadcast the visited nodes across all processes.
 * This is done so that work is not repeated across processes. 
 * In conclusion, the parallel implementation offers speed ups because of the N processes searching at the same time.
 * This divides the work of searching the graph N times.
 * Work is not repeated because the list of visited nodes is broadcasted to all other nodes.
 *
 */
int main() {
    MPI_Init(NULL, NULL);
    MPI_Comm com = MPI_COMM_WORLD;
    int world_size;
    int rank;
    MPI_Comm_rank(com, &rank);
    MPI_Comm_size(com, &world_size);
    FILE *texasRoads = fopen("./email-Enron.txt", "r");
    char line[256];
    char number[20];
    int i;
    int nodes;
    int nodeX = -1;
    int nodeY = -1;

    const char *delimiter_characters = " ";
    FILE *input_file = fopen( filename, "r" );
    char buffer[ BUFFER_SIZE ];
    char *last_token;
    nodes = 9877;
    struct Graph *graph = createGraph(nodes);

    if( input_file == NULL ){

        fprintf( stderr, "Unable to open file %s\n", filename );

    }else {

        // Read each line into the buffer
        while (fgets(buffer, BUFFER_SIZE, input_file) != NULL) {

            // Write the line to stdout
            //fputs( buffer, stdout );

            // Gets each token as a string and prints it
            last_token = strtok(buffer, delimiter_characters);

            while (last_token != NULL) {
                nodeX = atoi(last_token);
                nodeY = atoi(strtok(NULL, delimiter_characters));

    //            printf("Adding: (%d,%d)\n", nodeX, nodeY);
                addEdge(graph, nodeX, nodeY);
                last_token = strtok(NULL, delimiter_characters);
            }

        }

        if (ferror(input_file)) {
            perror("The following error occurred");
        }

    fclose(input_file);
    if (rank == 0) {
        printf("Total nodes in graph: %d\n", nodes);
        char* running = malloc(world_size*sizeof(char));    
        int i;
        for (i=0; i < world_size; i++) {
            running[i] = 1;
        }
        char run = 1;
        char status;
        while (run) {
            char sum = 0;
            for (i = 0; i<world_size; i++) {
                if (i != 0 && running[i] == 1) {
                    MPI_Recv(&status, 1, MPI_CHAR, i, 0, com, MPI_STATUS_IGNORE);
                    running[i] = status;
                }
                sum += running[i];
            }
            if (sum == 1) {
                run = 0;
            } else {
                for (i = 1; i<world_size; i++) {
                    if (running[i] == 1) {
                        MPI_Send(running, world_size, MPI_CHAR, i, 0, com);
                    }
                }
            }
        }
    } else {
        bfs(com, rank, world_size, graph, nodes);
    }
    printf("Process %d done\n", rank);
    MPI_Barrier(com);
    MPI_Finalize();
    //printGraph(graph); 
    return 0;
}

void bfs(MPI_Comm com, int rank, int world_size, struct Graph* graph, int graph_size) {
    struct queue* frontier = createQueue();
    char* visited = calloc(graph_size, sizeof(char));
    char* changedBuffer = calloc(graph_size, sizeof(char));
    char* running = malloc(world_size*sizeof(char));
    int start = (graph_size/world_size) * rank;
    int count = 0;
    visited[start] = 1;
    char run = 1;
    enqueue(frontier, start);
    
    // BFS loop
    while(!isEmpty(frontier)) {
        MPI_Send(&run, 1, MPI_CHAR, 0, 0, com);
        MPI_Recv(running, world_size, MPI_CHAR, 0, 0, com, MPI_STATUS_IGNORE);
        char* changed = calloc(graph_size, sizeof(char));
        int vert = dequeue(frontier);
        //printf("Cur: %d\n", vert);
        struct AdjListNode* temp = getHeadNode(graph, vert);
        while (temp) {
            int adjVert = temp->dest;
            if (visited[adjVert] == 0) {
                changed[adjVert] = 1;
                count++;
                visited[adjVert] = 1;
                enqueue(frontier, adjVert);
            }
            temp = temp->next;
        }
        int i;
        int j;
        for (i = 1; i < world_size; i++) {
            if (running[i] && i != rank) {
                MPI_Send(changed, graph_size, MPI_CHAR, i, 0, com);
            } else if (running[i]) {
                for (j = 1; j < world_size; j++) {
                    if (running[j] && j != rank) {
                        MPI_Recv(changedBuffer, graph_size, MPI_CHAR, j, 0, com, MPI_STATUS_IGNORE);
                        int k;
                        for (k = 1; k < graph_size; k++) {
                            if (changedBuffer[k] == 1) {
                                visited[k] = 1;
                            }
                        }
                    }
                }
            }
        }
        free(changed);
    }
    printf("Nodes searched by process %d: %d\n", rank, count);
    run = 0;
    MPI_Send(&run, 1, MPI_CHAR, 0, 0, com);
}



int getStartNode(char* line) {
    int i = 0;
    char index = *(line+i);
    while(index != '\t') {
       index = *(line + ++i);
    }
    char start[i];
    memcpy(start, line, i-1);
    start[i-1] = '\0';
    int node = atoi(start);
    return node;
}

int getEndNode(char* line) {
    int i = 0;
    char index = *(line+i);
    while(index != '\t') {
       index = *(line + ++i);
    }
    while (index == '\t') {
        index = *(line + ++i);
    }
    int endNodeIndex = i;
    while(index != '\n') {
       index = *(line + ++i);
    }
    char end[i-endNodeIndex];
    memcpy(end, line+endNodeIndex, i-endNodeIndex-1);
    end[i-endNodeIndex-1] = "\0";
    int node = atoi(end);
    //printf("%d\n", i-endNodeIndex-1);
    //printf("i=%d, nodeindex=%d\n", i,endNodeIndex);
  //  printf("%d\n", ids[1]);
    return node;
}
