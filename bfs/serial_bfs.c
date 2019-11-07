#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./graph.h"
#include "./queue.h"
#include "./list.h"
#define SIZE 4039
#define BUFFER_SIZE 1024
const char *filename = "fb.txt";


struct AdjListNode { 
    int dest; 
    struct AdjListNode* next; 
}; 

int getStartNode(char* line);
int getEndNode(char* line);
void bfs(struct Graph* graph, int graph_size, int start);

int main() {

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
    nodes = SIZE;
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

    }

    //printf("Nodes: %d\n", nodes);


    bfs(graph, nodes, 0);
    //printGraph(graph); 
    return 0;
}

void bfs(struct Graph* graph, int graph_size, int start) {
    struct queue* frontier = createQueue();
    int *visited = calloc(graph_size, sizeof(int));
    visited[start] = 1;
    enqueue(frontier, start);
    int count = 0;

    while(!isEmpty(frontier)) {
        // Print
        int vert = dequeue(frontier);
        //printf("Cur: %d\n", vert);
        struct AdjListNode* temp = getHeadNode(graph, vert);
        while (temp) {
            int adjVert = temp->dest;
            if (visited[adjVert] == 0) {
                count++;
                visited[adjVert] = 1;
                enqueue(frontier, adjVert);
            }
            temp = temp->next;
        }
    }
    printf("Managed to visit  %d nodes!\n",count);
    printf("Breadth First done!\nCostaincodes");
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
