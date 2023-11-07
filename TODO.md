# Cahier de Spécifications Techniques pour mysh

## Phase 1: Lancement de Commandes

| Tâche               | Étapes Clés                                  | Axé sur...                               | Ressources Utiles                 |
|---------------------|---------------------------------------------|-----------------------------------------|-----------------------------------|
| Exécution de Commandes | - Capturer et analyser les commandes entrées depuis l'invite. | - Interprétation des commandes. | - Analyse syntaxique.           |
| Attente d'Exécution   | - Exécuter les commandes et attendre leur achèvement. | - Contrôle du processus.     | - Gestion des processus.     |
| Retour à l'Invite     | - Réafficher l'invite de commandes après l'exécution. | - Interaction utilisateur.    | - Interface utilisateur.     |

## Phase 2: Séquencement et Opérateurs

| Tâche               | Étapes Clés                                         | Axé sur...                                    | Ressources Utiles                 |
|---------------------|----------------------------------------------------|----------------------------------------------|-----------------------------------|
| Séquencement        | - Gérer les séquences de commandes inconditionnelles. | - Contrôle du flux d'exécution.          | - Logique de séquencement.      |
| Opérateurs && et || | - Gérer les opérateurs de séquençage conditionnel.    | - Gestion des erreurs et succès.         | - Logique conditionnelle.       |

## Phase 3: Gestion des Wildcards

| Tâche               | Étapes Clés                             | Axé sur...                               | Ressources Utiles                 |
|---------------------|----------------------------------------|-----------------------------------------|-----------------------------------|
| Expansion Wildcards | - Identifier et gérer les wildcards (*, ?, [ens]). | - Correspondance des fichiers. | - Gestion des expressions régulières. |
| Caractères spéciaux | - Traiter les caractères spéciaux (\\). | - Interprétation des caractères. | - Analyse des chaînes de caractères.   |

## Phase 4: Commandes Internes

| Tâche               | Étapes Clés                       | Axé sur...                        | Ressources Utiles                 |
|---------------------|----------------------------------|----------------------------------|-----------------------------------|
| Changement de Répertoire | - Implémenter la commande interne 'cd'. | - Gestion de la navigation.  | - Gestion des répertoires.     |
| Sortie du Shell   | - Créer les commandes internes 'exit' et 'Ctrl-C'. | - Terminaison du shell. | - Gestion des signaux.       |
| Code de Retour    | - Intégrer la commande interne 'status'. | - Affichage des retours.   | - Contrôle des processus.    |

## Phase 5: Lister le Contenu d'un Répertoire

| Tâche               | Étapes Clés                              | Axé sur...                          | Ressources Utiles                 |
|---------------------|-----------------------------------------|------------------------------------|-----------------------------------|
| Commande myls     | - Créer la commande externe 'myls'. | - Listage de fichiers.       | - Commandes système (ls).    |
| Options -a et -R  | - Implémenter les options '-a' et '-R'. | - Personnalisation de l'affichage. | - Manipulation de répertoires. |

## Phase 6: Afficher l'État des Processus

| Tâche               | Étapes Clés                          | Axé sur...                       | Ressources Utiles                 |
|---------------------|-------------------------------------|---------------------------------|-----------------------------------|
| Commande myps     | - Créer la commande externe 'myps'. | - Affichage des processus.  | - Commande 'ps aux'.         |
| Mise en Couleur   | - Ajouter une mise en couleur en fonction de l'état. | - Visibilité des processus. | - Gestion de couleurs.     |

## Phase 7: Les Redirections

| Tâche               | Étapes Clés                      | Axé sur...                         | Ressources Utiles                 |
|---------------------|---------------------------------|-----------------------------------|-----------------------------------|
| Les Tubes         | - Permettre le pipeline avec '|'. | - Redirection des flux.        | - Gestion des tubes.         |
| Redirections vers/depuis un Fichier | - Gérer les redirections vers/depuis un fichier. | - Flux d'E/S vers des fichiers. | - Gestion des fichiers. |

## Phase 8: Premier et Arrière-Plans

| Tâche               | Étapes Clés                               | Axé sur...                            | Ressources Utiles                 |
|---------------------|------------------------------------------|--------------------------------------|-----------------------------------|
| Exécution en Arrière-Plan | - Permettre l'exécution en arrière-plan avec '&'. | - Exécution asynchrone.    | - Gestion des processus en arrière-plan. |
| Affichage des Jobs | - Afficher les informations des jobs en arrière-plan. | - Suivi des tâches.       | - Gestion des jobs.           |
| Gestion de Job      | - Implémenter les commandes internes 'myfg' et 'mybg'. | - Contrôle des jobs.     | - Gestion des signaux.       |

## Phase 9: Les Variables

| Tâche               | Étapes Clés                                | Axé sur...                             | Ressources Utiles                 |
|---------------------|-------------------------------------------|---------------------------------------|-----------------------------------|
| Variables d'Environnement | - Gérer les variables d'environnement. | - Stockage partagé des variables.  | - Manipulation des variables.  |
| Variables Locales   | - Implémenter les variables locales.     | - Portée locale des variables.      | - Gestion des variables.      |
| Utilisation des Variables | - Permettre l'accès et la définition de variables. | - Personnalisation des commandes. | - Syntaxe des variables.      |

## Modalité de Remise du Projet

| Tâche               | Étapes Clés                                | Axé sur...                             | Ressources Utiles                 |
|---------------------|-------------------------------------------|---------------------------------------|-----------------------------------|
| Préparation du Projet | - Préparer le projet conformément aux spécifications. | - Préparation pour la livraison.  | - Organisation du projet.    |
| Livraison du Projet  | - Soumettre le projet par email avec le README. | - Remise du projet.              | - Communication par email.   |

N'oubliez pas de respecter les délais de
