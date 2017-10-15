#include <iostream>
#include <string.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;
 
char ByteToChar(string s){
    int num=0,bit;
    for (int i = 0; i < 8; i++){
      bit = (s[i] == '0') ? 0 : 1;
      num+= bit*pow(2,8-1-i);
    }
    return char(num);
}

void writeFile(vector<string> v, char* filename){
    std::filebuf fb;
    fb.open (filename,std::ios::out);
    std::ostream os(&fb);
    for (int i = 0; i < v.size(); i++){
        os << ByteToChar(v[i]);
    }
    fb.close();
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
    vector<string> v;

    //keep listening for data
    while(1) {
        cout << "Waiting for data..." << endl;
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        recv_len = recvfrom(receiveSocket, buf, buffersize, 0, (struct sockaddr *) &si_other, &slen);
         
        //print details of the client/peer and the data received
        cout << "Received packet from " << inet_ntoa(si_other.sin_addr) << ":" << ntohs(si_other.sin_port) << endl;
        cout << "Data: " << buf << endl;

        //save the message if save
        if(strcmp(buf, "00000000\0") != 0){
            string message = buf;
            v.push_back(message);
        } else {
            break;
        }
        
        //now reply the client with the same data
        sendto(receiveSocket, buf, recv_len, 0, (struct sockaddr*) &si_other, slen);  
    } 

    writeFile(v, filename);
 
    return 0;
}
