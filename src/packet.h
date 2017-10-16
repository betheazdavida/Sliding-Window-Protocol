#ifndef PACKET_H
#define PACKET_H

typedef struct{
	unsigned char SOH;
	unsigned int seqnum;
	unsigned char STX;
	unsigned char data;
	unsigned char ETX;
	unsigned char checksum;
}__attribute__((packed)) PACKET;

#endif
