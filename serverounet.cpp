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

#define ADD_CLAIMED_REPORT 1
#define DL_REPORT 2

#define REPORT_READY 1
#define REPORT_WAITING 2
#define CLIENT_NOT_FOUND 3

struct Client {
  char message[BUFFER_SIZE];
	int des_client;
	char name[50];
	char password[30];
	bool controller;
	bool claimed_report;
	bool received_report;
};
//typedef Client* P_Client;
typedef Client* client_t;

struct data {
  int descripteur;
  client_t* ptr_client_list;
  int* nb_connected_client;
  int* nb_client;
};
//typedef data* P_data;
typedef data* data_t;

struct controller_infos {
   client_t controller;
   data_t data;  
};

/**
* Gère l'envoi du rapport PDF à l'employé
**/
void download_PDF(client_t employee)
{
//  cout << "descripteur client : " << employee->des_client << endl;
//  cout << "nom client : " << employee->name << endl;
	int file_size(0),read(-1),des_client(employee->des_client);
	bool file_sent(false);
	const char* pdf_path = "docs/sujet.pdf";
	FILE* pdf_file = NULL;
	pdf_file = fopen(pdf_path, "rb");
	char file_content[BUFFER_SIZE];

//  cout << "descripteur client : " << employee->des_client << " " <<des_client << endl;
//  cout << "nom client : " << employee->name << endl;
	if (pdf_file != NULL)
	{
		cout << "Rapport ouvert !" << endl;
		fseek(pdf_file,0,SEEK_END);
		file_size = ftell(pdf_file);
		rewind(pdf_file);		
		send(des_client,&file_size,sizeof(int),0);
		  
		while(read != 0)
		{
		  read = fread(file_content,sizeof(char),1024,pdf_file);		  
    	send(des_client, file_content,read,0);
    }
    
    cout << "Rapport envoyé !" << endl;
    file_sent = true;
    if(fclose (pdf_file) != 0) {
      perror("Erreur fclose()");
    }
  }
  else {
  	perror("Rapport PDF inconnu");
	}

}

