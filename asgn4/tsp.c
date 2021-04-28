//Derick Pan
//dpan7
//Asgn4
#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *cities;

void dfs(Graph *G, uint32_t v, Path *c, Path *s) {

    graph_mark_visited(G, v);
    uint32_t x;
//If is a hamiltonian path shorter than anything ever seen before
    if (path_length(c) < path_length(s)){
        printf("here?");
        printf("Before C:%u  ", path_vertices(c));
        printf("Before S:%u  \n", path_vertices(s));
        path_copy(s, c);
        printf("After C: %u  ", path_vertices(c));
        printf("After S: %u  \n", path_vertices(s));
    }

    for (uint32_t w = 0; w < graph_vertices(G); w++) { //For all edges

        if (graph_has_edge(G, v, w) && !graph_visited(G, w)) { //Only edges and not visited
            path_push_vertex(c, w, G); //Push it onto the stack
            dfs(G, w, c, s); //test it recursively
            path_pop_vertex(c, &x, G); //Pop the stack after testing all of dfs

        } //If all visited then it stops
        //dfs(G, v, c, s);
    }
    graph_mark_unvisited(G, v);
}

int main(int argc, char *argv[]) {
    int choice;
    bool flago= false;
    bool undirectedflag= false;
    char file[20];
    while ((choice = getopt(argc, argv, "hv:ui:o:")) != -1) {
        switch (choice) {
        case 'h': break; //Print helps

        case 'v': //Verbose printing
            break;


        case 'u': undirectedflag = true; break;

        case 'i': snprintf(file, 20,"%s", optarg); break; //The file
        case 'o': flago=true; break; //stdout
        }

    }
    char buffer[1024];
    char* token;
    int amcities;
    int temp[12];
    //printf("%s", file);
    FILE *read = fopen(file, "r");
    for (int i=0;fgets(buffer, 1024, read); i++){
        if (i==0){
            amcities = atoi(buffer); //Make this global
            //Graph *G = graph_create(26, true);
            cities = calloc(amcities, sizeof(char));

        }
        else if (i-1 < amcities){
            cities[i] = *buffer;

        }
        //printf("%s\n",buffer);
        else if (i>= amcities){
            token = strtok(buffer, " ");
            for (int j=0; token != NULL; j++){
                printf("%s\n",token);
                token = strtok(NULL, " ");
            }

        }
    }


    fclose(read);
}

















/*
int main() {
    Graph *G = graph_create(7, true);
    graph_add_edge(G, 0, 1, 10);
    graph_add_edge(G, 1, 2, 2);
    graph_add_edge(G, 1, 3, 5);
    graph_add_edge(G, 2, 5, 3);
    graph_add_edge(G, 2, 6, 5);
    graph_add_edge(G, 3, 4, 21);
    graph_print(G);
    printf("\n");

    path_push_vertex(p, 0, G);
    printf ("0Path Length: %u\n", path_length(p));  //0

	path_push_vertex(p, 1, G);
    printf ("1Path Length: %u\n", path_length(p)); //10

    #path_push_vertex(p, 2, G)
    printf ("2Path Length: %u\n", path_length(p)); //10
    printf ("Amount of vertices: %u\n", path_vertices(p)); //3

    Path *p = path_create();
    Path *s = path_create();
    dfs(G, 0, p, s);
    char *cities[] = { "El-C", "santee", "lakeside", "chula", "san", "camp", "jam", "lames",
        "lajol", "tijua" };
    FILE *outfile = fopen("outfile.txt", "w");
    path_print(s, outfile, cities);
    path_delete(&p);
    path_delete(&s);
    graph_delete(&G);
}
*/
