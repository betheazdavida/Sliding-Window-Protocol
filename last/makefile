all: sendermake recvmake

sendermake: sender.cpp ack.cpp packet.cpp
	g++ -o sendfile sender.cpp ack.cpp packet.cpp

recvmake: receiver.cpp ack.cpp packet.cpp
	g++ -o recvfile receiver.cpp ack.cpp packet.cpp

