//Derick Pan
//dpan7
//io.c
#include "io.h"

#include "code.h"

#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint64_t bytes_read = 0; // Variable is used in decode.c
uint64_t bytes_written = 0; // Variable is used in encode.c
static uint8_t buf[BLOCK]; // Buffer to hold either read or writing
static int bufind = 0; // Index for the buffer

//Wrapper function to read()
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int bytes = 0; //Number of bytes read with each call to read
    int localbytes = 0; //Number of bytes read total in current call
    while ((bytes = read(infile, buf, nbytes)) > 0) {
        buf += bytes; //Increase position of buffer
        bytes_read += bytes; //Increase total bytes read
        nbytes -= bytes; //Decrease bytes still need to be read
        localbytes += bytes; //Increase bytes just read
    }
    return localbytes; //Return number of bytes just read
}

//Wrapper function to write()
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int bytes = 0; //Number of bytes written in one write
    int localbytes = 0; //Number of bytes writen total in current call
    while ((bytes = write(outfile, buf, nbytes)) > 0) {
        buf += bytes; //Increase position of buffer
        bytes_written += bytes; //Increase total bytes written
        localbytes += bytes; //Increase bytes just written
        nbytes -= bytes; //Decrease bytes still need to be written
    }
    return localbytes; //Return number of bytes just written
}

// Function that doles out bits read one at a time
bool read_bit(int infile, uint8_t *bit) {
    // If buffer is full    or buffer is empty
    if (bufind == BLOCK * 8 || bufind == 0) {
        if (read_bytes(infile, buf, BLOCK) <= 0) { //If file is empty then exit
            bufind = 0; //Reset index
            return false; // return false
        }
        bufind = 0;
    }
    //pass back bit at bufind through pointer
    *bit = (buf[bufind / 8] >> (bufind % 8) & 0x1); //Get the bit
    bufind += 1;
    return true;
}

// Function to write out codes
//Code is based off of Euguene's Pseudocode
void write_code(int outfile, Code *c) {
    //Each bit in the code c is buffered into the buffer
    //Create a loop to iterate over the buffer
    for (uint32_t i = 0; i < c->top; i++) {

        if ((c->bits[i / 8] >> (i % 8) & 0x1) == 1) { // If bit is a 1
            //  Set the bit to 1 at buffer
            buf[bufind / 8] |= 0x1 << (bufind % 8);
        } else { // Else bit is 0
            //  Set the bit to 0 at the buffer location
            buf[bufind / 8] &= ~(0x1 << (bufind % 8));
        }
        bufind += 1;
        //If the Buffer is full, write it!
        if (bufind == BLOCK * 8) {
            write_bytes(outfile, buf, BLOCK);
            bufind = 0;
        }
    }
}

//Writes out the code if the buffer isn't size of BLOCk
void flush_codes(int outfile) { //Write out any leftover buffered bits.
    uint32_t amount = bufind; // Amount of bits to write
    if (bufind % 8 != 0) {
        amount += 8 - (bufind % 8); //In bits
        //For loop to zero out the bits till next byte
        for (uint32_t i = bufind; i < amount; i++) {
            buf[i / 8] &= ~(0x1 << (i % 8));
        }
    }
    write_bytes(outfile, buf, amount / 8);
}
