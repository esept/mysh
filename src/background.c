//
// Created by XuZY on 2023/12/23.
//

#include "background.h"
#include "main.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


/**
* 5 Premier et arrière plans
Jusqu’à présent toutes les commandes exécutées, l’étaient en premier plan ou en « foreground », c’est- à-dire que l’on attendait la fin de l’exécution de l’ensemble de la ligne de commandes avant d’afficher de nouveau l’invite de commandes. Votre shell devra prévoir le lancement de commandes en arrière-plan ou en « background », c’est-à-dire que l’invite de commandes sera affichée sans attendre la fin de la commande. Pour celà un & suivra la commande à lancer en background.
Exemple 7
∼> emacs &
∼> ls -lR | gzip > ls-lR.gz &
Au lancement d’une commande en arrière plan, avant de réafficher le l’invite de commandes, votre shell affichera une ligne de la forme : [xxx] yyy où xxx représente le numéro du « job » en « background » et yyy le pid du processus. Le numéro de job est un compteur qui est réinitialisé lorsque plus aucun job n’est exécuté en arrière plan.
Lorsqu’une commande lancée en arrière plan se termine, on affichera :
zzz (jobs=[xxx], pid=yyy) terminée avec status=sss
où zzz représente la commande lancée, xxx le numéro de job, yyy le pid et sss le code de retour de la commande (la valeur -1 sera affichée si la commande s’est terminée anormalement).
5.1 Commande myjobs
La commande interne myjobs permettra d’afficher la liste des processus en arrière plan. L’affichage se fera de la manière suivante : [xxx] yyy Etat zzz oùxxxreprésentelenumérodejob,yyylepid,Etatl’étatduprocessusquipourraêtreEn cours d’exécution ou Stoppé et zzz la commande lancée. Un job par ligne sera affiché.
5.2 Passer une commande de foreground à background et inversement
Lorsqu’une commande est lancée en foreground, il est possible de l’interrompre en lui propageant le signal envoyé par Ctrl-Z (signal SIGTSTP). La commande est alors stoppée, votre shell reprend la main en indiquant que la commande zzz devient le job xxx et qu’il est Stoppé.
Les commandes internes myfg et mybg permettent de modifier l’état d’un job. Ces commandes peuvent ad- mettre un numéro de job en paramètre. La commande mybg permettra de passer le job stoppé ou en foreground de plus grand numéro ou de numéro passé en paramètre en exécution en arrière plan. Inversement, la com- mande myfg permettra de passer le job stoppé ou en background de plus grand numéro ou de numéro passé en paramètre en exécution en premier plan. Le signal SIGCONT permet la demande de reprise d’un processus précédemment stoppé. Si la commande mybg s’applique à un numéro de processus déjà en « background », un erreur est signalée et aucune changement n’est réalisé sur les jobs.
*/