/**
* Déconnecte un client
**/
void disconnection(client_t client)
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
* Ajoute une ligne dans le rapport
**/
void add_lign(client_t client)
{
  int continu=-1;
  cout << "Début d'ajout de lignes"<< endl;
  while (continu!=2)
  {
    if(recv(client->des_client,client->message,sizeof(client->message),0)==0) {
      perror("Erreur réception");
      break;
    }
    cout <<"Message reçu : " << client->message << endl;
    cout <<"Client name : " << client->name <<endl;
    Ecrit("bilboquet","haddock");
    cout << "Ligne écrite !" << endl;
    recv(client->des_client,&continu,sizeof(int),0);
    cout << "Continu : "<< continu << endl;
  }
}
/**
* Réception d'un rapport que doit envoyer un employé
**/
void employee_report_to_pdf(client_t employee)
{
  int continu(-1),reception(0);
  int request(1);
  
  cout << "Début saisie rapport par le client "<< employee->name << endl;
  cout<< "Des client : " << employee->des_client << endl;

  while(continu!=0)
  {
    if(recv(employee->des_client,&request,sizeof(int),0)==-1) {
      perror("Erreur reception report to PDF");
		}
	  else {
	    cout << "Demande reçue : "<< request << endl;
	                        
	    switch(request) 
	    {		   
		    case ADD_LINES :  cout << "Demande d'ajout de ligne" << endl;
		      recv(employee->des_client,employee->message,sizeof(employee->message),0);
  	       //Ecrit(employee->message,employee->name);
  	       Ecrit("turlultuuthjtu","tata");
  	       cout << "Ligne ajoutée !" << endl;
  	       
		      //add_lign(employee);
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
  int request(-1),continu(-1);
  
  cout << endl<< "Employe management : " << employee->name << endl;
  cout<< "Des client : " << employee->des_client << endl;
//  Ecrit("tutu","tata");
//  Ecrit(employee->message,employee->name);
  
  while(continu != 0)
  {
    if(recv(employee->des_client,&request,sizeof(int),0)==-1)
      perror("Erreur reception employee management");
      
    switch(request) {
      case 1 :  cout << "employee go to report pdf"<<endl;
        employee_report_to_pdf(employee);
        break;
      case 2 :  cout << "employee go to download pdf" << endl;
        download_PDF(employee);
        break;
      case 3 : continu = 0; 
        cout << "employee quit" << endl;
        break;
      default : cout << "Erreur de saisie, veuillez recommencer" << endl;
    }
    
  }
  close(employee->des_client); 
pthread_exit(NULL);
}


/**
* si le nom de l'employé existe et doit envoyer rapport, lance le téléchargement.
**/
void download_report_employee(int des_controller, int nb_client, 
client_t listeClientsEntreprise[]) 
{
  int request(-1),indice(0);
  char employee_name[50];
  bool found(false);
  
  recv(des_controller,employee_name,sizeof(employee_name),0);
  cout << "Recherche de l'employé : "<< employee_name << endl;
  
  while (indice < nb_client && !found)
	{		  
		if(strcmp(listeClientsEntreprise[indice]->name,employee_name)==0)
		{
		  cout << "L'employé trouvé !"<< endl;
	    if(listeClientsEntreprise[indice]->received_report = true) 
	    {
	      request = REPORT_READY;
	      send(des_controller,&request,sizeof(int),0);
	      cout << "L'employee a envoyé son rapport, lancement du dl"<<endl;
	      download_PDF(listeClientsEntreprise[indice]);
	    }
	    else {
	      request = REPORT_WAITING;
	      cout << "L'employé n'a pas encore envoyé son rapport" << endl;
	      send(des_controller,&request,sizeof(int),0);	      
	    }
	    found = true;
    }
    indice++;
  }
  
  if(!found) {
    request = CLIENT_NOT_FOUND;
    send(des_controller,&request,sizeof(int),0);
    cerr << "L'employé "<< employee_name<< " n'a pas été trouvé"<<endl;
  }

}

/**
* Permet de modifie la valeur claimed_report d'un client désigné par son nom
**/
void add_claimed_report(int des_controller, int nb_client, 
client_t listeClientsEntreprise[])
{
  char employee_name[50];
  bool found(false);
  int indice(0);
  
  // réception du nom de l'employé
  recv(des_controller,&employee_name,sizeof(employee_name),0);
  cout<< "Début recherche de l'employé : "<< employee_name<< "..." << endl;
  
  while (indice < nb_client && !found)
	{		  
		if(strcmp(listeClientsEntreprise[indice]->name,employee_name)==0)
		{
		  cout << "L'employé trouvé : "<< listeClientsEntreprise[indice]->name << endl;
		  found = true;
		  // accès concurrant
      listeClientsEntreprise[indice]->claimed_report = true;
      send(des_controller,&found,sizeof(bool),0);
      cout << "Un rapport lui a été correctement demandé !" << endl;
    }
    indice++;
  }
  
  if(!found) {
    send(des_controller,&found,sizeof(bool),0);
    cerr << "L'employé "<< employee_name<< " n'a pas été trouvé"<<endl;
  }
  
}


/**
*
**/
void * th_controller_management(void* param) 
{
  struct controller_infos* controller_infos = (struct controller_infos*) param;
  data_t data = controller_infos->data;
  int request(-1),des_controller(controller_infos->controller->des_client);
  int continu(-1);
  
  while(continu != 0)
  {
    // reception de la demande du controller
    recv(des_controller,&request,sizeof(int),0);
    
    switch(request)
    {
      case ADD_CLAIMED_REPORT : cout << "Demande d'ajout une demande de rapport" << endl;
        add_claimed_report(des_controller,*data->nb_client,data->ptr_client_list);
        break;
      case DL_REPORT : cout << "Demande de téléchargement d'un rapport PDF"<<endl;
        download_report_employee(des_controller,*data->nb_client,
        data->ptr_client_list);
        break;
      default : cerr << "Erreur swith th_controller_management" << endl; 
        continu=0; 
    }
    cin.clear();
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
  data_t data(NULL); 
  if(param != NULL) {
    data = (data_t) param;
  }
  else {
    perror("Error : param value");
  }
  struct controller_infos controller_infos;
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
          perror("Erreur création thread employee");
        }
        pthread_join(idThread, NULL);
        break;			      
      case CONTROLEUR_OK : cout << "C'est un controleur !"<< endl;
        controller_infos.controller = data->ptr_client_list[infos_client[1]];
        controller_infos.data = data;
         if(pthread_create(&idThread,NULL,th_controller_management,
        (void*)&controller_infos)!= 0)  
        {
          perror("Erreur création thread controller");
        }
        pthread_join(idThread, NULL);
        break;
      default : perror("Petit souci switch(statusClient)");
        close(data->descripteur);
        exit(EXIT_FAILURE);
    }
    //cout << "Compteur nombre connectés incrémenté !" << endl;
    //data->nb_connected_client++; // gestion d'accès concurrant ?
    if(infos_client != NULL)
    free(infos_client);    
  }
  
  pthread_exit(NULL);
}



int main(int args,char* argv[]) {
	int port, desCurrentClient, DesServer, localBR, nb_connected_client(0);
	struct sockaddr_in brCv;
	socklen_t sizeLocalBr;
	
	int nb_client = 5;
  client_t listeClientsEntreprise[nb_client];
	
// On définit un pointeur sur la structure data
  data_t data = (data_t)malloc(sizeof(data_t));
  data->ptr_client_list = listeClientsEntreprise;
  data->nb_connected_client= &nb_connected_client;

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
	
	client_t testcli4 = (client_t) malloc(sizeof(client_t));
	sprintf(testcli4->name,"beber");	
	sprintf(testcli4->password,"boby");
	testcli4->claimed_report=false;
	
	client_t testController = (client_t) malloc(sizeof(client_t));
	sprintf(testController->name,"haddock"); // enfin lui !
	sprintf(testController->password,"boby");
  testController->controller=true;
  
	listeClientsEntreprise[0] = testcli;
	listeClientsEntreprise[1] = testcli2;
	listeClientsEntreprise[2] = testcli3;
	listeClientsEntreprise[3] = testcli4;
	listeClientsEntreprise[4] = testController;
	data->nb_client = &nb_client;

		
/* Petits test	
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
