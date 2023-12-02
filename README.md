# Mini-Shell

Ce projet à pour but de concevoir un mini-shell nommé mysh en Langage C.

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
    - La commande interne `exit` (quitte sans tuer les tâches de fonds);
    - Le `Ctrl c` qui demande confirmation et tuer les tâches de fonds avant de quitter;

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

## STOPPE BEFORE --> 4. Les redirections
