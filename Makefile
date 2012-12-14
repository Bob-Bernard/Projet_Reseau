CFLAGS = -lpthread -Wall
LDFLAGS=
LIB_FOLDER=libs
CC=g++
CC2=gcc
EXEC= server client  clean 

all: $(EXEC) 

server: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o serverounet.o
	$(CC) -o $@ $^ $(CFLAGS)
 
client: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o clientounet.o
	$(CC) -o $@ $^ $(CFLAGS)
	

		
#%.o: %.cc $(CC) -c $< $(LDFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o
	rm -rf $(LIB_FOLDER)/*.o

mrproper: clean
	rm -rf $(EXEC)
