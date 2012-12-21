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

#define ADD_LINES 1
#define FINISH_REPORT 2

struct Client {
  char message[BUFFER_SIZE];
	int des_Client;
	char* name;
	char* password;
	bool controller;
	bool claimed_report;
	bool recieved_report;
};
typedef Client* P_Client;

struct data {
  int descripteur;
  P_Client* ptr_client_list;
  int* nb_client;
};
typedef data* P_data;

/**
* Gère l'envoi du rapport PDF à l'employé
**/
void telechargement_pdf(Client* employe)
{
	
	
	
	  
}

/**
* Retourne vrai si l'employé donné en argument doit envoyer un rapport
* @param : employé
**/
bool verification_demande_rapport(Client* employe)
{
  return employe->claimed_report;
}

/**
* Réception d'un rapport que doit envoyer un employé
**/
void employee_report_to_pdf(Client* employee)
{
  cout << "Début saisie rapport par le client "<< employee->name << endl;
  int continu(1),reception(0);
  char request;

  while(continu)
  {
	  reception = read(employee->des_Client,employee->message,sizeof(employee->message));
	  if(!reception)
		  perror("Erreur réception read()");
	  else {
	    request = employee->message[0];
	    cout <<"Employé : "<<  employee->name <<" demande reçue "<< request<<endl;
	                        
	    switch((int)request) 
	    {		   
		    case ADD_LINES :  cout << "Demande d'ajout de ligne" << endl; break;
		    case FINISH_REPORT : cout << "Demande de finalisation" << endl; break;		
		    default : perror("Erreur switch reportToPDF");
	    }	
	    
// 	    TODO	
    }
  } 
  
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
void * th_employee_management(void* param) 
{
  Client* employe = (Client*)param;
  int reponse = 1;
  
  cout << "Bonjour "<< employe->name <<" !"<< endl;
  while(reponse == 0)
  {
    cout << "Que voulez vous faire ?" << endl;
    cout << "1 : Saisir un rapport."<< endl;
    cout << "2 : Télécharger un rapport déjà saisi."<< endl;
    cout << "3 : Quitter" << endl;    
    cout << "Tappez le chiffre correspondant à votre demande";
    
    cin >> reponse;
    switch(reponse) {
      case 1 : employee_report_to_pdf(employe); break;
      case 2 : telechargement_pdf(employe); break;
      case 3 : reponse = 0; break;
      default : cout << "Erreur de saisie, veuillez recommencer" << endl;
    }
  }  
pthread_exit(NULL);
}


/**
*
**/
void * th_verif_presence_rapport(void* param) 
{

	pthread_exit(NULL);
}


int isClient(Client client, P_Client listeClientsEntreprise[])
{
	int status(0),indice(0);
	
	while (listeClientsEntreprise[indice] != NULL)
	{
		if(listeClientsEntreprise[indice]->name == client.name) 
		{
			if(listeClientsEntreprise[indice]->controller == true) {
				status = CONTROLEUR_OK;
			}
			else if(verification_demande_rapport(listeClientsEntreprise[indice]) )	{
					status = CLIENT_OK_DISPO;	
			}
			else {
				status = CLIENT_OK;
		  }
		  listeClientsEntreprise[indice]->des_Client = client.des_Client;
		}
		indice++;
		
	} // end loop
return status;
}

int authentification (int desClient,int nb_client, P_Client listeClientsEntreprise[])
{
	int authentifie(0),reception(0),statusClient(0);
	Client tempCli;

	reception = read(desClient,tempCli.message,sizeof(tempCli.message));
	if(!reception)
		perror("Erreur réception read()");
	else {
		cout <<"Login reçu : " << tempCli.message << endl;
		sprintf(tempCli.name,tempCli.message);
		tempCli.des_Client = desClient;
		statusClient = isClient(tempCli,listeClientsEntreprise);
	  
	  if(statusClient == CLIENT_REFUSE) {
	    authentifie = CLIENT_REFUSE;
	  }
	  else {                        
		  switch(statusClient) 
		  {		   
			  case CLIENT_OK : authentifie = CLIENT_OK; break;
			  case CLIENT_OK_DISPO : authentifie = CLIENT_OK_DISPO; break;
			  case CONTROLEUR_OK : authentifie = CONTROLEUR_OK; break;		
			  default : perror("Erreur switch statusClient");
		  }		
	  } // End if (statusClient == ...)
	} // End if(reception > 0)
	
return authentifie;
}



/**
* Thread lancé dès qu'un client a été accept() par le server. Il va gérer la
* la partie authentification puis oriente le client.
**/
void* th_new_client(void* param)
{
  P_data data = (P_data) param;
  int statusClient = 0;
  
  statusClient = authentification(data->descripteur,*data->nb_client,
  data->ptr_client_list);
  if(statusClient == 0) {
    close(data->descripteur);
  }
  else 
  {
    pthread_t idThread;			  		  
    switch(statusClient) 
    {
      case CLIENT_OK : cout << "C'est un client !"<< endl; break;			    			    
      case CLIENT_OK_DISPO : cout << "JC'est un client, rapport dispo !"<< endl; 
        if(pthread_create(&idThread,NULL,th_employee_management,
        (void*)data->ptr_client_list[*data->nb_client])!= 0)  {
          perror("Erreur création thread");
        }
        break;			      
      case CONTROLEUR_OK : cout << "C'est un controleur !"<< endl; 
      
        break;
      default : perror("Petit souci switch(statusClient)"); 
                exit(EXIT_FAILURE);
    }

    data->nb_client++; // gestion d'accès concurrent ?
  }
  
pthread_exit(NULL);
}



int main(int args,char* argv[]) {
	int port, desCurrentClient, DesServer, localBR, nb_client(0);
	struct sockaddr_in brCv;
	socklen_t sizeLocalBr;
	P_Client listeClientsEntreprise[50];
	P_data data; // création d'un pointeur sur une struct "data"
	
  data->nb_client= &nb_client;
  data->ptr_client_list = listeClientsEntreprise;
		
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
		perror("Erreur Sock()");
		exit(EXIT_FAILURE);
	}
	
	if(listen(DesServer,5) == -1)
	perror("Erreur listen()");

	while(1)
	{
/* Acceptation de la connexion du Client */
		desCurrentClient = accept(DesServer,(struct sockaddr *)&brCv,&sizeLocalBr);
		if(desCurrentClient == -1)
			perror("Erreur accept ");
		else {
		  pthread_t idThread; 
			cout << "Nouveau Client accepté" << endl;
			data->descripteur = desCurrentClient;
			
			if(pthread_create(&idThread,NULL,th_new_client,(void*)data) != 0) {
        perror("Erreur création th_new_client");
      }
      
		}// End if Client accepté
	} // End loop
	
/* Fermeture socket server */
	close(DesServer);

return EXIT_SUCCESS;
}
