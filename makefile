network : libconfiguration.so libFileTransmit.so libInstantMessage.so ./network/utilities.cpp ./network/main.cpp
	g++ -o ./bin/network ./network/utilities.cpp ./network/main.cpp -L./bin -lconfiguration -lFileTransmit -lInstantMessage

libconfiguration.so : ./configuration/config.cpp ./configuration/config.h
	g++ -c -fPIC ./configuration/config.cpp  -o ./bin/config.o
	g++ -shared ./bin/config.o ./bin/libconfiguration.so

libFileTransmit.so : transmit.cpp transmit.h
	g++ -c -fPIC ./FileTransmit/transmit.cpp -o ./bin/transmit.o
	g++ -shared ./bin/transmit.o ./bin/libtransmit.so

libInstantMessage.so : chat.cpp chat.h
	g++ -c -fPIC ./InstantMessage/chat.cpp -o ./bin/chat.o
	g++ -shared ./bin/chat.o ./bin/libInstantMessage.so

clean :
	rm ./bin/*.o