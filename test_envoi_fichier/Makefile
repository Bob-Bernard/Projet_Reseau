CFLAGS = -lpthread -Wall
FLAGS=`pkg-config --cflags --libs gtk+-2.0`
LIB_FOLDER=libs
CC=g++
CC2=gcc
EXEC= sender receiver sauvegarde

all: $(EXEC) 

sender: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o sender.o
	$(CC) -o exec/$@ $^ $(CFLAGS)
 
receiver: $(LIB_FOLDER)/sockdist.o $(LIB_FOLDER)/sock.o receiver.o
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
