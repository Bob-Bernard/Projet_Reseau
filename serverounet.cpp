#include "libs/sockdist.h"
#include "libs/sock.h"

#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

typedef struct {
}employe;

typedef struct {
}controleur;


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
bool Verification_demande_rapport(employe emp)
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




int main(int args,char* argv[]) {
	int port, descripteurBR, DesServer, BRLocal;
	int nb_client = 0;
	struct sockaddr_in brCv;
	socklen_t lgLoc;
	
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
/* Acceptation de la connexion du client */
		descripteurBR = accept(DesServer,(struct sockaddr *)&brCv,&lgLoc);
		if(descripteurBR == -1)
			perror("Erreur accept ");	
		else {
				cout << "Nouveau client accepté !" << endl;
				pthread_t idThread;
				nb_client++;
		}
		
		
		
		
		
		
		
		
	} // End loop
	
	/* Attente de tout les threads créés*/
//	for(int i=0; i<nb_client; i++)
//  {
//    pthread_join(ListeIdThread[i],NULL);
//  }
  
	/* Fermeture socket server */
	close(DesServer);

return EXIT_SUCCESS;
}
