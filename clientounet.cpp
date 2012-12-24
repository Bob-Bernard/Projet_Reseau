#include "libs/sockdist.h"
#include "libs/sock.h"

#include <string.h>
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
	char* name;
	char* password;
	bool controller;
	bool claimed_report;
	bool received_report;
};

/**
* Retourne le status du client après demande d'authentification au server
**/
int authentification (Client* client)
{
  int statusClient;
  char * mdp;
  char* name;
  
	cout<<"taper votre mot de passe:"<<endl;
	cin>>mdp;
	cout<<mdp<<endl;
	return *mdp;
	
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
  Client* client;	
	
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
      case CLIENT_OK_DISPO : cout << "Vous êtes connecté !";
                             cout << "Un rapport vous ai demandé"<< endl;
                             // lancement fonction client
                             break;
      case CONTROLEUR_OK : cout << "Vous êtes connecté en tant que controleur" << endl;
                           // lancement fonction controleur
                           break;
      default : cerr << "Switch error after authentification()"<< endl;
                disconnection(client);
    }
    
	}

return EXIT_SUCCESS;
}
