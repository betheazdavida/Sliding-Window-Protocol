#ifndef ACK_H
#define ACK_H
#pragma

class ACK {
  private:
    int seqnum;
    const char SOH;
    char AWS;
    char checksum;

  public:
    // CTOR
    ACK(int seqnum,char AWS, char checksum);
    // Getter
    int getSeqnum();
    const char getSOH();
    char getAWS();
    char getChecksum();
    // Utility
    bool isCheckSumEqual(char checksum);
    void printACK();
}__attribute__((packed));

#endif
