# Mini-Shell

Ce projet à pour but de concevoir un mini-shell (mysh), en Langage C.

## Fonctionnalités

1. Lancement de commandes
    - Les programmes sont exécutés depuis un invité de commandes qui indique le répertoire courant `~` (la home directory).
    - L'invité de commande est réaffiché à la fin de l'exécution de la commande précédente.

2. Prise en charge d'enchaînements
    - Conditionnels

    ```bash
    ∼> gcc -o mysh myshell.c && ./mysh
    ∼> test -d .can || mkdir .can
    ```

    - Inconditionnels

    ```bash
    ∼> ls ; cat /etc/passwd
    ```

3. Wildcards
    Les lignes de commandes du shell remplacent les caractères « jokers » (`*,` `?`, `[]`) par leur(s) correspondance(s) calculée(s) à partir du répertoire courant.

    ```bash
    ∼> cat *.[ch]
    ∼> ls ../[A-Z.]*[ˆ∼]
    ∼> wc -l /etc/?????
    ```

    NB: Les caractères « jokers » précédés d’un `\` ne sont pas à remplacer.

4. Changer de répertoire
    Afin de pouvoir se ballader dans l’arborescence de fichiers, le shell dispose d'un `cd` interne.

5. Sortie du Shell
    Pour la sortie du shell
    - La commande interne `exit` (quitte sans tue les tâches de fonds);
    - Le `Ctrl c` qui demande confirmation et tue les tâches de fonds avant de quitter;

6. Code de retour d'un processus
    La commande interne status affiche pour le dernier processus `xxxx` exécuté en premier plan (foreground):
    - `xxxx` terminé avec comme code de retour `YYY` si le processus s’est terminé normalement;
    - `xxxx` terminé anormalement dans le cas d’une terminaison anormale (comme par exemple l’interruption via un `Ctrl c`);

7. Lister le contenu d'un répertoire
    Pour lister le contenu d'un répertoire, le shell utilise un programme nommé myls (équivalent de `ls -l`).

    ```bash
    ∼> myls / .. foo bar duc
    ∼> myls -aR /
    ∼> myls -R
    ∼> myls -R -a /
    ∼> myls -Ra ∼
    ```

8. Afficher l’état des processus en cours
    Pour afficher l'état des processus en cours, le shell utilise un programme myps (équivalent à `ps aux` avec affichage en couleur en fonction de l'état du processus)

9. Permettre l'usage du pipeline (`|`)
    Il permettra de rediriger la sortie standard du programme précédant le | vers l’entrée standard du programme suivant le |.

    ```bash
    ∼> ls | sort -r
    ∼> ps | grep mysh | wc -l
    ```

    Le premier exemple permet de trier dans l’ordre alphanumérique inverse les entrées du répertoires courant, alors que le second permet de compter le nombre de processus mysh en cours d’exécution.

10. Redirections vers ou depuis un fichier

    ```bash
    ∼> find . -type f -name \*.mp3 >> /home/ens/mazure/listofsongs
    ∼> nl < myshell.c
    ∼> grep ’ˆ#include "’ < myshell.c | sed ’s/#include "\(.*\)"/\1/’ > dependencies
    ```

11. Premier et arrière plans

    ```bash
    ∼> emacs &
    ∼> ls -lR | gzip > ls-lR.gz &
    ```

12. Commande myjobs

13. Passer une commande de foreground à background et inversement

14. Les variables

    **Les variables d’environnment**

    **Les variables locales**

    ```bash
    ∼>set a=foo
    ∼>echo $a
    foo
    ∼>setenv b=duc
    ∼>mysh
    ∼>echo $b
    duc
    ∼>set a=bar
    ∼>set b=tmp
    ∼>echo $b
    tmp
    ∼>setenv b=$a
    ∼>exit
    ∼>echo $b+$a
    bar+foo
    ∼>unset $a
    ```