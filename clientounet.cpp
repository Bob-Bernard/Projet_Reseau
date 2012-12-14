#include "libs/sockdist.h"
#include "libs/sock.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFSIZE 1024

typedef struct {
	int des_client;
	char * mdp;
}employe;

	


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
	//char test=authentification();
	//cout<<test<<endl;
		
	char TamponExp[BUFSIZE]="";
	char TamponRec[BUFSIZE]="";
	
	
	//=================================Protocole envoie==============================================

if(connect(DesClient,(struct sockaddr *)BRDist,lgbrSrv) == -1)
	{
		perror("Erreur de connexion");
		exit(EXIT_SUCCESS);
	}
	else {
		cout << "Connexion acceptée" << endl;
	}
		
	while(1)
	{	
		cout << "Veuillez saisir un message : ";
		cin.getline(TamponExp,sizeof(TamponExp));

/*Envoie d'un message au server */
		if(write(DesClient,TamponExp,sizeof(TamponExp))==-1)
		{
			perror("Erreur d'envoie");
		}
		else {
			cout << "Message envoyé ! "<< endl << "Attente d'une réponse du server..." << endl;
		}
			

/* Reception message du server */
		if(read(DesClient,TamponRec,sizeof(TamponRec))==-1)
		{
			perror("Erreur réception");
		}
		else {
			strcat(TamponRec,"\0");
			cout << "Réponse server : " << TamponRec << endl << endl;			
		}
	} // fin while

	close(DesClient);

}
