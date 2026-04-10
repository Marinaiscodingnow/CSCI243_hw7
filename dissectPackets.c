//File: dissectPackets.c
//Author: Marina Kania
//Purpose: Read a binary file of packets, print # of packets,
//and header data of each packet.
////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
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
static void print_ipv4_header(const unsigned char *buf){
    //Byte 0: Version
    unsigned char version = (buf[0] >> 4) & 0x0F;
    unsigned char ihl= buf[0] & 0x0F;

    //Byte 1: TOS:
    unsigned char tos = buf[1];
    //Byte 2-3: total length
    unsigned int total_length = ntohs(*(uint16_t *)(buf +2));
    //Byte 4-5: Identification
    unsigned int identification = ntohs(*(uint16_t *)(buf + 4));
    //Byte 6-7: IP flags
    //Convert the 16 bit word from network order, then mask
    uint16_t flags_frag     = ntohs(*(uint16_t *)(buf + 6));
    unsigned char ip_flags  = (flags_frag >> 13) & 0x07; //Top 3
    uint16_t frag_offset    = flags_frag & 0x1FFF; //Lower 13
    //Byte 8: TTL
    unsigned char ttl = buf[8];
    //Byte 9: protocol
    unsigned char protocol = buf[9];
    //Byte 10-11: Header Checksum
    uint16_t checksum = ntohs(*(uint16_t *)(buf + 10));

    //Print it
    printf("Version:               0x%x (%u)\n", version, version);
    printf("IHL (Header Length):   0x%x (%u)\n",ihl,ihl);
    printf("Type of Service (TOS): 0x%x (%u)\n",tos,tos);
    printf("Total Length:          0x%x (%u)\n",total_length,total_length);
    printf("Identification:        0x%x (%u)\n",identification, identification);
    printf("IP Flags:              0x%x (%u)\n",ip_flags,ip_flags);
    printf("Fragment Offset:       0x%x (%u)\n",frag_offset,frag_offset);
    printf("Time To Live (TTL):    0x%x (%u)\n",ttl,ttl);
    printf("Protocol:              %s 0x%x (%u)\n",
           protocol_name(protocol), protocol, protocol);
    printf("Header Checksum:       0x%x (%u)\n",checksum,checksum);
    //Byte 12-15: Source Address
    printf("Source Address:        %u.%u.%u.%u\n",
           buf[12], buf[13], buf[14], buf[15]);
    //Byte 16-19: Destination Address
    printf("Destination Address:   %u.%u.%u.%u\n",
           buf[16], buf[17], buf[18], buf[19]);
}


//Get the file name from the command line, and if no name, print err
//Open file, with relevant err message
//Read the count of packets and iterate through the packets 
//Print packets heading
//Read packet lenfth preceding data block
//Decode and print each field in dirst 20 bytes of the header
//Close file
int main(int argc, char *argv[]){
    int packet_count;
    if(argc < 2){
        fprintf(stderr, "Error: No input file specified.");
        return EXIT_FAILURE;
    }
    //Open file
    FILE *fp = fopen(argv[1], "rb");
    if(fp == NULL){
        fprintf(stderr, "Error opening file");
        return EXIT_FAILURE;
    }

    //Read the count of packets needed
    if(fread(&packet_count, sizeof(int), 1, fp) !=1){
        fprintf(stderr, "Error: Failed to read packet count.");
        fclose(fp);
        return EXIT_FAILURE;
    }
    //Print header
    printf("==== File %s contains %d Packets\n", argv[1], packet_count);

    unsigned char buf[MAX_PACKET_SIZE];
    int exit_status = EXIT_SUCCESS;

    //Iterate through the packets
    for(int i =0; i < packet_count; i++){
        printf("==>Packet %d\n", i+1);
        
        int packet_length = 0;
        if(fread(&packet_length, sizeof(int), 1, fp) != 1){
            fprintf(stderr, 
                "Error: could not read the length for the packet %d.\n", i);
            exit_status = EXIT_FAILURE;
            break;
        }

        if(fread(buf, sizeof(char), packet_length, fp)!=(size_t)packet_length){
            fprintf(stderr, "Error: could not read data for packet %d.\n", i);
            exit_status = EXIT_FAILURE;
            break;
        }

        if(packet_length >= 20){
            print_ipv4_header(buf);
        }else{
            fprintf(stderr, "Warning: packet %d is too short", i);
            exit_status = EXIT_FAILURE;
        }
    }
    //Close the file and return success
    fclose(fp);
    return exit_status;
}


