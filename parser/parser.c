

#include <iostream>
#include <string.h>


// Number of vertices in the graph

#define g_size 6
#define buffer_size 2024
const char *filename = "../edge.txt";
using namespace std;
int graph_array [g_size * g_size ];
#define INFINITY 10000000


//driver function to check the program
int printMatrix(){
    int print_count = 0;
    printf("=============\nSize of Array,%d\n\n",g_size);
    for(int u = 0; u < g_size * g_size; u++){

            printf("%d ",graph_array[u]);


        if (print_count == g_size - 1  ) {
            printf("\n");
            print_count = - 1;
        }
        print_count ++;
    }

    return 0;

}

int main()
{
    //initialize every connection to 0
    int count = g_size;
    int g_max = g_size * g_size;
    int prod = 0;

    for(int u = 0; u < g_max ; u++){

            prod = u * g_size + u;
            printf("PROD%d\n",prod);
            if( count == g_size  ){
                graph_array[u] = 0;
                count = - 1;
            }
            else {
                graph_array[u] = INFINITY;

        }
        printf("COunt %d: \n",count);
        count++;

    }

    int nodeU = -1;
    int nodeV = -1;
    int nodeZ = -1;

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
                nodeU = atoi(last_token);
                nodeV = atoi(strtok(NULL, delimiter_characters));
                nodeZ = atoi(strtok(NULL, delimiter_characters));


                graph_array[nodeU * g_size + nodeV] = nodeZ;
                //graph_array[nodeU][nodeV] = nodeZ;
                // graph_array[nodeV][nodeU] = nodeZ;


                printf("Adding: (%d,%d, %d)\n", nodeU, nodeV,nodeZ);
                last_token = strtok(NULL, delimiter_characters);
            }

        }

        if (ferror(input_file)) {
            perror("The following error occurred");
        }

        fclose(input_file);

    }

    printMatrix();




    return 0;
}



