#include <iostream>
#include <string.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;
 
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
    char message[buffersize];

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
 
    while(1)
    {
        cout << "Enter message : ";
        cin >> message;
         
        //send the message
        sendto(sendSocket, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen);
         
        //receive a reply and print it
        //clear the buffer
        for(int i = 0; i < buffersize; i++){
            buf[i] = '\0';
        }

        //try to receive some data, this is a blocking call
        recvfrom(sendSocket, buf, buffersize, 0, (struct sockaddr *) &si_other, &slen);
         
        puts(buf);
    }
 
    return 0;
}