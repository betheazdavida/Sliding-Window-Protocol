/*
        demo-udp-03: udp-send: a simple udp client
	send udp messages
	sends a sequence of messages (the # of messages is defined in MSGS)
	The messages are sent to a port defined in destinationPort

        usage:  udp-send

        Paul Krzyzanowski
*/

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

  char *filename;
  int windowSize;
  int buffersize;
  char *destinationIp;
  int destinationPort;
  vector<char> v;
  int currentSeqnum=0;

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

  char buffer[buffersize];


	struct sockaddr_in myaddr, remaddr;
  socklen_t addrlen = sizeof(remaddr);
	int fd, i, slen=sizeof(remaddr);
	//char *destinationIp = "127.0.0.1";	/* change this to use a different destinationIp */
	char buf[buffersize];

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

  while (1){
    //send data of sliding window size
    int remainder = (v.size()-1 - currentSeqnum) < windowSize ? v.size() - currentSeqnum : windowSize;
    for (int i = 0; i < remainder; i++){
        //Framing packet
        PACKET p(i+currentSeqnum,v[i + currentSeqnum]);
        p . printPACKET();
        int packet_len = sizeof(p);
        char res[packet_len];

        memcpy(res,&p,packet_len);

        for (int j = 0; j < packet_len; j++) buf[j]=res[j];

        cout <<"BUG" <<endl;

        //insert the data to buffer
        buffer[(currentSeqnum)%256+i] = p.getData();
        cout << "iterator di buffer : " << currentSeqnum+i << endl;
        cout << "isi buffer sesudah: "<< buffer << endl;
        //send the message
        cout << "Packet len : "<<packet_len << endl;
        for (int j = 0; j < packet_len; j++)
          cout << buf[j] << " - ";
        cout<<"\n";
        sendto(fd, buf, packet_len, 0 , (struct sockaddr *)&remaddr, slen);
        cout <<"BUG2" <<endl;

    }

    for(int i = 0; i < remainder; i++){
        //try to receive some data, this is a blocking call
        int recvlen = recvfrom(fd, buf, buffersize, 0, (struct sockaddr *)&remaddr, &addrlen);
        ACK* a = (ACK * )buf;
        //a -> printACK();
        //now slide the window
        if(a->getSeqnum() < v.size()-1){
            currentSeqnum = a->getSeqnum();
        } else {
            //over
            currentSeqnum += 2;
            break;
        }
    }
    if(currentSeqnum >= v.size()-1) break;
    cout << "sekarang ada di sini : " << currentSeqnum << endl;
    if(currentSeqnum == sizeof(buffer)){
        for(int i = 0; i < sizeof(buffer); i++){
            buffer[i] = '\0';
        }
        cout << "ngosongin" << endl;
    }
  }

  //send terminate packet
     //Framing packet
     PACKET p(-1,'\0'); //headernya nol
     int packet_len = sizeof(p);
     char res[packet_len];

     memcpy(res,&p,packet_len);

     for (int j = 0; j < packet_len; j++) buf[j]=res[j];
     buf[packet_len]='\0';
     sendto(fd, buf, packet_len, 0 , (struct sockaddr *)&remaddr, slen);

	close(fd);
	return 0;
}
