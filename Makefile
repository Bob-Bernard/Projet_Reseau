CFLAGS = -lpthread -Wall
LDFLAGS=
CC=g++
EXEC= server

all: $(EXEC) 

server: sockdist.o sock.o serverounet.o
	$(CC) -o $@ $^ $(CFLAGS)
	
#%.o: %.cc $(CC) -c $< $(LDFLAGS)
m
.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
