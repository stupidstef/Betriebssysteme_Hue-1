#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#include "utils.h"
#include "listen.h"
#include "wortspeicher.h"
#include "kommandos.h"
#include "frontend.h"
#include "parser.h"
#include "variablen.h"

int interpretiere(Kommando k, int forkexec);

void do_execvp(int argc, char **args){
  execvp(*args, args);
  perror("exec-Fehler"); 
  fprintf(stderr, "bei Aufruf von \"%s\"\n", *args);
  exit(1);
}


int interpretiere_pipeline(Liste l, int forkexec) {
	int pipesCount, i, newPipe[2], oldPipe[2];
	pid_t pid;

	pipesCount = listeLaenge(l);

	for(i = 0; i<pipesCount; i++){
		// das nächste auszuführende Kommando
		k = listeKopf(l);

		// Wenn es noch Kommandos zum ausführen gibt
		if(i < pipesCount) {
			pipe(newPipe);
		}

		pid = fork();

		if(pid==0) {
			// Child:
			//Wenn es vorher schon ein Kommando gab
			if(i > 0) {
				close(oldPipe[1]);
				dup2(oldPipe[0], 0);
				close(oldPipe[0]);
			}

			//Wenn es immer noch Kommandos zum ausführen gibt
			if(i < pipesCount - 1){
				close(newPipe[0]);
				dup2(newPipe[1],1);
				close(newPipe[1]);
			}

			//Ausführen
			do_execvp(k->u.einfach.wortanzahl, k->u.einfach.worte);
		} else {
			// Father:
			//Wenn es vorher schon ein Kommando gab
			if(i > 0) {
				close(oldPipe[0]);
				close(oldPipe[1]);
			}

			//Wenn es immer noch Kommandos zum ausführen gibt
			if(i < pipesCount - 1){
				oldPipe[0] = newPipe[0];
				oldPipe[1] = newPipe[1];
			}

			// warten bis letztes kommando ausgeführt wird
			if(i == pipesCount -1 ){
				waitpid(pid, NULL, 0);
				return 0;
			}
			// Den Kopf "abschneiden"
			l = listeRest(l);
		}
	}

	return 0;
}

int umlenkungen(Kommando k){
  /* Umlenkungen bearbeiten */
  return 0;
}

int aufruf(Kommando k, int forkexec){

  /* Programmaufruf im aktuellen Prozess (forkexec==0)
     oder Subprozess (forkexec==1)
  */
     
  if(forkexec){
    int pid = fork();

    switch (pid){
    case -1:
      perror("Fehler bei fork"); 
      return(-1);
    case 0:
      if(umlenkungen(k))
	exit(1);
      do_execvp(k->u.einfach.wortanzahl, k->u.einfach.worte);
      abbruch("interner Fehler 001"); break; /* sollte nie ausgeführt werden */
    default:
      if(k->endeabwarten)
        /* So einfach geht das hier nicht! */
	waitpid(pid, NULL, 0);
      return 0;
    }
  }

  /* nur exec, kein fork */
  if(umlenkungen(k))
    exit(1);
  do_execvp(k->u.einfach.wortanzahl, k->u.einfach.worte);
  abbruch("interner Fehler 001"); /* sollte nie ausgeführt werden */
  exit(1);
}


int interpretiere_einfach(Kommando k, int forkexec){

  char **worte = k->u.einfach.worte;
  int anzahl=k->u.einfach.wortanzahl;

  if (strcmp(worte[0], "exit")==0) {
    switch(anzahl){
    case 1:
      exit(0);
    case 2:
      exit(atoi(worte[1]));
    default:
      fputs( "Aufruf: exit [ ZAHL ]\n", stderr);
      return -1;
    }
  }

  if (strcmp(worte[0], "cd")==0) {
    switch(anzahl){
    case 1:
      return chdir(getenv("HOME"));
    case 2:
      return chdir(worte[1]);
    default:
      fputs("Aufruf: cd [ PFAD ]\n", stderr);
      return -1;
    }
  }

  return aufruf(k, forkexec);
}

int interpretiere(Kommando k, int forkexec){
  int status;

  switch(k->typ){
  case K_LEER: 
    return 0;
  case K_EINFACH:
    return interpretiere_einfach(k, forkexec);
  case K_SEQUENZ:
    {
       Liste l = k->u.sequenz.liste;
       while(!listeIstleer(l)){
			status = interpretiere((Kommando)listeKopf(l), forkexec);
			l = listeRest(l);
		}
    }
    return status;
  case K_PIPE:
	{
       Liste l = k->u.sequenz.liste;
	   status = interpretiere_pipeline(l, forkexec);
	}
	return status;
  default:
    fputs("unbekannter Kommandotyp, Bearbeitung nicht implementiert\n", stderr);
    break;
  }
  return 0;
}

