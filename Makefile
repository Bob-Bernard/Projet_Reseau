CFLAGS = -lpthread -Wall
LDFLAGS=
LIB_FOLDER=libs
CC=g++
EXEC= server

all: $(EXEC) 

server: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o serverounet.o
	$(CC) -o $@ $^ $(CFLAGS)
	
#%.o: %.cc $(CC) -c $< $(LDFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
