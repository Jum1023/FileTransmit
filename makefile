network : libconfiguration.so libFileTransmit.so libInstantMessage.so ./network/utilities.cpp ./network/main.cpp
	g++ -o ./bin/network ./network/utilities.cpp ./network/main.cpp -L. -L./bin -lconfiguration -lFileTransmit -lInstantMessage -lboost_filesystem -lboost_system

libconfiguration.so : ./configuration/config.cpp ./configuration/config.h
	g++ -c -fPIC ./configuration/config.cpp  -o ./bin/config.o
	g++ -shared ./bin/config.o -o ./bin/libconfiguration.so

libFileTransmit.so : ./FileTransmit/transmit.cpp ./FileTransmit/transmit.h
	g++ -c -fPIC ./FileTransmit/transmit.cpp -o ./bin/transmit.o
	g++ -shared ./bin/transmit.o -o ./bin/libFileTransmit.so -lboost_filesystem -lboost_system

libInstantMessage.so : ./InstantMessage/chat.cpp ./InstantMessage/chat.h
	g++ -c -fPIC ./InstantMessage/chat.cpp -o ./bin/chat.o
	g++ -shared ./bin/chat.o -o ./bin/libInstantMessage.so -lboost_filesystem -lboost_system

clean :
	rm ./bin/*.o
