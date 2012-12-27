#include "libs/sock.h"
#include "libs/sockdist.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

void envoi_rapport(int descripteur)
{
	int file_size(0),read(-1);
	const char* pdf_path = "sujet.pdf";
	FILE* pdf_file = fopen(pdf_path, "rb");
	char file_content[1024];

	if (pdf_file != NULL)
	{
		cout << "Rapport ouvert !" << endl;
		fseek(pdf_file,0,SEEK_END);
		file_size = ftell(pdf_file);
		rewind(pdf_file);		
		send(descripteur,&file_size,sizeof(int),0);
		  
		while(read != 0)
		{
		  read = fread(file_content,sizeof(char),1024,pdf_file);		  
    	send(descripteur, file_content,read,0);
    }
    
    cout << "Rapport envoyé !" << endl;
    if(fclose (pdf_file) != 0) {
      perror("Erreur fclose()");
    }
  }
  else {
  	perror("Rapport PDF inconnu");
	}
	
}




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

int request(0);

  if(recv(desCurrentClient,&request,sizeof(int),0)==-1)
      perror("Erreur reception employee management");
      
    switch(request) {
      case 1 :  cout << "employee go to report pdf"<<endl;
        //employee_report_to_pdf(desCurrentClient);
        break;
      case 2 :  cout << "employee go to download pdf"<<endl;
        envoi_rapport(desCurrentClient);
        break;
      case 3 : request = 0; 
        cout << "employee quit" << endl;
        break;
      default : cout << "Erreur de saisie, veuillez recommencer" << endl;
    }
  
  	
	close(desCurrentClient);
	close(DesServer);
return 1;
}
