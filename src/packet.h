#ifndef PACKET_H
#define PACKET_H
#pragma

class PACKET{
	private:
		int seqnum;
		const  char SOH;
		const  char STX;
		char data;
		const  char ETX;
		char checksum;

	public:
		// CTOR
		PACKET( int seqnum,  char data, char checksum);
		// GETTER
		int getSeqnum();
		const char getSOH();
		const char getSTX();
		char getData();
		const char getETX();
		char getChecksum();
		// UTILITY
		bool isCheckSumEqual( char checksum);
		void printPACKET();
}__attribute__((packed));

#endif
