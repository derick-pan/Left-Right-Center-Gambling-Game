//Derick Pan
//dpan7
//Asgn4

#include "path.h"
#include "graph.h"
#include "stack.h"
#include "vertices.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Path {
    Stack *vertices;  //Vertices comprising path
    uint32_t length;
} Path;

Path *path_create(void){
    Path *p = (Path *) malloc(sizeof(Path));

    p->vertices = stack_create(VERTICES);
    p->length = 0;

    return p;
}

void path_delete(Path **p){
    if (*p && (*p)->vertices) {
        free((*p)->vertices);
        free(*p);
        *p = NULL;
    }
    return;
}

bool path_push_vertex(Path *p, uint32_t v, Graph *G){
    if (stack_full(p->vertices)){
        return false;
    }
    stack_push(p->vertices, v);
    uint32_t counter = 0;
    for (uint32_t i = 0; i < graph_vertices(G); i++) { //Iterate over I
        for (uint32_t j = 0; j < graph_vertices(G); j++) { //Iterate over J
            if (graph_has_edge(G, i, j)) { //If there is an edge
                counter++; //Then add 1 to counter
            }
            if (counter == v && graph_has_edge(G, i, j)) {
                p->length += graph_edge_weight(G, i, j);
                return true;
            }
        }
    }
    return false;
    //Length of path is increased by edge weight
    //Return True if vertex was successfully pushed and false otherwise

}

bool path_pop_vertex(Path *p, uint32_t *v, Graph *G){
    uint32_t x;

    if (!stack_empty(p->vertices)){
        stack_peek(p->vertices, &x); //Gets the current vertx on stack.

        uint32_t counter = 0;
        for (uint32_t i = 0; i < graph_vertices(G); i++) { //Iterate over I
            for (uint32_t j = 0; j < graph_vertices(G); j++) { //Iterate over J
                if (graph_has_edge(G, i, j)) { //If there is an edge
                    counter++; //Then add 1 to counter
                }
                if (counter == x && graph_has_edge(G, i, j)) {
                    p->length -= graph_edge_weight(G, i, j);
                    stack_pop(p->vertices, &x);
                    *v = x;
                    return true;
                }
            }
        }


    }
    return false;
}



//returns number of vertices in the path
uint32_t path_vertices(Path *p){
    return stack_size(p->vertices);
}
//That means you visited all of em


//returns the length of the path
uint32_t path_length(Path *p){
    return p->length;
}


void path_copy(Path *dst, Path *src){
    stack_copy(dst->vertices, src->vertices);
    dst->length =  src->length;
}

void path_print(Path *p, FILE *outfile, char *cities[]){

    outfile = fopen("outfile.txt", "w");

    stack_print(p->vertices, outfile, cities);



    fclose(outfile);



}
