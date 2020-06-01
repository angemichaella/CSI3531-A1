/*------------------------------------------------------------
Fichier: cpr.c

Nom #1: Ange Michaella NIYONKURU
Numero d'etudiant #1: 8962161

Nom #2: Mugabo Kevin GATERA
Numero d'etudiant #2: 8725061

Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.

Explication du processus zombie
(point 5 de "A completer" dans le devoir):

	(s.v.p. completez cette partie);
	Explication: on a un processus zombie, qui ne consomme aucune resource
	de la memoire, et dont la fermeture est occasionée par l'appel de la
	fonction wait() par le processus parent.

-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
/* Prototype */
void creerEnfantEtLire(int );

/*-------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
    int numeroProcessus; 

    if(ac == 2)
    {
       if(sscanf(av[1],"%d",&numeroProcessus) == 1)
       {
           creerEnfantEtLire(numeroProcessus);
       }
       else fprintf(stderr,"Ne peut pas traduire argument\n");
    }
    else fprintf(stderr,"Arguments pas valide\n");
    return(0);
}


/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments: 
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a 
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{

    /* S.V.P. completez cette fonction selon les
       instructions du devoirs. */

	printf("Processus %d commence \n", prcNum);
	fflush(stdout);

	if (prcNum > 1)
	{
		int tuyau[2];
		if (pipe(tuyau) == -1)
		{
			fprintf(stderr, "Erreur du tuyau");
			exit(1);
		}

		char arg[10];
		sprintf(arg, "%d", prcNum - 1);

		// Creation de l' enfant direct
		pid_t pid = fork();

		//Vérification d'erreur avec la fonction fork
		if (pid < 0)
		{
			fprintf(stderr, "Erreur Fork");
			exit(1);
		}
		// Maintenant que les processus ont ete crees, 
		// il faut maintenant ecrire le contenu du tuyau a l' ecran 
		else if (pid > 0)
		{
			close(tuyau[1]);
			char buf[1];

			// lecture du bout de lecture du tuyeau et ecriture des données
            // lues à la sortie standard
			while (read(tuyau[0], buf, 1) > 0)
			{
				write(1, buf, 1);
			}
			close(tuyau[0]);
			wait(NULL);
		}
		// Fermer et dupliquer le tuyau courrant
		else if (pid == 0)	
		{
			close(tuyau[0]);
			dup2(tuyau[1], 1);

			// Lancer/executer le programme cpr courrant dans le processur enfant
			execlp("./cpr", "cpr", arg, (char *)NULL);
		}
	}
	else
	{
		// Insérer un délai de 10 secondes (avec sleep(10)) avant la terminaison des processus 
		// pour introduire un délai après avoir écrit le message «Processus termine».
		sleep(5);
	}
	
	printf("Processus %d termine \n", prcNum);
	fflush(stdout);
}
