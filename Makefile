CFLAGS = -lpthread -Wall
FLAGS=`pkg-config --cflags --libs gtk+-2.0`
LIB_FOLDER=libs
CC=g++
CC2=gcc
EXEC= client server sauvegarde

all: $(EXEC) 

server: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o serverounet.o 
	$(CC) -o exec/$@ $^ $(CFLAGS) $(FLAGS)
 
client: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o clientounet.o 
	$(CC) -o exec/$@ $^ $(CFLAGS)

sauvegarde:
	gcc $(FLAGS) sauvegarde.c -o exec/$@
		
#%.o: %.cc $(CC) -c $< $(LDFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o
	rm -rf $(LIB_FOLDER)/*.o

mrproper: clean
	rm -rf exec/*
