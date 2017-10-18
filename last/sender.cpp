#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fstream>

#include "packet.h"
#include "ack.h"

using namespace std;
#define MSGS 5	/* number of messages to send */


struct sockaddr_in myaddr, remaddr;
socklen_t addrlen = sizeof(remaddr);
int fd,slen=sizeof(remaddr);
char* buffer;
int na,nt,wt;
vector<bool> packetreceived;
char *filename;
int windowSize;
int buffersize;
char *destinationIp;
int destinationPort;
vector<char> v;

void sendPACKET(PACKET p){ //Packet yg akan dikirim always valid
  cout << "[SEND] PACKET SEQNUM : " << p.getSeqnum() <<"\n";
  int bufsize = sizeof(PACKET);
  char buf[buffersize];
  memcpy(buf,&p,bufsize);
  sendto(fd, buf, bufsize, 0 , (struct sockaddr *)&remaddr, slen);
}

ACK receiveACK(){
  int bufsize = sizeof(ACK);
  char buf[buffersize];
  static ACK nack(-1,'\0');
  int recvlen = recvfrom(fd, buf, bufsize, 0, (struct sockaddr *)&remaddr, &addrlen);
  ACK *a = (ACK *)buf;
  if (recvlen == sizeof(ACK) && a->isCheckSumEqual()) { //Validation
    cout <<"[RECEIVE]" << "ACK for - " <<a->getSeqnum()-1<<"\n";
    ACK aret(a->getSeqnum(),a->getAWS());
    return aret;
  }
  return nack;
}

void sendMsg(){
  for (int i =0; i < wt; ++i){
    cout <<(nt+i <= na+wt)<<" - "<<((nt+i)<=v.size()) <<" - "<<!packetreceived[nt+i]<<"\n";
    if (nt+i <= na+wt && (nt+i)< v.size() && !packetreceived[nt+i]){
      //cout <<"packet-"<<nt+i<<" sent "<<buffer[(nt+i)%buffersize]<<"\n";
      PACKET p(nt+i,buffer[(nt+i)%buffersize]);
      sendPACKET(p);
    }
  }
}

void recvMsg(){
  //for (int i = 0; i < wt; i++){
      ACK a = receiveACK();
      if (a.getSeqnum() != -1){ //validation
        packetreceived[a.getSeqnum()-1]=true;
        na = max(na,a.getSeqnum()); //update the Largest ACK received
      }

    //cout <<"pakcket-"<<a.getSeqnum()-1<<" : "<<packetreceived[a.getSeqnum()-1]<<"\n";
  //}
}

void emptyBuffer(){
  for (int i = 0; i<buffersize; ++i){
      packetreceived[i]=false;
      buffer[i]='\0';
  }
}

void fillBuffer(const vector<char>& v){
  for (int i =0; i<buffersize && i+nt < v.size(); ++i)
    buffer[i]=v[i+nt];
}



vector<char> readToByte(char* filename){

  // Read file
  ifstream infile(filename,ifstream::binary);

  // Get size in byte
  infile.seekg (0,infile.end);
  long size = infile.tellg();
  infile.seekg (0);

  // allocate memory for file content
  char* buffer = new char[size];

  // read file and copy to buffer
  infile.read (buffer,size);

  // Prepare for return vector
  static vector<char> v_buf;
  for (int i = 0; i < size; i++)
    v_buf.push_back(buffer[i]);

  // release dynamically-allocated memory & close file
  delete[] buffer;
  infile.close();

  return v_buf;

}

int main(int argc, char** argv)
{



  /* Get Arguments */
  if(argc != 6){
      cout << "./sendfile filename windowsize buffersize IP port" << endl;
      exit(0);
  } else {
      filename = argv[1];
      windowSize = atoi(argv[2]);
      buffersize = atoi(argv[3]);
      destinationIp = argv[4];
      destinationPort = atoi(argv[5]);
  }
  buffer = new char[buffersize];
  for (int i=0; i<buffersize;i++)
    packetreceived.push_back(false);


	/* create a socket */

	if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
		printf("socket created\n");

	/* bind it to all local addresses and pick any port number */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	/* now define remaddr, the address to whom we want to send messages */
	/* For convenience, the host address is expressed as a numeric IP address */
	/* that we will convert to a binary format via inet_aton */

	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(destinationPort);
	if (inet_aton(destinationIp, &remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	/* now let's send the messages */

  v = readToByte(filename);
  nt = 0; na = 0; wt = windowSize;
  fillBuffer(v);
  while (nt < v.size()){
    cout << " NT : " << nt <<" FILESIZE : "<<v.size()<<"\n";
    sendMsg();
    recvMsg();
    while(packetreceived[nt]&&nt < v.size()){
      nt++;
      if ( (buffersize - (nt%buffersize)) < windowSize)
        wt = buffersize - (nt%buffersize);
      else wt = windowSize;

      if (nt == buffersize){
        emptyBuffer();
        fillBuffer(v);
      }
    }
  }

  //send terminate packet
     //Framing packet
     PACKET p(-1,'\0'); //headernya nol
     sendPACKET(p);
  delete[] buffer;
	close(fd);
  cout<<" FILE SIZE : "<<v.size()<<"\n";
	return 0;
}
