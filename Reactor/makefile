createsocket.o : createsocket.h createsocket.cpp
	gcc -c -o createsocket.o createsocket.cpp

callback.o : callback.h callback.cpp
	gcc -c -o callback.o callback.cpp

reactor.o : reactor.h reactor.cpp
	gcc -c -o reactor.o reactor.cpp

reactor : reactor_run.cpp
	gcc -o reactor reactor_run.cpp createsocket.o callback.o reactor.o