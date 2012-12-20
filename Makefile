CFLAGS = -lpthread -Wall
FLAGS=`pkg-config --cflags --libs gtk+-2.0`
LIB_FOLDER=libs
CC=g++
CC2=gcc
EXEC= server client sauvegarde

all: $(EXEC) 

server: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o serverounet.o
	$(CC) -o exec/$@ $^ $(CFLAGS)
 
client: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o clientounet.o
	$(CC) -o exec/$@ $^ $(CFLAGS)

sauvegarde: 
	gcc $(FLAGS) $(LIB_FOLDER)/sauvegarde.c -o exec/$@
  
		
#%.o: %.cc $(CC) -c $< $(LDFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o
	rm -rf $(LIB_FOLDER)/*.o

mrproper: clean
	rm -rf exec/*
