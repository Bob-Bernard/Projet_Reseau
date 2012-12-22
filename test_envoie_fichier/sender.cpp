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
		perror("   accept ");

	cout << "test" << endl;
	int taille(0);
	const char* pdf_path = "sujet.pdf";
	FILE* pdf_file = fopen("sujet.pdf", "rb");
	int sent = 0;
	int read = -1;
	char file_content[1024];

	if (pdf_file != NULL)
	{
		cout << "Fichier ouvert !" << endl;
		fseek(pdf_file,0,SEEK_END);
		taille = ftell(pdf_file);
		rewind(pdf_file);
		
		cout << "Taille : " << taille << endl;
		send(desCurrentClient,&taille,sizeof(int),0);
		  
		while(read != 0)
		{
		  read = fread(file_content,sizeof(char),1024,pdf_file);		  
    	send(desCurrentClient, file_content,read,0);
    	cout << "Envoyé : " << read << endl;
    }
    
    cout << "Fichier envoyé !" << endl;
    if(fclose (pdf_file) != 0) {
      perror("Erreur fclose()");
    }
    else { cout << "fichier fermé" << endl; }
  }
  else
  {
  	cout << "Rapport PDF inconnu" << endl;
	}
	
	cout << "Close() " << endl;
	
	close(desCurrentClient);
	close(DesServer);
return 1;
}
