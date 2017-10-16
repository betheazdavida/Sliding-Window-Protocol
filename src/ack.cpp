#include "ack.h"
#include <iostream>

using namespace std;


ACK::ACK(unsigned int seqnum,unsigned char AWS, unsigned char checksum): SOH(6) {
  this -> seqnum = seqnum;
  this -> AWS = AWS;
  this -> checksum = checksum;
}

void ACK::printACK(){
  cout << " SOH : " << SOH << "\n";
  cout << " SEQNUM : " << seqnum << "\n";
  cout << " AWS : " << AWS << "\n";
  cout << " Checksum : " << checksum << "\n";
}

unsigned int ACK::getSeqnum(){
  return seqnum;
}

const unsigned char ACK::getSOH(){
  return SOH;
}

unsigned char ACK::getAWS(){
  return AWS;
}

unsigned char ACK::getChecksum(){
  return checksum;
}

bool ACK::isCheckSumEqual(unsigned char checksum){
  return (this -> checksum == checksum);
}
