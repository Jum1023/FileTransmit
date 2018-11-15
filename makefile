network : libconfiguration.so libFileTransmit.so libInstantMessage.so main.a
	g++ -o ./bin/network ./bin/libmain.a -lboost_filesystem -lboost_system -L./bin -lconfiguration -lFileTransmit -lInstantMessage -rpath @executable_path

main.a : ./network/utilities.cpp ./network/main.cpp
	g++ -c -fPIC ./network/utilities.cpp -o ./bin/utilities.o
	g++ -c -fPIC ./network/main.cpp -o ./bin/main.o
	ar -rc ./bin/libmain.a ./bin/main.o ./bin/utilities.o

libconfiguration.so : ./configuration/config.cpp ./configuration/config.h
	g++ -c -fPIC ./configuration/config.cpp  -o ./bin/config.o
	g++ -shared ./bin/config.o -o ./bin/libconfiguration.so -install_name @rpath/libconfiguration.so

libFileTransmit.so : ./FileTransmit/transmit.cpp ./FileTransmit/transmit.h
	g++ -c -fPIC ./FileTransmit/transmit.cpp -o ./bin/transmit.o
	g++ -shared ./bin/transmit.o -o ./bin/libFileTransmit.so -lboost_filesystem -lboost_system -install_name @rpath/libFileTransmit.so

libInstantMessage.so : ./InstantMessage/chat.cpp ./InstantMessage/chat.h
	g++ -c -fPIC ./InstantMessage/chat.cpp -o ./bin/chat.o
	g++ -shared ./bin/chat.o -o ./bin/libInstantMessage.so -lboost_filesystem -lboost_system -install_name @rpath/libInstantMessage.so

clean :
	rm ./bin/*.o


#	clang
#	otool to check the executable file
#	-install_name so_file_name to specify the shared library name
#   -rpath @executable_path to find the shared library under the execute path not only the command line current working directory
#	-rpath @loader_path this does the same with the above argument

#	gcc
#	ldd to find the needed shared library
#	readelf to check the executable file
#	-Wl,-rpath='$ORIGIN' to find the shared library under the execute path not only the command line current working directory