/*
        demo-udp-03: udp-recv: a simple udp server
	receive udp messages

        usage:  udp-recv

        Paul Krzyzanowski
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "packet.h"
#include "ack.h"

#define BUFSIZE 2048

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


int main(int argc, char **argv)
{
  /* Get Arguments */
  char *filename;
  int windowSize;
  int buffersize;
  int destinationPort;
  if(argc != 5){
      cout << "./recvfile filename windowsize buffersize port" << endl;
      exit(0);
  }
  else {
      filename = argv[1];
      windowSize = atoi(argv[2]);
      buffersize = atoi(argv[3]);
      destinationPort = atoi(argv[4]);
  }

	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
  int slen=sizeof(remaddr);
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	unsigned char buf[BUFSIZE];	/* receive buffer */


	/* create a UDP socket */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(destinationPort);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

  //Vector to save the message
  vector<char> v;
  char buffer[buffersize];
  int currentSeqnum = 0;
  int count;
  bool eof = false;
  while(!eof){
      //try to receive some data, this is a blocking call
      cout << "waiting for segment" << endl;
      fflush(stdout);
      int recv_len = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&remaddr, &addrlen);
      printf("LEN : %d\n",recv_len);
      for (int j = 0; j < sizeof(PACKET); j++)
        cout << buf[j] << " - ";
      cout << "\n";

      if(recv_len != 0){ //dapet segment
        PACKET* pp = (PACKET *) buf;
        //endoffile
        if(pp->getSeqnum() == -1){
          eof=true;
        }
        //print details of the client/peer and the data received
        cout << "Received packet from " << inet_ntoa(remaddr.sin_addr) << ":" << ntohs(myaddr.sin_port) << endl;
        pp -> printPACKET();
        if (pp -> isCheckSumEqual()){
          buffer[currentSeqnum] = pp -> getData(); //copy data
          currentSeqnum++;
          if (currentSeqnum == sizeof(buffer)){ // jika penuh maka kosongkan
            for(int i = 0; i < sizeof(buffer); i++){
              v.push_back(buffer[i]);
              buffer[i] = '\0';
            }
            currentSeqnum=0;
          }
          int aws = (sizeof(buffer)-currentSeqnum) > windowSize ? windowSize : sizeof(buffer)-currentSeqnum ;
          ACK a(pp->getSeqnum()+1, aws, '\50');
          int ack_len = sizeof(a);
          char res[ack_len];
          memcpy(res, &a, ack_len);
          //Send ACK
          sendto(fd, res, ack_len, 0, (struct sockaddr *)&remaddr, slen);
        }
      }
  }

  for(int i = 0; i < strlen(buffer); i++){
    v.push_back(buffer[i]);
  }
  writeToFile(filename,v);


}
