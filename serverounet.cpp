#include "libs/sockdist.h"
#include "libs/sock.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define REFUSE 0
#define CLIENT_OK 1
#define CLIENT_OK_DISPO 2
#define CONTROLEUR_OK 3

struct Client{
  char message[BUFFER_SIZE];
	int des_Client;
	char* name;
	char* password;
	bool controller;
};
typedef Client* P_Client;



/**
* Gère l'envoi du rapport PDF à l'employé
**/
void dl_pdf()
{

}

/**
* Retourne vrai si l'employé donné en argument doit envoyer un rapport
* @param : employé
**/
bool Verification_demande_rapport(Client emp)
{

return true;
}

/**
* Reception d'un rapport d'un employé
**/
void Reception_rapport_employe()
{

}

/**
* Reception de la liste d'employés devant envoyer un rapport
**/
void* th_Recieved_Empl_List(void* param) 
{

pthread_exit(NULL);
}


/**
* Traitement d'une demande d'envoie de rapport d'un employé
**/
void * th_Gestion_Rapport_PDF(void* param) 
{

Reception_rapport_employe();
dl_pdf();


pthread_exit(NULL);
}


/**
*
**/
void * th_Verif_presence_rapport(void* param) 
{

	pthread_exit(NULL);
}


int isClient(P_Client client, P_Client listeClientsEntreprise[])
{
	int status;
	int i=0;
	
	while (listeClientsEntreprise[i] != NULL)
	{
		if(listeClientsEntreprise[i]->name == client->name) {		
			if(client->controller == true) {
				status = CONTROLEUR_OK;
			}
			else if(Verification_demande_rapport)	{
					status = CLIENT_OK_DISPO;	
			}
			else {
				status = CLIENT_OK;
		  }
		}
		i++;
	} // end loop
	

return status;
}

int authentification (P_Client Client,P_Client listeClientsEntreprise[])
{
	int authentifie = 0;
	int reception = 1;
	int statusClient = 0;

	while(!reception)
	{
		reception = read(Client->des_Client,Client->message,sizeof(Client->message));
		if(reception > 0)
			perror("Erreur réception");
		else {
			cout <<"Login reçu : " << Client->message << endl;
			
			statusClient = isClient(Client,listeClientsEntreprise);
			switch(statusClient) {
				case 1 : authentifie = CLIENT_OK; break;
				case 2 : authentifie = CLIENT_OK_DISPO; break;
				case 3 : authentifie = CONTROLEUR_OK; break;		
				default : authentifie = REFUSE; break;
			}
		}
	}
return authentifie;
}


int main(int args,char* argv[]) {
	int port, desBrClient, DesServer, BRLocal;
	int nb_Client = 0;
	struct sockaddr_in brCv;
	socklen_t lgLoc;
	Client* listeClientsEntreprise[50];
//	Client* clicli;
//	clicli->name ="tintin";
//	listeClientsEntreprise[0] = clicli;
	
	
	if(args == 2) {
		cout << "N° port saisit : "<< argv[1] << endl;
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
		BRLocal = server-> getsRetour();
		lgLoc = sizeof(brCv);
	}
	else {
		perror("Erreur Sock()");
		exit(EXIT_FAILURE);
	}
	
/* Création file */
		if(listen(DesServer,5) == -1)
		perror("Erreur listen");		
		
	while (1)
	{
/* Acceptation de la connexion du Client */
		desBrClient = accept(DesServer,(struct sockaddr *)&brCv,&lgLoc);
		if(desBrClient == -1)
			perror("Erreur accept ");
		else {
			cout << "Nouveau Client accepté !" << endl;
			Client* currentCli; 
				currentCli->des_Client = desBrClient;
			  
			listeClientsEntreprise[nb_Client]= currentCli;
			//pthread_t idThread;
			nb_Client++;
		
			if(authentification(currentCli,listeClientsEntreprise) == 0) {
				close(desBrClient);
			}
			else {
		
		
		
		
		
		
			} // End if authentification()
			
		}// End if Client accepté
	} // End loop
	
	/* Attente de tout les threads créés*/
//	for(int i=0; i<nb_Client; i++)
//  {
//    pthread_join(ListeIdThread[i],NULL);
//  }
  
  
	/* Fermeture socket server */
	close(DesServer);

return EXIT_SUCCESS;
}
