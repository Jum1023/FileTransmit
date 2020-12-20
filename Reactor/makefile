MAKEFILE      = Makefile

EQ            = =

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -Dlinux
CFLAGS        = -pipe -std=c99 -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -std=c++11 -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I.
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
LINK          = gcc
LFLAGS        = -Wl,--no-undefined -Wl,-rpath,'$$ORIGIN'
LIBS          = $(SUBLIBS) -L. -lpthread
AR            = ar cqs
RANLIB        =
SED           = sed
STRIP         = strip

####### Output directory

OBJECTS_DIR   = .

####### Files

SOURCES       = reactor_run.c \
				reactor.c \
				createsocket.c \
				callback.c \

OBJECTS       = reactor_run.o \
				reactor.o \
				createsocket.o \
				callback.o \

HEADERS       = reactor_run.h \
				reactor.h \
				createsocket.h \
				callback.h \

TARGET        = reactor

####### Build rules

reactor:  $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS)

reactor_run.o: reactor_run.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o reactor_run.o reactor_run.c

reactor.o: reactor.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o reactor.o reactor.c

createsocket.o: createsocket.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o createsocket.o createsocket.c

callback.o: callback.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o callback.o callback.c