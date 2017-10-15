/*
    Simple udp client
*/
#include <iostream>
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;
 
#define SERVER "127.0.0.1"
#define BUFLEN 256  //Max length of buffer
#define PORT 8000   //The port on which to send data
 
/* arguments: filename windowsize buffersize destination_ip destination_port*/
int main(int argc, char ** argv)
{
    struct sockaddr_in si_other;
    int s, i;
    socklen_t slen = sizeof(si_other);
    //char buf[BUFLEN];
    char *buf = new char[BUFLEN];
    char message[BUFLEN];

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
 
    s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
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
        for(int i = 0; i < BUFLEN; i++){
            buf[i] = '\0';
        }
        //memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
         
        puts(buf);
    }
 
    //TODO: klo kirim asci spasi blm bis
    return 0;
}