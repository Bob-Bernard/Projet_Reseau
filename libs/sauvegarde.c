// compilation : gcc `pkg-config --cflags --libs gtk+-2.0` sauvegarde.c -o sauvegarde

#include "sauvegarde.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <glib.h>
#include <time.h>
#include <locale.h>


#define TAILLEBUF 100
#define FICHIERIMAGE "../libs/vuillemin.jpg"

int OuvreRapport(const char *employe) {
  int res, r;
  FILE *f;
  GString *s = NULL;
  struct stat st;
  
  res = stat(employe, &st);
  if (res == -1) {
    perror("problème à l'ouverture du répertoire");
    return -1;
  }
  if (!(S_ISDIR(st.st_mode))) {
    fprintf(stderr,
	    "problème car '%s' existe mais n'est pas un répertoire\n",
	    employe);
    return -1;
  }
  
  s = g_string_new(employe);
  g_string_append(s, "/");
  g_string_append(s, employe);
  g_string_append(s, ".tex");

  res = open(s->str, O_RDONLY);
  if (res == -1) {
    fprintf(stderr, "problème à l'ouverture du fichier %s\n",
	    s->str);
    perror("");
    return -1;
  }
  close(res);
  g_string_free(s, TRUE);

// make crash the pdf creation
/*  res = open(FICHIERIMAGE, O_RDONLY);*/
/*  if (res == -1) {*/
/*    fprintf(stderr, "problème à l'ouverture du fichier image %s\n",*/
/*	    FICHIERIMAGE);*/
/*    perror("");*/
/*    return -1;*/
/*  }*/
/*  close(res);*/
  

  s = g_string_new(employe);
  g_string_append(s, "/temp.tex");

  f = fopen(s->str, "w");
  g_string_free(s, TRUE);

  fprintf(f, "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage[frenchb]{babel}\n\\usepackage{graphics,graphicx}\n");
  fprintf(f, "\\title{Rapport d'activité}\n\\date{Université Montpellier 2 \\\\ \\mbox{}\\\\ \\mbox{}\\\\\\includegraphics[width=13cm]{%s}}\n",FICHIERIMAGE);
  fprintf(f, "\\author{%s}", employe);
  fprintf(f, "\\begin{document}\n\\maketitle\n\\newpage\n");
  fprintf(f, "\\include{%s}\n", employe);
  fprintf(f, "\\end{document}\n");
  fclose(f);
  
  s = g_string_new("cd ");
  g_string_append(s, employe);
  g_string_append(s, "/; pdflatex temp.tex");
  system(s->str);
  g_string_free(s, TRUE);
  
  s = g_string_new(employe);
  g_string_append(s, "/temp.pdf");
  
  r = open(s->str, O_RDONLY);
  if (r < 0) {
    fprintf(stderr, "problème à l'ouverture du fichier pdf %s\n",
	    s->str);
    perror("");
    return -1;
  }
  g_string_free(s, TRUE);
  
  return r;
}

int Ecrit(const char *message, const char *employe) {
  FILE *f;
  int res;
  struct stat st; 
  time_t t;
  struct tm *tmp;
  char buff[TAILLEBUF];
  GString *s = NULL;

  // setlocale(LC_TIME, "fr_FR") si vous n'utilisez pas l'encodage utf8
  if (setlocale(LC_TIME, "fr_FR.utf8")==NULL) {
    perror("probleme avec setlocal ");
    return -1;
  }

  if (strlen(employe) >= 20) {
    fprintf(stderr, "Le nom de l'employé est trop long : %s\n", employe);
    return -1;
  }

  res = stat(employe, &st);
  if (res == -1) {
    if (errno == ENOENT) { // le répertoire n'existe pas
      res = mkdir(employe, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
      if (res == -1) {
	perror("problème à la création du répertoire");
	return -1;
      }
    } else {
      perror("problème à l'ouverture du répertoire");
      return -1;
    }
  } else {
    if (!(S_ISDIR(st.st_mode))) {
      fprintf(stderr, "problème car '%s' existe déja mais n'est pas un répertoire\n", employe);
      return -1;
    }
  }

  s = g_string_new(employe);
  g_string_append(s, "/");
  g_string_append(s, employe);
  g_string_append(s, ".tex");

  f = fopen(s->str, "a+");
  
  // calcul du temps
  t = time(NULL);
  tmp = localtime(&t);
  if (tmp == NULL) {
    perror("problème avec localtime : ");
    exit(-1);
  }

  //  titre = g_string_new("\n\section{");
  res = strftime(buff, TAILLEBUF, "\n\\section{%A %e %B %Y %Hh %Mmin %Ss}\n",tmp);
  if (res == 0) {
    perror("probleme avec strftime : ");
    return -1;
  }

  fprintf(f, "%s\n", buff);
  fprintf(f, "%s\n", message);
  
  g_string_free(s, TRUE);
  fclose(f);
  return 0;
}

/*int main(int argc, char *argv[]){*/
/*  int fd, res;*/
/*  char buff[100];*/
/*  */
/*  Ecrit("Bloc infos 1", "Employe");*/
/*  Ecrit("Bloc infos 2", "Employe");*/
/*  */
/*  fd = OuvreRapport("Employe");*/

/*  return 0;*/
/*} */
