#include "libs/sock.h"
#include "libs/sockdist.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

int main (int args, char* argv[] ) {
	
	char hote[12];
	int port;	
	if(args == 1)
	{
		cout << "Saisir le nom ou l'adresse du server : ";
		cin >> hote;
		cout << "Numéro du port : ";	
		cin >> port;
	}
	else {
		port = atoi(argv[2]);
		strcpy(hote,argv[1]);
	}	
	
	
	//===================Description BR local =============================================
	Sock* client = new Sock(SOCK_STREAM,31469,0);
	int DesClient = client->getsDesc();
	int BRLocal = client-> getsRetour();
	socklen_t* lgLoc;

	SockDist* server = new SockDist(hote,(short)htons(port));
	struct sockaddr_in *BRDist;
	BRDist = server ->getAdrDist(); // Récupération de sockaddr_in 
	int lgbrSrv = server->getsLen();

	
	//=================================Protocole envoie==============================================

if(connect(DesClient,(struct sockaddr *)BRDist,lgbrSrv) == -1) {
		perror("Erreur de connexion");
		exit(EXIT_SUCCESS);
	}
	else {
		cout << "Connexion acceptée" << endl;
	}


		int file_size(0),read(0),received_bytes(0);
		char file_content[1024];
		FILE* pdf_file = fopen("monRapport.pdf", "wb");
		
		if(pdf_file != NULL) 
		{
		  recv(DesClient,&file_size,sizeof(int),0);
		  if(file_size == 0) { 
		   perror("Erreur reception taille fichier"); 
		  }
		
		  while(received_bytes < file_size)
		  {	
			  read = recv(DesClient,file_content,sizeof(file_content),0);
			  fwrite(file_content,sizeof(char),read,pdf_file);
			  received_bytes += read;
		  }
		  cout << "Rapport reçu !" << endl;
		  fclose(pdf_file);	
		}
		else { 
		  cout << "Fichier PDF inconnu" << endl;
		}

close(DesClient);


return 1;
}
