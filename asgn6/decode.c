//Derick Pan
//dpan7
//decode.c

#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char usage[1000] = "SYNOPSIS\n\
  A Huffman decoder.\n\
  Decompresses a file using the Huffman coding algorithm.\n\n\
USAGE\n\
  ./decode [-h] [-i infile] [-o outfile]\n\n\
OPTIONS\n\
  -h             Program usage and help.\n\
  -v             Print compression statistics.\n\
  -i infile      Input file to decompress.\n\
  -o outfile     Output of decompressed data.\n";

uint64_t bytes_read;
int main(int argc, char *argv[]) {
    int choice;
    bool stats = false;
    int infile = 0; //File descriptor for stdin is 0
    int outfile = 1; //File descriptor for stdout is 1
    while ((choice = getopt(argc, argv, "hvi:o:")) != -1) {
        switch (choice) {
        case 'h': fprintf(stderr, "%s", usage); exit(0); // Print helps
        case 'v': stats = true; break; //Print stats of decoding
        case 'i':
            if (optarg != NULL) { //If argument isn't null
                if ((infile = open(optarg, O_RDONLY)) == -1) {
                    fprintf(stderr, "Error: failed to open %s.\n", optarg);
                    exit(1);
                }
            }
            break;
        case 'o':
            if (optarg != NULL) {
                outfile = open(optarg, O_WRONLY | O_CREAT);
                break;
            }
            fprintf(stderr, "Error: failed to open infile.\n");
            exit(1);
        case '?': fprintf(stderr, "%s", usage); exit(1);
        }
    }

    uint8_t readingbuff[sizeof(Header)]; // Buffer for bytes while reading.
    Header myheader; // Initalize the Header
    myheader.permissions = 0, myheader.tree_size = 0, myheader.file_size = 0, myheader.magic = 0;

    /* ################## Step 1&2 #################  */
    // Authenticate MAGIC Number

    read_bytes(infile, readingbuff, sizeof(Header)); // Read the header ONLY

    for (int i = 3; i >= 0; i--) { //Grabbing Magic Number
        myheader.magic <<= 8;
        myheader.magic |= readingbuff[i];
    }
    if (myheader.magic != MAGIC) {
        fprintf(stderr, "Invalid Magic Number.\n");
        exit(1);
    }

    /* ### Grabbing all of the information from the header of infile ### */
    for (int i = 5; i >= 4; i--) {
        myheader.permissions <<= 8;
        myheader.permissions |= readingbuff[i];
    }
    for (int i = 7; i >= 6; i--) {
        myheader.tree_size <<= 8;
        myheader.tree_size |= readingbuff[i];
    }
    for (int i = 15; i >= 8; i--) {
        myheader.file_size <<= 8;
        myheader.file_size |= readingbuff[i];
    }
    fchmod(outfile, myheader.permissions); //Set perms of outfile

    /* ################## Step 3 ###################  */
    // Reconstruct the Huffman Tree
    uint8_t temp; // A buffer
    uint8_t tree[myheader.tree_size]; // The dumped tree

    for (uint16_t i = 0; i < myheader.tree_size; i++) { // Read&Save tree dump
        read_bytes(infile, &temp, 1);
        tree[i] = temp;
    }

    Node *root = rebuild_tree(myheader.tree_size, tree); // Rebuild the Tree

    uint64_t decodedsym = 0; // Counter for amount of decoded symbols
    Node *walk = root; // Copy of the root node
    uint8_t writeout[BLOCK]; // Buffer of symbols
    uint16_t bufind = 0; // Buffer Index used when writing symbols

    while (myheader.file_size != decodedsym) {
        // If node am at a leaf then add node->symbol to the buffer.
        if (walk->left == NULL && walk->right == NULL) {
            writeout[bufind] = walk->symbol;
            bufind += 1;
            decodedsym += 1;
            walk = root;
        }
        read_bit(infile, &temp);
        if (bufind == BLOCK) { // If buffer is full then write it out.
            write_bytes(outfile, writeout, BLOCK);
            bufind = 0;
        }
        if (temp == 0) {
            //Walk down to left child
            walk = walk->left;
        } else {
            //Walk down to right
            walk = walk->right;
        }
    }
    if (bufind != 0) { // Flush out left over bits
        write_bytes(outfile, writeout, bufind);
    }

    /* ### Print the statistics ### */
    if (stats == true) {
        double spacesave = (100 * (1 - ((double) bytes_read / myheader.file_size)));
        fprintf(stderr, "Compressed file size: %lu bytes\n\
Decompressed file size: %lu bytes\n\
Space saving: %.2lf%%\n",
            bytes_read, myheader.file_size, spacesave);
    }

    /* ### Free leftover memory ### */
    delete_tree(&root);
    close(infile);
    close(outfile);
}
