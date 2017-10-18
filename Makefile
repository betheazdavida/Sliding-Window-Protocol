allmake: recvfile sendfile

recvfile: src/recvfile.cpp src/packet.cpp src/ack.cpp
	g++ -o recvfile src/recvfile.cpp src/packet.cpp src/ack.cpp

sendfile: src/sendfile.cpp src/packet.cpp src/ack.cpp
	g++ -o sendfile src/sendfile.cpp src/packet.cpp src/ack.cpp