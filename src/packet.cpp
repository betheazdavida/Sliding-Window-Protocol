#include <iostream>
#include "packet.h"

using namespace std;

PACKET::PACKET(unsigned int seqnum, unsigned char data,
               unsigned char checksum): SOH(1),STX(2),ETX(3){
  this -> seqnum = seqnum;
  this -> data = data;
  this -> checksum = checksum;
}

unsigned int PACKET::getSeqnum(){
    return seqnum;
}

const unsigned char PACKET::getSOH(){
  return SOH;
}

const unsigned char PACKET::getSTX(){
  return STX;
}

unsigned char PACKET::getData(){
  return data;
}

const unsigned char PACKET::getETX(){
  return ETX;
}

unsigned char PACKET::getChecksum(){
  return checksum;
}

bool PACKET::isCheckSumEqual(unsigned char checksum){
  return (this -> checksum == checksum);
}

void PACKET::printPACKET(){
  cout << "SOH : " << SOH << "\n";
  cout << "STX : " << STX << "\n";
  cout << "ETX : " << ETX << "\n";
  cout << "seqnum : " << seqnum << "\n";
  cout << "data : " << data << "\n";
  cout << "checksum : " << checksum << "\n";
}
