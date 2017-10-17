#ifndef PACKET_H
#define PACKET_H
#pragma

class PACKET{
	private:
		int seqnum;
		char data;
		char checksum;
		const  char SOH;
		const  char STX;
		const  char ETX;


	public:
		// CTOR
		PACKET( int seqnum,  char data);
		// GETTER
		int getSeqnum();
		const char getSOH();
		const char getSTX();
		char getData();
		const char getETX();
		char getChecksum();
		// UTILITY
		char generateChecksum();
		bool isCheckSumEqual();
		void printPACKET();
}__attribute__((packed));

#endif
