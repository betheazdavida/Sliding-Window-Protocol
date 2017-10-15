#include <iostream>
#include <string.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
 

string convertToByte(char c){
    int oct = int(c);
    char s[8];
    vector<int> v;
    while(oct){
      v.push_back(oct%2);
      oct/=2;
    }
    string ret="";
    int diff = 8-v.size();
  
    for (int i=0; i<diff; i++){
      ret+="0";
    }
    for (int i = diff; i < 8; i++){
      ret+= (v[v.size()-1-i+diff] == 1)? '1' : '0';
    }
  
    return ret;
}
  
vector<string> readFile(char* filename){
    static vector<string> v;

    filebuf fb;
    if (fb.open (filename,ios::in))
    {
        istream is(&fb);
        while (is){
        string ss = convertToByte(char(is.get()));
        v.push_back(ss);
        }
        fb.close();
    }
    else {
        cout << "Unable to open file";
    }

    for(int i = 0; i < v.size(); i++){
        cout << i << ":" << v[i] << endl;
    }

    return v;

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
 
    //Vector to be sent
    vector<string> v = readFile(filename);
    v.pop_back(); v.pop_back();
    char msg[8];
    
    for(int i = 0; i < v.size(); i++){
        char msg[8];
        
        //cout << "counter " << i << endl;
        for(int j = 0; j < 8; j++){
            msg[j] = v[i][j]; 
        } 
        msg[8] = '\0';
        //cout << "isi vector " << v[i] << endl; 
        //cout << "isi msg " << msg << endl; 

        //send the message
        sendto(sendSocket, msg, strlen(msg), 0 , (struct sockaddr *) &si_other, slen);
         
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
    sendto(sendSocket, "00000000\0", 8, 0 , (struct sockaddr *) &si_other, slen);
 
    return 0;
}