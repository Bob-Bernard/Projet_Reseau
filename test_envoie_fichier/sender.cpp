#include "libs/sock.h"
#include "libs/sockdist.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>


int main(int args,char* argv[]) {
	int port, desCurrentClient, DesServer, localBR, nb_client(0);
	struct sockaddr_in brCv;
	socklen_t sizeLocalBr;
	
		
	if(args == 2) {
		cout << "N° port saisi : "<< argv[1] << endl;
		port = atoi(argv[1]);
	}
	else {
		cout << "Numéro du port du server : ";	
		cin >> port;
	}
	
	/* Création de la BR Publique */
	Sock* server = new Sock(SOCK_STREAM,(short)htons(port),0);
	if(server->good()) {
		cout << "Server lancé !" << endl;		
		DesServer = server->getsDesc();
		localBR = server-> getsRetour();
		sizeLocalBr = sizeof(brCv);
	}
	else {
		perror("Erreur employee->des_client()");
		exit(EXIT_FAILURE);
	}
	
	if(listen(DesServer,5) == -1)
	perror("Erreur listen()");

	desCurrentClient = accept(DesServer,(struct sockaddr *)&brCv,&sizeLocalBr);
	if(desCurrentClient == -1)
		perror("Erreur accept ");

	cout << "test" << endl;
	int taille(0);
	const char* pdf_path = "sujet.pdf";
	FILE* pdf_file = fopen(pdf_path, "rb");
	
	char file_content[1024];

	if (pdf_file != NULL)
	{
		cout << "Fichier ouvert !" << endl;
		fseek(pdf_file,0,SEEK_END);
		taille = ftell(pdf_file);
		fseek(pdf_file,0,SEEK_SET);
		
		cout << "Taille : " << taille << endl;
		send(desCurrentClient,&taille, sizeof(int), 0);
		
		while (fgets(file_content, sizeof (file_content),pdf_file) != NULL)
		{
    	send(desCurrentClient, file_content, sizeof (file_content),0);
    }
    
    fclose (pdf_file);    
  }
  else
  {
  	cout << "Rapport PDF inconnu" << endl;
	}
	
	close(desCurrentClient);
	close(DesServer);
return 1;
}
