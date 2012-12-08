#include "sockdist.h"
#include "sock.h"

#include <string>
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


return EXIT_SUCCESS;
}
