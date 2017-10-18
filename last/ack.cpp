#include "ack.h"
#include <iostream>

using namespace std;


ACK::ACK(int seqnum,char AWS): SOH(6) {
  this -> seqnum = seqnum;
  this -> AWS = AWS;
  this -> checksum = generateChecksum();
}

void ACK::printACK(){
  cout << " SOH : " << SOH << "\n";
  cout << " SEQNUM : " << seqnum << "\n";
  cout << " AWS : " << AWS << "\n";
  cout << " Checksum : " << checksum << "\n";
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


char ACK::generateChecksum(){
  return ( SOH+ (char)seqnum+AWS );
}

bool ACK::isCheckSumEqual(){
  return (generateChecksum() == checksum);
}
