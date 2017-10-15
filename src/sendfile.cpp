/*
    Simple udp client
*/
#include <iostream>
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;
 
//#define SERVER "127.0.0.1"
//#define BUFLEN 256  //Max length of buffer
//#define PORT 8000   //The port on which to send data
 
/* arguments: filename windowsize buffersize destinationIp destinationPort*/
int main(int argc, char ** argv)
{
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

    struct sockaddr_in si_other;
    int s, i;
    socklen_t slen = sizeof(si_other);
    char *buf = new char[buffersize];
    char message[buffersize];

    /* Create Socket */
    s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(destinationPort);
     
    if (inet_aton(destinationIp , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 
    while(1)
    {
        cout << "Enter message : ";
        cin >> message;
         
        //send the message
        sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen);
         
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        for(int i = 0; i < buffersize; i++){
            buf[i] = '\0';
        }
        //try to receive some data, this is a blocking call
        recvfrom(s, buf, buffersize, 0, (struct sockaddr *) &si_other, &slen);
         
        puts(buf);
    }
 
    //TODO: klo kirim asci spasi blm bis
    return 0;
}