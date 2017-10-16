#ifndef PACKET_H
#define PACKET_H
#pragma

class PACKET{
	private:
		unsigned int seqnum;
		const unsigned char SOH;
		const unsigned char STX;
		unsigned char data;
		const unsigned char ETX;
		unsigned char checksum;

	public:
		// CTOR
		PACKET(unsigned int seqnum, unsigned char data,unsigned char checksum);
		// GETTER
		unsigned int getSeqnum();
		const unsigned char getSOH();
		const unsigned char getSTX();
		unsigned char getData();
		const unsigned char getETX();
		unsigned char getChecksum();
		// UTILITY
		bool isCheckSumEqual(unsigned char checksum);
		void printPACKET();
}__attribute__((packed));

#endif
