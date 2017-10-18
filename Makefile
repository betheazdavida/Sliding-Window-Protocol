allmake: recvfile sendfile

recvfile: src/recvfile.cpp src/packet.cpp src/ack.cpp
	g++ -o recvfile.exe src/recvfile.cpp src/packet.cpp src/ack.cpp

sendfile: src/sendfile.cpp src/packet.cpp src/ack.cpp
	g++ -o sendfile.exe src/sendfile.cpp src/packet.cpp src/ack.cpp