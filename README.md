# Mini-Shell

Ce projet à pour but de concevoir un mini-shell (mysh), en Langage C.

```bash
$ ls
1S-C-project-myshell-readme.md  bin  docs  include  Makefile  obj  README.md  mysh  src  test_mysh.md
chmod +x mysh
./mysh
```

[La vidéo](https://drive.google.com/file/d/11YxA0DOFH0yJBsS8oTgiCZeZPF8kfljN/view?usp=sharing)

[Voir sur Github](https://github.com/esept/mysh/tree/main)

[La suite des tests](./docs/test_mysh.md)

| function               | file         | realisator |
| ---------------------- | ------------ | ---------- |
| myls                   | myls.c       | xu         |
| signal (SIGINT)        | main.c       | xu         |
| variable (env + local) | variable.c   | xu         |
| redirect               | exec.h       | xu         |
| logic operator & `;`   | prepro_input | xu         |
| myps                   | myps.c       | Bernard    |
| status                 | status.c     | Bernard    |
| wildcards              | in exec.c    | Bernard    |
| signal (suite)         | in main.c    | Bernard    |

Xu avait débuté le projet d'au moins 10% avant que n'y prenne part.

| members | % of work |
| ------- | --------- |
| Xu      | 60%       |
| Bernard | 40%       |

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

    ex:

    ```bash
    mysh ~> wc -l /etc/?????
    wc: /etc/avahi: est un dossier
        0 /etc/avahi
    wc: /etc/dconf: est un dossier
        0 /etc/dconf
        .
        .
        .
    wc: /etc/rcS.d: est un dossier
        0 /etc/rcS.d
        3 /etc/vtrgb
        123 total
    ```

4. Changer de répertoire
    Afin de pouvoir se ballader dans l’arborescence de fichiers, le shell dispose d'un `cd` interne.

5. Sortie du Shell
    Pour la sortie du shell
    - La commande interne `exit` (quitte sans tue les tâches de fonds);
    - Le `Ctrl c` qui demande confirmation et tue les tâches de fonds avant de quitter;

    ```bash
    mysh ~> ^C Do you really want to quit? [y/n] y
    ./mysh : ligne 4 : 38846 Processus arrêté      ./bin/exec
    ```

6. Code de retour d'un processus
    La commande interne status affiche pour le dernier processus `xxxx` exécuté en premier plan (foreground):
    - `xxxx` terminé avec comme code de retour `YYY` si le processus s’est terminé normalement;
    - `xxxx` terminé anormalement dans le cas d’une terminaison anormale (comme par exemple l’interruption via un `Ctrl c`);

    ```bash
    mysh ~> status
    No terminated process.
    mysh ~> ls
    1S-C-project-myshell-readme.md  bin  docs  include  Makefile  obj  README.md  mysh  src  test.m  test_mysh.md
    mysh ~> status
    `39706` terminé avec comme code de retour 0
    mysh ~> 
    ```

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

    Pour afficher l'état des processus en cours, le shell utilise un programme myps (équivalent à `ps aux` avec affichage en couleur en fonction de l'état du processus).

    ```bash
    /home/trab/Bureau/mysh ~> myps
    USER     PID   %CPU  %MEM  VSZ      RSS      TTY      STAT  START    TIME     COMMAND 
    root     1     0.0   0.2   166716   12072    ?        S     01.18    00:00:50 (systemd)
    root     2     0.0   0.0   0        0        ?        S     12.29    00:00:00 (kthreadd)
    root     3     0.0   0.0   0        0        ?        I     12.29    00:00:00 (rcu_gp)
    .
    .
    .
    root     39587 0.0   0.0   0        0        ?        I     12.29    00:00:00 (kworker/3:2)
    trab     39666 0.0   0.0   12648    3840     ?        S     12.31    00:00:00 (mysh)
    trab     39669 0.0   0.0   2808     1792     ?        R     12.30    00:00:00 (exec)
    trab     39826 0.0   0.2   4258744  18128    ?        S     05.17    00:00:00 (cpptools-srv)
    ```

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

[LA SUITE DES TESTS](./docs/test_mysh.md)

## Anomalies / Non implémentées

On a pas eu le temps d'implémenter quelques fonctionnalités:

- Ctrl d --> infinite loop
- myjobs
- myfg
- mybg
