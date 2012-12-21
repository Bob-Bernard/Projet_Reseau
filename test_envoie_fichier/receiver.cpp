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


		int taille(0),octetrecu(0);
		char file_content[1024];
		FILE* pdf_file = fopen("monRapport.pdf", "wb");
		
		if(pdf_file != NULL) {
		
		recv(DesClient,&taille,sizeof(int),0);
		cout << "taille : " << taille << endl;
		
		while(octetrecu < taille)
		{	
			int lu = recv(DesClient,file_content,sizeof(file_content),0);
			octetrecu += lu;
			fwrite(file_content,lu,sizeof(file_content),pdf_file);
			
			cout << "Octet lu : "<< lu <<  "  Octet recu " << octetrecu << endl;
		}
		
		fclose(pdf_file);
	
		}
		else { cout << "pdf inconnu" << endl;}

close(DesClient);
return 1;
}
