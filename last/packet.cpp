#include <iostream>
#include "packet.h"

using namespace std;

PACKET::PACKET( int seqnum,  char data): SOH(1),STX(2),ETX(3){
  this -> seqnum = seqnum;
  this -> data = data;

  this -> checksum  = generateChecksum();
}

 int PACKET::getSeqnum(){
    return seqnum;
}

const  char PACKET::getSOH(){
  return SOH;
}

const  char PACKET::getSTX(){
  return STX;
}

 char PACKET::getData(){
  return data;
}

const  char PACKET::getETX(){
  return ETX;
}

char PACKET::getChecksum(){
  return checksum;
}

char PACKET::generateChecksum(){
  return ( (char)seqnum+SOH+STX+ETX+data );
}

bool PACKET::isCheckSumEqual(){
  return (this -> generateChecksum() == checksum);
}

void PACKET::printPACKET(){
  cout << "SOH : " << SOH << "\n";
  cout << "STX : " << STX << "\n";
  cout << "ETX : " << ETX << "\n";
  cout << "seqnum : " << seqnum << "\n";
  cout << "data : " << data << "\n";
  cout << "checksum : " << checksum << "\n";
}
