#include "libs/sockdist.h"
#include "libs/sock.h"

#include <string>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

#define CLIENT_REFUSE 0
#define CLIENT_OK 1
#define CLIENT_OK_DISPO 2
#define CONTROLEUR_OK 3

struct Client {
  char message[BUFFER_SIZE];
	int des_client;
	bool controller;
	bool claimed_report;
	bool received_report;
};
typedef Client* client_t;

/**
* Gère l'envoi du rapport PDF à l'employé
**/
bool download_PDF(Client* client)
{
  int file_size(0),read(0),received_bytes(0);
	char file_content[BUFFER_SIZE];
	bool file_received = false;
	FILE* pdf_file = fopen("monRapport.pdf", "wb");
	
	if(pdf_file != NULL) 
	{
	  recv(client->des_client,&file_size,sizeof(int),0);
	  if(file_size == 0) { 
	   perror("Erreur reception taille fichier"); 
	  }
	
	  while(received_bytes < file_size)
	  {	
		  read = recv(client->des_client,file_content,sizeof(file_content),0);
		  fwrite(file_content,sizeof(char),read,pdf_file);
		  received_bytes += read;
	  }
	  cout << "Rapport reçu !" << endl;
	  file_received = true;
	  fclose(pdf_file);	
	}
	else { 
	  cout << "Fichier PDF inconnu" << endl;
	}		

return file_received;
}
/*
**
*/
void* th_controller()
{


pthread_exit(NULL);
}


/*
**
*/
void* th_employee(void * param)
{
  client_t client = (client_t) param;
  int request(0);
  
  cout << "Que voulez vous faire ?" << endl;
  cout << "1 : Saisir un rapport."<< endl;
  cout << "2 : Télécharger un rapport déjà saisi."<< endl;
  cout << "3 : Quitter" << endl;    
  cout << "Tappez le chiffre correspondant à votre demande : ";
  cin >> request;
  
  cout << "request : " << request <<endl;
  if(send(client->des_client,&request,sizeof(int),0) < 1) {
    perror("Erreur envoi request");
  }


pthread_exit(NULL);
}


/**
* Retourne le status du client après demande d'authentification au server
**/
int authentification (client_t client)
{
  int statusClient(-1);
  char name[50];

	cout<<"Entrez votre nom : ";
	cin >> name;
	
  if(send(client->des_client,name,sizeof(name),0) == -1) {
    perror("Erreur envoi nom client");
  }
  //client->name = name;
  recv(client->des_client,&statusClient,sizeof(int),0);
  
return statusClient;
}


/**
* Déconnecte un client
**/
void disconnection(Client* client)
{
  if(close(client->des_client) == -1) {
    perror("Erreur déconnexion");
  }
  else {
    cout << "Déconnexion terminée." << endl;
  }
}


int main (int args, char* argv[] ) {
	
	char hote[12];
	int port,BRLocal,lgbrSrv,connected(0),statusClient(-1);
	socklen_t* lgLoc;
	struct sockaddr_in *BRDist;
  client_t client = (client_t) malloc(sizeof(client_t));
  pthread_t idThread;
	
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
	Sock* client_socket = new Sock(SOCK_STREAM,31469,0);
	client->des_client = client_socket->getsDesc();
	BRLocal = client_socket-> getsRetour();
	SockDist* server_socket = new SockDist(hote,(short)htons(port));
	BRDist = server_socket->getAdrDist(); // Récupération de sockaddr_in 
	lgbrSrv = server_socket->getsLen();
	
	//=================================Protocole envoie==============================================

  cout << "Connexion au server en cours..." << endl;
  if(connect(client->des_client,(struct sockaddr *)BRDist,lgbrSrv) == -1) {
    perror("Erreur de connexion");
  }
  else {
    cout << "Connecté au server !" << endl;
    statusClient = authentification(client);
    switch(statusClient) 
    {
      case CLIENT_REFUSE : cout << "Identifiants incorrects !" << endl; 
                           disconnection(client);
                           break;
      case CLIENT_OK : cout << "Vous êtes connecté ! ";
                       cout << "Aucun rapport demandé." << endl;
                       cout << "Vous allez être déconnecté" << endl;
                       disconnection(client);
                       break;
      case CLIENT_OK_DISPO : cout << "Vous êtes connecté ! ";
        cout << "Un rapport vous ai demandé"<< endl;
        if(pthread_create(&idThread,NULL,th_employee,(void*)client)!= 0) {
          perror("Erreur creation thread client");
        }
        pthread_join(idThread,NULL);
        break;
      case CONTROLEUR_OK : cout << "Vous êtes connecté en tant que controleur" << endl;
        // lancement fonction controleur
        pthread_join(idThread,NULL);
        break;
      default : cerr << "Switch error after authentification()"<< endl;
                disconnection(client);
    }    
	}
		
return EXIT_SUCCESS;
}
