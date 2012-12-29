int OuvreRapport(const char *employe);
// Fonction qui cree et ouvre le rapport d'activité
// employe : le nom de l'employe
// return  : un file descriptor sur le fichier pdf
//           -1 en cas d'erreur

int Ecrit(const char *message, const char *employe);
// Fonction qui sauvegarde un message
// message : le message à sauvegarder
// employe : le nom de l'employe
// return  : 0 en cas de succès, -1 en cas d'échec
