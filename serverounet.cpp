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

struct Client{
  char message[BUFFER_SIZE];
	int des_Client;
	char* name;
	char* password;
	bool controller;
	bool rapport;
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
bool Verification_demande_rapport(Client* employe)
{
  return employe->rapport;
}

/**
* Reception d'un rapport d'un employé
**/
void reception_rapport_employe(Client* employe)
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
  Client* client = (Client*)param;
  cout << "Début de saisie d'un rapport de l'employe : "<< client->name << endl;
  
  reception_rapport_employe(client);
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


int isClient(char* client_name, P_Client listeClientsEntreprise[])
{
	int status;
	int i=0;
	
	while (listeClientsEntreprise[i] != NULL)
	{
		if(listeClientsEntreprise[i]->name == client_name) {		
			if(listeClientsEntreprise[i]->controller == true) {
				status = CONTROLEUR_OK;
			}
			else if(Verification_demande_rapport(listeClientsEntreprise[i]) )	{
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

int authentification (int desClient,int nb_Client, P_Client listeClientsEntreprise[])
{
	int authentifie(0),reception(0),statusClient(0);
	Client currentCli;

	reception = read(desClient,currentCli.message,sizeof(currentCli.message));
	if(!reception)
		perror("Erreur réception read()");
	else {
		cout <<"Login reçu : " << currentCli.message << endl;	
		statusClient = isClient(currentCli.message,listeClientsEntreprise);
	  
	  if(statusClient == CLIENT_REFUSE) {
	    authentifie = CLIENT_REFUSE;
	  }
	  else {
		  currentCli.des_Client= desClient;
      currentCli.name = currentCli.message;
		  currentCli.controller = false;
		  		                          
		  switch(statusClient) 
		  {		   
			  case CLIENT_OK : authentifie = CLIENT_OK; break;
			  case CLIENT_OK_DISPO : authentifie = CLIENT_OK_DISPO; break;
			  case CONTROLEUR_OK : authentifie = CONTROLEUR_OK;  
			                       currentCli.controller = true; 
			                       break;		
			  default : perror("Erreur switch statusClient");
		  }
		  listeClientsEntreprise[nb_Client] =  &currentCli;			
	  }
	} // End if(reception > 0)
return authentifie;
}


int main(int args,char* argv[]) {
	int port, desBrClient, DesServer, BRLocal, statusClient;
	int nb_Client = 0;
	struct sockaddr_in brCv;
	socklen_t lgLoc;
	P_Client listeClientsEntreprise[50];
	
	/* Petit test ici */
  Client clicli;
  clicli.name ="tintin";
  listeClientsEntreprise[0] = &clicli;	
	
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
		
	while(1)
	{
	
/* Acceptation de la connexion du Client */
		desBrClient = accept(DesServer,(struct sockaddr *)&brCv,&lgLoc);
		if(desBrClient == -1)
			perror("Erreur accept ");
		else {
			cout << "Nouveau Client accepté" << endl;	
			statusClient = authentification(desBrClient,nb_Client,listeClientsEntreprise);
			if(statusClient == 0) {
				close(desBrClient);
			}
			else 
			{
 			  pthread_t idThread;			  		  
			  switch(statusClient) 
			  {
			    // Juste une trace, mais dans ce cas ci, le client sera déconnecté.
			    case CLIENT_OK : cout << "Je suis client !"<< endl;break;
			    			    
			    case CLIENT_OK_DISPO : cout << "Je suis client, rapport dispo !"<< endl; 
			      if(pthread_create(&idThread,NULL,th_Gestion_Rapport_PDF,listeClientsEntreprise[nb_Client])!= 0){
			        perror("Erreur création thread");
			      }
			      break;			      
			    case CONTROLEUR_OK : cout << "Je suis controleur !"<< endl; break;
			    default : perror("Petit souci switch(statusClient)"); 
			              exit(EXIT_FAILURE);
			              break;
			  } // End switch
			  
			  nb_Client++; 	
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
