//File: dissectPackets.c
//Author: Marina Kania
//Purpose: Read a binary file of packets, print # of packets,
//and header data of each packet.
////////////////////////////////////////////

#define _DEFAULT_SOURCE
#include <arpa/inet.h>
#include <stdint.h>
#include <endian.h>



//Get the file name from the command line, and if no name, print err
//Open file, with relevant err message
//Read the count of packets and iterate through the packets 
//Print packets heading
//Read packet lenfth preceding data block
//Decode and print each field in dirst 20 bytes of header
//Close file
int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Error: No input file specified.\n");
        return EXIT_FAILURE;
    }
    FILE *fp = fopen(argv[1], "rb");
    if(fp == NULL){
        perror("Error opening file");
        return EXIT_FAILURE;
    }
}
