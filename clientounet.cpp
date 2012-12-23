#include "libs/sockdist.h"
#include "libs/sock.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

struct Client {
  char message[BUFFER_SIZE];
	int des_client;
	char* name;
	char* password;
	bool controller;
	bool claimed_report;
	bool received_report;
};

char authentification ()
{
	char * mdp;
	cout<<"taper votre mot de passe:"<<endl;
	cin>>mdp;
	cout<<mdp<<endl;
	return *mdp;
}




int main (int args, char* argv[] ) {
	
	char hote[12];
	int port,DesClient,BRLocal,lgbrSrv,connected(0);
	socklen_t* lgLoc;
	struct sockaddr_in *BRDist;
  Client* client;
  p
	
	
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
	DesClient = client_socket->getsDesc();
	BRLocal = client_socket-> getsRetour();
	SockDist* server = new SockDist(hote,(short)htons(port));
	BRDist = server ->getAdrDist(); // Récupération de sockaddr_in 
	lgbrSrv = server->getsLen();	
	
	//=================================Protocole envoie==============================================

  cout << "Connexion au server en cours..." << endl;
  if(connected = connect(DesClient,(struct sockaddr *)BRDist,lgbrSrv) == -1) {
    perror("Erreur de connexion");
  }
  else {
    cout << "Connecté !" << endl;
    
	}
		


	close(DesClient);

return EXIT_SUCCESS;
}
