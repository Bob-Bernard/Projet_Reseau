#include "libs/sock.h"
#include "libs/sockdist.h"

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

#define ADD_LINES 1
#define FINISH_REPORT 2

struct Client {
  char message[BUFFER_SIZE];
	int des_client;
	char name[50];
	char password[30];
	bool controller;
	bool claimed_report;
	bool received_report;
};
typedef Client* P_Client;
typedef Client* client_t;

struct data {
  int descripteur;
  P_Client* ptr_client_list;
  int* nb_client;
};
//typedef data* P_data;
typedef data* data_t;

/**
* Gère l'envoi du rapport PDF à l'employé
**/
bool download_PDF(Client* client)
{
/* COTE RECEVEUR 	
int file_size(0),read(0),received_bytes(0);
		char file_content[BUFFER_SIZE];
		FILE* pdf_file = fopen("monRapport.pdf", "wb");
		
		if(pdf_file != NULL) 
		{
		  recv(DesClient,&file_size,sizeof(int),0);
		  if(file_size == 0) { 
		   perror("Erreur reception taille fichier"); 
		  }
		
		  while(received_bytes < file_size)
		  {	
			  read = recv(DesClient,file_content,sizeof(file_content),0);
			  fwrite(file_content,sizeof(char),read,pdf_file);
			  received_bytes += read;
		  }
		  cout << "Rapport reçu !" << endl;
		  fclose(pdf_file);	
		}
		else { 
		  cout << "Fichier PDF inconnu" << endl;
		}		
*/
	
  int file_size(0),read(-1);
  bool file_sent = false;
	const char* pdf_path = strcat(client->name,".pdf");
	FILE* pdf_file = fopen(pdf_path, "rb");
	char file_content[BUFFER_SIZE];

	if (pdf_file != NULL)
	{
		cout << "Rapport ouvert !" << endl;
		fseek(pdf_file,0,SEEK_END);
		file_size = ftell(pdf_file);
		rewind(pdf_file);		
		send(client->des_client,&file_size,sizeof(int),0);
		  
		while(read != 0)
		{
		  read = fread(file_content,sizeof(char),BUFFER_SIZE,pdf_file);		  
    	send(client->des_client,file_content,read,0);
    }
    
    cout << "Rapport envoyé !" << endl;
    if(fclose (pdf_file) != 0) {
      perror("Erreur fclose()");
    }
    file_sent = true;
  }
  else {
  	perror("Rapport PDF inconnu");
	}

return file_sent;	
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
	  reception = read(employee->des_client,employee->message,sizeof(employee->message));
	  if(!reception)
		  perror("Erreur réception read()");
	  else {
	    request = employee->message[0];
	    cout <<"Employé : "<<  employee->name <<" demande reçue "<< request<<endl;
	                        
	    switch((int)request) 
	    {		   
		    case ADD_LINES :  cout << "Demande d'ajout de ligne" << endl; break;
		    case FINISH_REPORT : cout << "Demande de finalisation" << endl; 
		    										 continu = 0; break;		
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
  Client* employee = (Client*)param;
  int continu = 1;
  
  cout << "Bonjour "<< employee->name <<" !"<< endl;
  while(continu)
  {
    cout << "Que voulez vous faire ?" << endl;
    cout << "1 : Saisir un rapport."<< endl;
    cout << "2 : Télécharger un rapport déjà saisi."<< endl;
    cout << "3 : Quitter" << endl;    
    cout << "Tappez le chiffre correspondant à votre demande";
    
    cin >> continu;
    switch(continu) {
      case 1 : employee_report_to_pdf(employee); break;
      case 2 : download_PDF(employee); break;
      case 3 : continu = 0; break;
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


int isClient(Client client, client_t listeClientsEntreprise[])
{
	int status(0),indice(0);
	bool found(false);
		
	while (listeClientsEntreprise[indice] != NULL && !found)
	{
	  cout << "je suis dans la boucle" << endl;
	  cout << "Nom client : "<< client.name << endl;
	  cout << "nom client courrant : " << listeClientsEntreprise[indice]->name << endl;
	  
		if(strcmp(listeClientsEntreprise[indice]->name,client.name)==0)
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
		  found = true;
		  listeClientsEntreprise[indice]->des_client = client.des_client;
		}
		cout << "test direct ici" << endl;
		indice++;
		
	} // end loop
return status;
}

int authentification (int desClient, client_t listeClientsEntreprise[])
{
	int reception(0),statusClient(-1);
	Client tempCli;
//	tempCli.name = "tintin";
//	cout << tempCli.name << endl;
  
  cout << "Attente login client... "<< endl;
	reception = recv(desClient,tempCli.message,sizeof(tempCli.message),0);
	if(reception == -1)
		perror("Erreur réception recv authentification()");
	else 
	{
		cout << tempCli.message << endl;
		sprintf(tempCli.name,tempCli.message);
		tempCli.des_client = desClient;
		//cout << "Test name : "<< tempCli.name << endl;
		
		statusClient = isClient(tempCli,listeClientsEntreprise);	  

	  cout << "Status client : "<< statusClient << endl;
	  send(desClient,&statusClient,sizeof(int),0);
	} // End if(reception > 0)
	
return statusClient;
}



/**
* Thread lancé dès qu'un client a été accept() par le server. Il va gérer la
* la partie authentification puis oriente le client.
**/
void* th_new_client(void* param)
{
  data_t data = (data_t) param;
  int statusClient = 0;
  
  statusClient = authentification(data->descripteur, data->ptr_client_list);
  if(statusClient == 0) 
  {
    close(data->descripteur);
  }
  else 
  {
    pthread_t idThread;			  		  
    switch(statusClient) 
    {
      case CLIENT_OK : cout << "C'est un client !"<< endl; break;			    			    
      case CLIENT_OK_DISPO : cout << "C'est un client, rapport dispo !"<< endl; 
        if(pthread_create(&idThread,NULL,th_employee_management,
        (void*)data->ptr_client_list[*data->nb_client])!= 0)  {
          perror("Erreur création thread");
        }
        break;			      
      case CONTROLEUR_OK : cout << "C'est un controleur !"<< endl; 
        // lancement thread controleur
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
	client_t listeClientsEntreprise[1];
// On définit un pointeur sur la structure data
  data_t data = (data_t)malloc(sizeof(data_t));
  data->ptr_client_list = listeClientsEntreprise; 


// Test création client
	client_t testcli = (client_t) malloc(sizeof(client_t));
	sprintf(testcli->name,"tintin");	
	sprintf(testcli->password,"milou");
	testcli->claimed_report=false;
	listeClientsEntreprise[0]=testcli;
		
/* Petits test	
  data->nb_client= &nb_client;
  data->ptr_client_list = listeClientsEntreprise;  
  cout << *data->nb_client << endl;
  cout << testcli->name << endl;
  cout << listeClientsEntreprise[0]->password << endl;
*/

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
	
/* Fermeture employee->des_clientet server */
	close(DesServer);

return EXIT_SUCCESS;
}
