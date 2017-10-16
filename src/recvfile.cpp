#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#include "ack.h"
#include "packet.h"

using namespace std;


void writeToFile(char* filename, vector<char> v){

  // Prepare for write file
  ofstream outfile (filename, ofstream::binary);

  // Get size
  long size = v.size();

  // Allocate mem for buffer
  char* buffer = new char[size];

  // copy buffer from vector
  for (int i=0; i < size; i++)
    buffer[i]=v[i];

  // Write the file
  outfile.write (buffer,size);

  // release dynamically-allocated memory & close file
  delete[] buffer;
  outfile.close();
}


/* arguments: filename windowsize buffersize port */
int main(int argc, char ** argv)
{
    /* Get Arguments */
    char *filename;
    int windowSize;
    int buffersize;
    int destinationPort;
    if(argc != 5){
        cout << "./recvfile filename windowsize buffersize port" << endl;
        exit(0);
    } else {
        filename = argv[1];
        windowSize = atoi(argv[2]);
        buffersize = atoi(argv[3]);
        destinationPort = atoi(argv[4]);
    }

    /* Create Socket */
    struct sockaddr_in si_me, si_other;
    int receiveSocket, i, recv_len;
    socklen_t slen = sizeof(si_other);

    //akan dipake buat framenya
    char *buf = new char[buffersize];

    //create a UDP socket
    receiveSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(destinationPort);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    bind(receiveSocket, (struct sockaddr*)&si_me, sizeof(si_me));

    //Vector to save the message
    vector<char> v;

    //keep listening for data
    while(1) {
        cout << "Waiting for data..." << endl;
        fflush(stdout);

        //try to receive some data, this is a blocking call
        recv_len = recvfrom(receiveSocket, buf, buffersize, 0, (struct sockaddr *) &si_other, &slen);

        PACKET* pp = (PACKET *) buf;
                //print details of the client/peer and the data received
        cout << "Received packet from " << inet_ntoa(si_other.sin_addr) << ":" << ntohs(si_other.sin_port) << endl;
        //pp -> printPACKET();
        PACKET p(pp->getSeqnum(),pp->getData(),pp->getChecksum());
        for(int k = 0; k < buffersize; k++){
            buf[k] = '\0';
        }
        //save the message if save


        if(recv_len == 0 ){
            break;
        } else {
          v.push_back(p.getData());
        }

        //asumsi checksum bener
        int n_pkg_rcv = recv_len/sizeof(PACKET);
        //cout << "N PKG RCV: " << n_pkg_rcv <<"\n\n";

        int lastPtr = 0;
        for (int i=0; i< n_pkg_rcv; i++){
          //Generate fungsi checksum dari suatu paket
          char checksum_hsl = 50;

          //cout << " SEQ NUM : " << p . getSeqnum() << "\n";
          //cout << " P CHECKSUM : " << p . getChecksum() << "\n";


          ACK a(p.getSeqnum(),n_pkg_rcv,p.getChecksum());
          //a.printACK();
          if (a.isCheckSumEqual(checksum_hsl)){
            //cout << "checksum true " <<endl;
            int ack_len = sizeof(a);
            char res[ack_len];
            memcpy(res, &a, ack_len);
            for (int j = lastPtr; j < ack_len; j++){
              buf[j]=res[j-lastPtr];
            }
            lastPtr+=ack_len;
          } else {
            break;
          }
        }

        //cout << "LAST PTR : " << lastPtr <<"\n";


        if (lastPtr>0){
          //now reply the client with the same data
          sendto(receiveSocket, buf, lastPtr, 0, (struct sockaddr*) &si_other, slen);
        }
    }

    writeToFile(filename,v);

    return 0;
}
