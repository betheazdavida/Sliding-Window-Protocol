#include "ack.h"
#include <iostream>

using namespace std;


ACK::ACK(int seqnum,char AWS, char checksum): SOH(6) {
  this -> seqnum = seqnum;
  this -> AWS = AWS;
  this -> checksum = checksum;
}

void ACK::printACK(){
  cout << " SOH : " << SOH << "\n";
  cout << " SEQNUM : " << seqnum << "\n";
  cout << " AWS : " << AWS << "\n";
  cout << " Checksum : " << int(checksum) << "\n";
}

int ACK::getSeqnum(){
  return seqnum;
}

const char ACK::getSOH(){
  return SOH;
}

char ACK::getAWS(){
  return AWS;
}

char ACK::getChecksum(){
  return checksum;
}

bool ACK::isCheckSumEqual(char checksum){
  return (this -> checksum == checksum);
}
