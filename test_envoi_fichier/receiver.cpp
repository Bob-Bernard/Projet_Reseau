#include "libs/sock.h"
#include "libs/sockdist.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

void recoit_rapport(int descripteur)
{

	int file_size(0),read(0),received_bytes(0);
	char file_content[1024];
	FILE* pdf_file = fopen("monRapport.pdf", "wb");
	
	if(pdf_file != NULL) 
	{
	  recv(descripteur,&file_size,sizeof(int),0);
	  if(file_size == 0) { 
	   perror("Erreur reception taille fichier"); 
	  }
	
	  while(received_bytes < file_size)
	  {	
		  read = recv(descripteur,file_content,sizeof(file_content),0);
		  fwrite(file_content,sizeof(char),read,pdf_file);
		  received_bytes += read;
	  }
	  cout << "Rapport reçu !" << endl;
	  fclose(pdf_file);	
	}
	else { 
	  cout << "Fichier PDF inconnu" << endl;
	}
	
}



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
  int request(0);
  
  cout << "Que voulez vous faire ?" << endl;
  cout << "1 : Saisir un rapport."<< endl;
  cout << "2 : Télécharger un rapport déjà saisi."<< endl;
  cout << "3 : Quitter" << endl;    
  cout << "Tappez le chiffre correspondant à votre demande : ";
  cin >> request;
  
  if(send(DesClient,&request,sizeof(int),0) < 1) {
    perror("Erreur envoi request");
  }
  
	switch(request)
	{
	case 1 : cout << "Demande saisie rapport reçue" << endl;
	  //write_employee_report(client);
	  break;
	case 2 : cout << "Demande download PDF reçue" << endl;
    recoit_rapport(DesClient);
	  break;
	default : cout << "Demande refusée" << endl;	
	}
	

close(DesClient);


return 1;
}
