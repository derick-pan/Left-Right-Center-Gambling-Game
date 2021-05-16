//Derick Pan
//dpan7
//huffman.c
//#include "huffman.h"
#include "huffman.h"

#include "code.h"
#include "defines.h"
#include "node.h"
#include "pq.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALPHABET 256

Node *build_tree(uint64_t hist[static ALPHABET]) {
    //uint32_t freq;
    Node *temp;
    PriorityQueue *q = pq_create(ALPHABET);
    for (uint32_t i = 0; i < ALPHABET; i++) {

        if (hist[i] > 0) {
            printf("index here: %u \n", i);
            temp = node_create(i, hist[i]);
            enqueue(q, temp);
        }
    }
    //node_delete(&temp); //This node is no longer needed
    Node *l;
    Node *r;
    Node *j;
    while (pq_size(q) >= 2) {
        dequeue(q, &l);
        dequeue(q, &r);
        j = node_join(l, r);
        enqueue(q, j);
    }
    node_delete(&l), node_delete(&r);
    // The one node left in priority queue is the root node
    dequeue(q, &j);
    pq_delete(&q);
    return j;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init(); //Doesn't allocate any memory so make as many as i want

    //While i'm an interior node
    uint8_t temp;
    //If Current node is a leaf then save the code it took to get here
    if (root->left == NULL
        || root->left == NULL) { //If there's a left kid then there's a right, same vice versa
        //Save this code into code table.
        //Already in the code no?
        printf("how many times am i here?\n");
        while (code_pop_bit(&c, &temp)) {

            //table->top = root->symbol-1; //Skip forward to the correct indice
            code_push_bit(table, temp);
            //code_pop_bit(table, &temp );
            //code_push_bit(table, root->symbol / 64)
        }
        return;
    }
    //The current node is an interior node

    //Push, recurse left, pop
    code_push_bit(&c, 0); // Push a 0 because we're going left
    build_codes(root->left, &c); // RECURSE to left link
    code_pop_bit(&c, &temp);

    //Push, recurse right, pop
    code_push_bit(&c, 1);
    build_codes(root->right, &c); // RECURSE to right
    code_pop_bit(&c, &temp); // pop from c
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]);

void delete_tree(Node **root);
