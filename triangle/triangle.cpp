#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// C++ program to count triangles.txt
// in a graph. The program is for
// adjacency matrix representation
// of the graph.


// Number of vertices in the graph

#define g_size 4040
#define buffer_size 1024
const char *filename = "fb.txt";
using namespace std;
int graph_array [g_size][g_size];


// function to calculate the
// number of triangles.txt in a
// simple directed/undirected
// graph. isDirected is true if
// the graph is directed, its
// false otherwise
int countTriangle(int graph[g_size][g_size],
                  bool isDirected)
{
    // Initialize result
    int count_Triangle = 0;

    // Consider every possible
    // triplet of edges in graph
    for (int i = 0; i < g_size; i++)
    {
        for (int j = 0; j < g_size; j++)
        {
            for (int k = 0; k < g_size; k++)
            {
                // check the triplet if
                // it satisfies the condition
                if (graph[i][j] && graph[j][k]
                    && graph[k][i])
                    count_Triangle++;
            }
        }
    }

    // if graph is directed ,
    // division is done by 3,
    // else division by 6 is done
    isDirected? count_Triangle /= 3 :
            count_Triangle /= 6;

    return count_Triangle;
}

//driver function to check the program
int printMatrix(){


    for(int i = 0; i < g_size; i++){
        for(int j = 0; j < g_size; j++){
            printf("%d ",graph_array[i][j]);

        }
        printf("\n");

    }

    return 0;

}


int main()
{

        //initialize every connection to 0

        for(int i = 0; i < g_size; i++){
            for(int j = 0; j < g_size; j++){

                graph_array[i][j]=0;
            }

        }

        int nodeX = -1;
        int nodeY = -1;

        const char *delimiter_characters = " ";
        FILE *input_file = fopen( filename, "r" );
        char buffer[ buffer_size ];
        char *last_token;
        int nodes = g_size;


        if( input_file == NULL ){

            fprintf( stderr, "Unable to open file %s\n", filename );

        }else {

            // Read each line into the buffer
            while (fgets(buffer, buffer_size, input_file) != NULL) {


                last_token = strtok(buffer, delimiter_characters);

                while (last_token != NULL) {
                    nodeX = atoi(last_token);
                    nodeY = atoi(strtok(NULL, delimiter_characters));

                    graph_array[nodeX][nodeY]= 1;
                    graph_array[nodeY][nodeX]= 1;
		    
                    last_token = strtok(NULL, delimiter_characters);
                }

            }

            if (ferror(input_file)) {
                perror("The following error occurred");
            }

            fclose(input_file);

        }

    //printMatrix();




    // Create adjacency matrix
    // of an undirected graph
/*    int graph[][V] = { {0, 1, 1, 0},
                       {1, 0, 1, 1},
                       {1, 1, 0, 1},
                       {0, 1, 1, 0}
    };

    // Create adjacency matrix
    // of a directed graph
    int digraph[][V] = { {0, 0, 1, 0},
                         {1, 0, 0, 1},
                         {0, 1, 0, 0},
                         {0, 0, 1, 0}
    };*/

    cout << "\nThe Number of Triangles :  "
         << countTriangle(graph_array, false);
   /* cout << "\n\nThe Number of triangles.txt in directed graph : "
         << countTriangle(digraph, true);*/

	printf("\n");
    return 0;
}


