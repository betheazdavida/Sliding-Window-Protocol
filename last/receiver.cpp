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



using namespace std;


	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
  int slen=sizeof(remaddr);
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
  /* Get Arguments */
  char *filename;
  int windowSize;
  int buffersize;
  int destinationPort;

  vector<char> v;
	vector<bool> packetreceived;
  char* buffer;
	int nr,ns,wr;

  void sendACK(ACK p){ //Packet yg akan dikirim always valid
		cout << "[SEND] ACK for - " << p.getSeqnum()-1 <<". AWS : "<<int(p.getAWS())<<"\n";
    int bufsize = sizeof(ACK);
    char buf[buffersize];
    memcpy(buf,&p,bufsize);
    sendto(fd, buf, bufsize, 0 , (struct sockaddr *)&remaddr, slen);
  }

PACKET receivePACKET(){
  int bufsize = sizeof(PACKET);
  char buf[buffersize];
  PACKET npack(-1,'\0');
  int recvlen = recvfrom(fd, buf, bufsize, 0, (struct sockaddr *)&remaddr, &addrlen);
  PACKET *a = (PACKET *)buf;
  //cout << "SEQNUM : "<<a->getSeqnum()<<" ,recv len : " << recvlen <<" ,Is equal checksum :" << a->isCheckSumEqual()<<"\n";

	if (recvlen == sizeof(PACKET) && a->isCheckSumEqual()) { //Validation
		cout << "[RECEIVE] PACKET SEQNUM : " << a->getSeqnum() <<". Data : "<<a->getData()<<"\n";
    PACKET aret(a->getSeqnum(),a->getData());
    return aret;
  }
  return npack;
}

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


bool isBufferFull(){
	return (nr != 0 && (nr % buffersize) == 0);
}

void addtoVector(int N){
	for (int i=0; i<N; i++){
			v.push_back(buffer[i]);
			packetreceived[i]=false;
	}
}
void clearBuffer(){
	for (int i=0; i<buffersize; i++)
		buffer[i]='\0';
}
bool isFallInWindow(int x){
	return (nr <= x && x <= (nr+wr));
}
bool isPreStored(int x){
	for (int i=x-1; i>=nr; i--)
		if (!packetreceived[i]) return false;
	return true;
}

int main(int argc, char **argv)
{

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

  buffer = new char[buffersize];

	for (int i=0;i<buffersize;i++)
		packetreceived.push_back(false);

  //Vector to save the message
  int currentSeqnum = 0;
  int count;
  bool eof = false;
	nr = 0; ns = 0; wr = windowSize;
  while(1){
      //try to receive some data, this is a blocking call
      cout << "waiting for segment" << endl;
      fflush(stdout);

      PACKET p = receivePACKET();

			if (p.isCheckSumEqual()){
				int x = p.getSeqnum();
				cout <<" OK \n";
				//cout <<"seqnum : "<<x<<" ,data : "<<p.getData()<<"\n";

	      if (x == -1){
						cout <<" END here\n";
						addtoVector((nr%buffersize));
		        break;
				}
				if (isBufferFull()){
					addtoVector(buffersize);
					clearBuffer();
				}
				if (isFallInWindow(x)){
					buffer[x]=p.getData();
					packetreceived[x]=true;
					if (x == nr) nr += 1;
					if (x >= ns ) ns = x+1;
				}
			}
			cout<<"STATS:\n";
			for (int i=0;i<wr;i++)
				cout<<nr+i<<" - "<<packetreceived[nr+i]<<"\n";
			cout<<"\n";

			wr = min(windowSize,buffersize-(nr%buffersize));
      ACK a(nr,wr);
      //cout <<"PACKET SEQNUM : "<<p.getSeqnum()<< " ,ACK SEQNUM : "<<a.getSeqnum()<<"\n";
      sendACK(a);

  }
	cout <<" FILE "<<v.size()<<":\n";
	for (int i = 0; i<nr;i++){
		cout << buffer[i];
	}
	cout<<"\n";
  writeToFile(filename,v);
  delete[] buffer;
  return 0;

}
