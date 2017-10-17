#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <fstream>
#include <string>
#include <unistd.h>

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

    //Vector yang terisi seluruh data yang akan dikirim
    vector<char> v = readToByte(filename);
    //Buffer yang direpresentasikan dalam pointer to char
    char buffer[buffersize];
    int currentSeqnum = 0;
    while(1){
        //send data of sliding window size
        int remainder = (v.size()-1 - currentSeqnum) < windowSize ? v.size() - currentSeqnum : windowSize;
        cout << "sisa : " << remainder << endl;
        for(int i = 0; i < remainder; i++){
            //Framing packet
            PACKET p(i+currentSeqnum,v[i + currentSeqnum],50);
            int packet_len = sizeof(p);
            char res[packet_len];

            memcpy(res,&p,packet_len);
            
            for (int j = 0; j < packet_len; j++) buf[j]=res[j];
            buf[packet_len]='\0';

            //insert the data to buffer
            buffer[(currentSeqnum)%256+i] = p.getData();
            cout << "iterator di buffer : " << currentSeqnum+i << endl;
            cout << "isi buffer sesudah: "<< buffer << endl;
            //send the message
            sendto(sendSocket, buf, packet_len, 0 , (struct sockaddr *) &si_other, slen);
        }
        //sleep(1);
        //wait for ack
        for(int i = 0; i < remainder; i++){
            //try to receive some data, this is a blocking call
            recvfrom(sendSocket, buf, sizeof(buf), 0, (struct sockaddr *) &si_other, &slen);
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
    PACKET p(-1,'\0',0); //headernya nol
    int packet_len = sizeof(p);
    char res[packet_len];

    memcpy(res,&p,packet_len);
    
    for (int j = 0; j < packet_len; j++) buf[j]=res[j];
    buf[packet_len]='\0';
    sendto(sendSocket, buf, packet_len, 0 , (struct sockaddr *) &si_other, slen);





    // for(int i = 0; i < v.size(); i++){
    //     //Framing packet
    //     PACKET p(i,v[i],50);
    //     int packet_len = sizeof(p);
    //     char res[packet_len];

    //     memcpy(res,&p,packet_len);

    //     for (int j = 0; j < packet_len; j++)
    //       buf[j]=res[j];
    //     buf[packet_len]='\0';

    //     //send the message
    //     sendto(sendSocket, buf, packet_len, 0 , (struct sockaddr *) &si_other, slen);
    //     //cout << " BUF LEN : " <<strlen(buf) <<"\n";
    //     //receive a reply and print it
    //     //clear the buffer
    //     for(int k = 0; k < buffersize; k++){
    //         buf[k] = '\0';
    //     }

    //     //try to receive some data, this is a blocking call
    //     recvfrom(sendSocket, buf, buffersize, 0, (struct sockaddr *) &si_other, &slen);
    //     ACK* a = (ACK * )buf;
    //     //a -> printACK();

    //     cout << "feedback" << buf << endl;
    // }

    // //send terminate connection
    // char msg[1];
    // msg[0] = 4;
    // msg[1] = '\0';
    // sendto(sendSocket, msg, 0, 0 , (struct sockaddr *) &si_other, slen);

    return 0;
}
