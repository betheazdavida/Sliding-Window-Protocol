#ifndef ACK_H
#define ACK_H
#pragma

typedef struct{
	const unsigned char SOH = 6;
	unsigned int seqnum;
	unsigned char AWS;
	unsigned char checksum;
}__attribute__((packed)) ACK;

#endif
