#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <fstream>
#include <string>

#include "packet.h"
#include "ack.h"

using namespace std;

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



/* arguments: filename windowsize buffersize destinationIp destinationPort*/
int main(int argc, char ** argv)
{
    /* Get Arguments */
    char *filename;
    int windowSize;
    int buffersize;
    char *destinationIp;
    int destinationPort;
    if(argc != 6){
        cout << "./recvfile filename windowsize buffersize IP port" << endl;
        exit(0);
    } else {
        filename = argv[1];
        windowSize = atoi(argv[2]);
        buffersize = atoi(argv[3]);
        destinationIp = argv[4];
        destinationPort = atoi(argv[5]);
    }

    /* Create Socket*/
    struct sockaddr_in si_other;
    int sendSocket, i;
    socklen_t slen = sizeof(si_other);

    /* Create message buffer */
    char *buf = new char[buffersize];

    /* Initialize Socket */
    sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(destinationPort);

    if (inet_aton(destinationIp , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    //Vector to be sent
    vector<char> v = readToByte(filename);



    for(int i = 0; i < v.size(); i++){
        //Framing packet
        PACKET p(i,v[i],50);
        int packet_len = sizeof(p);
        char res[packet_len];

        memcpy(res,&p,packet_len);

        for (int j = 0; j < packet_len; j++)
          buf[j]=res[j];
        buf[packet_len]='\0';

        //send the message
        sendto(sendSocket, buf, packet_len, 0 , (struct sockaddr *) &si_other, slen);
        //cout << " BUF LEN : " <<strlen(buf) <<"\n";
        //receive a reply and print it
        //clear the buffer
        for(int k = 0; k < buffersize; k++){
            buf[k] = '\0';
        }

        //try to receive some data, this is a blocking call
        recvfrom(sendSocket, buf, buffersize, 0, (struct sockaddr *) &si_other, &slen);

        cout << "feedback" << buf << endl;
    }

    //send terminate connection
    char msg[1];
    msg[0] = 4;
    msg[1] = '\0';
    sendto(sendSocket, msg, 0, 0 , (struct sockaddr *) &si_other, slen);

    return 0;
}
