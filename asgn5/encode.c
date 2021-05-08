//Derick Pan
//dpan7
//Asgn5
//Encode.c
#include "bm.h"
#include "bv.h"
#include "hamming.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char usage[1000] = "SYNOPSIS\n\
  A Hamming(8, 4) systematic code generator.\n\n\
USAGE\n\
  ./encode [-h] [-i infile] [-o outfile]\n\n\
OPTIONS\n\
  -h             Program usage and help.\n\
  -i infile      Input data to encode.\n\
  -o outfile     Output of encoded data.\n\
";

uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}
// Returns the upper nibble of val
uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}
// Packs two nibbles into a byte
uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

int main(int argc, char *argv[]) {
    int choice;
    char infile[20]; //Read the file input from user
    char fileout[100]; //File output for user
    FILE *filein;
    filein = stdin;
    FILE *outfile;
    outfile = stdout;
    while ((choice = getopt(argc, argv, "hi:o:")) != -1) {
        switch (choice) {
        case 'h': fprintf(stderr, "%s", usage); exit(0); // Print helps
        case 'i':
            if (optarg != NULL) { //If argument isn't null
                snprintf(infile, 20, "%s", optarg);
                filein = fopen(optarg, "r");
                if (access(infile, R_OK) != 0) { // if file exists
                    fprintf(stderr, "Error: failed to open infile.\n");
                    exit(0);
                }
            }
            break;
        case 'o':
            if (optarg != NULL) {
                snprintf(fileout, 20, "%s", optarg);
                outfile = fopen(optarg, "w");
                break;
            }
            fprintf(stderr, "Error: failed to open infile.\n");
            exit(0);

        case '?': fprintf(stderr, "%s", usage); exit(0);
        }
    }
    struct stat statbuf;
    fstat(fileno(filein), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    BitMatrix *G = bm_create(4, 8);
    for (uint8_t k = 0; k < 4; k++) { //Left side Diagonal
        bm_set_bit(G, k, k);
    }
    for (uint8_t j = 0; j < 4; j++) { //Rows
        for (uint8_t i = 4; i < 8; i++) { //Columns
            if (i != j + 4) {
                bm_set_bit(G, j, i);
            }
        }
    }
    uint8_t msg1;
    uint8_t msg2;
    int read;
    while ((read = fgetc(filein)) != EOF) { //Every byte we read becomes two message bits
        msg1 = ham_encode(G, lower_nibble(read)); //8 bits long
        msg2 = ham_encode(G, upper_nibble(read)); //8 bits long
        fputc(msg1, outfile);
        fputc(msg2, outfile);
    }
    fclose(filein);
    fclose(outfile);
    bm_delete(&G);
}
