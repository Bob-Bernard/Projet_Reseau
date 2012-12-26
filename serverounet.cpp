#include "libs/sock.h"
#include "libs/sockdist.h"
#include "libs/sauvegarde.h"

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
  client_t* ptr_client_list;
  int* nb_connected_client;
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
bool verification_demande_rapport(client_t employee)
{
  return employee->claimed_report;
}

/**
* Réception d'un rapport que doit envoyer un employé
**/
void employee_report_to_pdf(client_t employee)
{
  int continu(-1),reception(0);
  int request(1);
  
  //send(employee->des_client,&request,sizeof(int),0);  
  cout << "Début saisie rapport par le client "<< employee->name << endl;
  //cout << "Des client : "<< employee->des_client << endl;
  
  while(continu)
  {
    if(recv(employee->des_client,&request,sizeof(int),0)==-1) {
      perror("Erreur reception report to PDF");
		}
	  else {
	    cout << "Demande reçue : "<< request << endl;
	                        
	    switch(request) 
	    {		   
		    case ADD_LINES :  cout << "Demande d'ajout de ligne" << endl; 
		      // lancement fonction ajoute ligne rapport
		      break;
		    case FINISH_REPORT : cout << "Demande de finalisation" << endl; 
		      continu = 0;
		      // lancement OuvreRapport
		    	break;		
		    default : cerr << "Erreur switch reportToPDF"<<endl;
	    }	
    }
  } // end loop
  
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
void* th_employee_management(void* param) 
{
  client_t employee = (client_t)param;
  int request(-1);
  
  cout << endl<< "Employe management : " << employee->name << endl;
  cout<< "Des client : " << employee->des_client << endl;
    
//  while(continu)
//  {
    if(recv(employee->des_client,&request,sizeof(int),0)==-1)
      perror("Erreur reception employee management");
      
    switch(request) {
      case 1 :  cout << "employee go to report pdf"<<endl;
        employee_report_to_pdf(employee);
        break;
      case 2 :  cout << "employee go to download pdf"<<endl;
        download_PDF(employee);
        break;
      case 3 : request = 0; 
        cout << "employee quit" << endl;
        break;
      default : cout << "Erreur de saisie, veuillez recommencer" << endl;
    }
    
//  }
  close(employee->des_client); 
pthread_exit(NULL);
}


/**
*
**/
void * th_verif_presence_rapport(void* param) 
{

	pthread_exit(NULL);
}


int* isClient(Client client, int* nb_client, client_t listeClientsEntreprise[])
{
	int status(0),indice(0);
	bool found(false);
	int* infos_client = (int*)malloc(2*sizeof(int));
	
	if(infos_client == NULL ) {
       perror("Allocation impossible");
       exit(EXIT_FAILURE);
  }
	infos_client[0]= status;
	infos_client[1]= indice;
		
	while (indice < *nb_client && !found)
	{
	  //cout << "indice courant : " << indice <<endl;
	  cout << listeClientsEntreprise[indice]->name<< " == " << client.name<< endl;
	  
		if(strcmp(listeClientsEntreprise[indice]->name,client.name)==0)
		{
			if(listeClientsEntreprise[indice]->controller == true) {
				status = CONTROLEUR_OK;
			}
			else if(verification_demande_rapport(listeClientsEntreprise[indice]))	{
					status = CLIENT_OK_DISPO;	
			}
			else {
				status = CLIENT_OK;
		  }
		  found = true;
		  infos_client[0]= status;
		  infos_client[1]= indice;
		  cout << "status : " << infos_client[0] << " indice : "<< infos_client[1] << endl;
		  
		  // Accès concurrant ?
		  listeClientsEntreprise[indice]->des_client = client.des_client;
		}
		indice++;		
	} // end loop
	cout << "test client name after strcmp : "<<listeClientsEntreprise[0]->name<<endl;
return infos_client;
}

int* authentification (int desClient, int* nb_client, client_t listeClientsEntreprise[])
{
	int reception(-1);
	int* infos_client(NULL);
	//int* infos_client = (int*)malloc(2*sizeof(int));
	
	Client temp_client;
  
  cout << "Attente login client... "<< endl;
	reception = recv(desClient,temp_client.message,sizeof(temp_client.message),0);
	if(reception == -1)
		perror("Erreur réception recv authentification()");
	else 
	{
		cout << "login : " << temp_client.message << endl;
		sprintf(temp_client.name,temp_client.message);
		temp_client.des_client = desClient;
		
		infos_client = isClient(temp_client, nb_client, listeClientsEntreprise);
//		infos_client[0]=1;
//		infos_client[1]=1;
		
	  cout << "Status client : "<< infos_client[0] << endl;
	  send(desClient,&infos_client[0],sizeof(int),0);
	} // End if(reception > 0)
	
return infos_client;
}



/**
* Thread lancé dès qu'un client a été accept() par le server. Il va gérer la
* la partie authentification puis oriente le client.
**/
void* th_new_client(void* param)
{
  data_t data = (data_t) param;
 	int* infos_client(NULL);
  int statusClient(-1);
  
  cout << "test client name th_new : "<< data->ptr_client_list[0]->name << endl;
  
  infos_client = authentification(data->descripteur,data->nb_client,data->ptr_client_list);
  cout << "Status : " << infos_client[0] << " Indice : "<< infos_client[1] << endl;
  
  statusClient = infos_client[0];
  if(statusClient == 0) 
  {
    cout << "Erreur identifiants"<<endl;
    close(data->descripteur);
  }
  else 
  {
    pthread_t idThread;			  		  
    switch(statusClient) 
    {
      case CLIENT_OK : cout << "C'est un client !"<< endl;
        close(data->descripteur);
        break;			    			    
      case CLIENT_OK_DISPO : cout << "C'est un client, rapport dispo !"<< endl; 
        if(pthread_create(&idThread,NULL,th_employee_management,
        (void*)data->ptr_client_list[infos_client[1]] )!= 0)  
        {
          perror("Erreur création thread");
        }
        break;			      
      case CONTROLEUR_OK : cout << "C'est un controleur !"<< endl; 
        // lancement thread controleur
        break;
      default : perror("Petit souci switch(statusClient)");
        close(data->descripteur);
        exit(EXIT_FAILURE);
    }
    //cout << "Compteur nombre connectés incrémenté !" << endl;
    //data->nb_connected_client++; // gestion d'accès concurrant ?
  }
  if(infos_client != NULL)
  free(infos_client);
  
  pthread_exit(NULL);
}



int main(int args,char* argv[]) {
	int port, desCurrentClient, DesServer, localBR, nb_connected_client(0);
	struct sockaddr_in brCv;
	socklen_t sizeLocalBr;
	
	int nb_client = 3;
  client_t listeClientsEntreprise[nb_client];
	
// On définit un pointeur sur la structure data
  data_t data = (data_t)malloc(sizeof(data_t));
  data->ptr_client_list = listeClientsEntreprise;

// Test création client
	client_t testcli = (client_t) malloc(sizeof(client_t));
	sprintf(testcli->name,"tintin");	
	sprintf(testcli->password,"milou");
	testcli->claimed_report=false;
	
	client_t testcli2 = (client_t) malloc(sizeof(client_t));
	sprintf(testcli2->name,"bob");	
	sprintf(testcli2->password,"boby");
	testcli2->claimed_report=false;
	
	client_t testcli3 = (client_t) malloc(sizeof(client_t));
	sprintf(testcli3->name,"tutu");	
	sprintf(testcli3->password,"boby");
	testcli3->claimed_report=true;
	

	listeClientsEntreprise[0] = testcli;
	listeClientsEntreprise[1] = testcli2;
	listeClientsEntreprise[2] = testcli3;
	data->nb_client = &nb_client;

		
/* Petits test	
  data->nb_connected_client= &nb_connected_client;
  data->ptr_client_list = listeClientsEntreprise;
  cout << *data->nb_client << endl;
  cout << *data->nb_connected_client << endl;
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
	
	if(listen(DesServer,10) == -1)
	perror("Erreur listen()");

	while(1)
	{
/* Acceptation de la connexion du Client */
		desCurrentClient = accept(DesServer,(struct sockaddr *)&brCv,&sizeLocalBr);
		if(desCurrentClient == -1)
			perror("Erreur accept ");
		else {
  		pthread_t idThread; 
			cout << "Nouveau Client accepté" << endl<<endl;
		  data->descripteur = desCurrentClient;
		  
		  cout << "test nom client main : " <<listeClientsEntreprise[0]->name << endl;
			
			if(pthread_create(&idThread,NULL,th_new_client,(void*)data) != 0) {
        perror("Erreur création th_new_client");
      }
		}// End if Client accepté
	} // End loop
	
/* Fermeture employee->des_clientet server */
	close(DesServer);

return EXIT_SUCCESS;
}
