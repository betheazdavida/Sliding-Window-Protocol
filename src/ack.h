#ifndef ACK_H
#define ACK_H
#pragma

class ACK {
  private:
    unsigned int seqnum;
    const unsigned char SOH;
    unsigned char AWS;
    unsigned char checksum;

  public:
    // CTOR
    ACK(unsigned int seqnum,unsigned char AWS, unsigned char checksum);
    // Getter
    unsigned int getSeqnum();
    const unsigned char getSOH();
    unsigned char getAWS();
    unsigned char getChecksum();
    // Utility
    bool isCheckSumEqual(unsigned char checksum);
    void printACK();
}__attribute__((packed));

#endif
