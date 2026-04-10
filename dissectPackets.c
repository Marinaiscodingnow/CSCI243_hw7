//File: dissectPackets.c
//Author: Marina Kania
//Purpose: Read a binary file of packets, print # of packets,
//and header data of each packet.
////////////////////////////////////////////

#define _DEFAULT_SOURCE
#include <arpa/inet.h>
#include <stdint.h>
#include <endian.h>

#define MAX_PACKET_SIZE 2048
#define HEADER_SIZE 20

static const char *protocol_name(unsigned char proto){
    switch(proto){
        case 1: return "ICMP";
        case 6: return "TCP";
        case 17: return "UDP";
        default: return "UNKNOWN";
    }
}

//Decode and print the first 20 bytes of the IPv4 header
static void print_ivp4_header(const unsigned char *buf){
    //Byte 0: Version
    unsigned char version = (buf[0] >> 4) & 0x0F;
    unsigned char ihl= buf[0] & 0x0F;

    //Byte 1: TOS:

    //Byte 2-3: total length

    //Byte 4-5: Identification

    //Byte 6: IP flags

    //Byte 7: Fragment offset

    //Byte 8: TTL

    //Byte 9: protocol

    //Byte 10-11: Header Checksum

    //Byte 12-15: Source Address

    //Byte 16-19: Destination Address
}

//Get the file name from the command line, and if no name, print err
//Open file, with relevant err message
//Read the count of packets and iterate through the packets 
//Print packets heading
//Read packet lenfth preceding data block
//Decode and print each field in dirst 20 bytes of header
//Close file
int main(int argc, char *argv[]){
    int packet_count;
    if(argc < 2){
        perror("Error: No input file specified.");
        return EXIT_FAILURE;
    }
    //Open file
    FILE *fp = fopen(argv[1], "rb");
    if(fp == NULL){
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    //Read the count of packets needed
    if(fread(&packet_count, sizeof(int), 1, fp) !=1){
        perror("Error: Failed to read packet count.");
        fclose(fp);
        return EXIT_FAILURE;
    }
    //Print header
    printf("File %s contains %d Packets", argv[1], packet_count);

    unsigned char buf[MAX_PACKET_SIZE];
    int exit_status = EXIT_SUCCESS;

    //Iterate through the packets
    for(int i =0; i < packet_count; i++){
        printf("==>Packet %d", i+1);
        
        int packet_length = 0;
        if(fread(&packet_length, sizeof(int), 1, fp) != 1){
            perror("Error: could not read the length for the packet %d.\n", i);
            exit_status = EXIT_FAILURE;
            break;
        }

        if(fread(buf, sizeof(char), packet_length, fp)!=(size_t)packet_length){
            perror("Error: could not read data for packet %d.\n", i);
            exit_status = EXIT_FAILURE;
            break;
        }

        if(packet_length >= 20){
            print_ipv4_header(buf);
        }else{
            perror("Warning: packet %d is too short", i);
            exit_status = EXIT_FAILURE;
        }
    }
    //Close the file and return success
    fclose(fp);
    return exit_status;
}
