# Projet Chat Serveur - TCP et SSL/TLS

## Vidéo de démonstration

Pour mieux visualiser le fonctionnement du chat, voici une vidéo qui montre :

- Connexion des clients sur le chat avec SSL/TLS  
- Gestion des messages colorisés et horodatés  
- Comportement lors de la déconnexion  

> [chat_ssl.webm](https://github.com/user-attachments/assets/fef7f8ab-40e2-4a18-9e46-e6a01dc6f9d8)

Ce projet contient deux versions d'un serveur de chat en language c:

- **Chat TCP classique** : un serveur/client simple utilisant des connexions TCP non chiffrées, idéal pour les environnements de développement ou de test en local.
- **Chat SSL/TLS** : un serveur/client sécurisé destiné à la production, utilisant SSL/TLS via OpenSSL pour chiffrer toutes les communications, garantissant confidentialité et intégrité des échanges.

## Sommaire

- [Vidéo de démonstration](#vidéo-de-démonstration)  
- [Fonctionnalités](#fonctionnalités)  
- [Distinction entre version TCP et SSL](#distinction-entre-version-tcp-et-ssl)  
- [Génération et gestion des certificats SSL](#génération-et-gestion-des-certificats-ssl)  
- [Déploiement](#déploiement)  
- [Licence](#licence)

## Fonctionnalités

- Gestion multi-utilisateurs avec listes de participants  
- Attribution dynamique de couleurs aux pseudos dans le chat  
- Support TCP simple ou SSL/TLS sécurisé  
- Messages horodatés envoyés à tous les clients connectés  
- Messages de bienvenue et départ avec diffusion à tous  
- Gestion propre des déconnexions et erreurs réseau  

## Distinction entre version TCP et SSL

| Aspect            | Chat TCP classique            | Chat SSL/TLS (Production)             |
|-------------------|------------------------------|--------------------------------------|
| **Protocole**     | TCP sans chiffrement          | TCP avec SSL/TLS via OpenSSL         |
| **Sécurité**      | Transmissions en clair        | Chiffrement complet des données      |
| **Usage**         | Développement, tests locaux  | Environnements production ou réseau non sécurisé |
| **Performance**   | Légèrement plus rapide (pas de chiffrement) | Chiffrement induit une légère surcharge |
| **Nécessité**     | Facilité d’implémentation     | Protection contre écoute et attaques man-in-the-middle |

Le serveur TCP simple est parfait pour commencer et comprendre la base du fonctionnement. Pour déployer en production, il est fortement recommandé d’utiliser la version SSL qui assure la confidentialité de vos échanges.

## Génération et gestion des certificats SSL

Pour que le serveur SSL fonctionne correctement, il nécessite un **certificat SSL** ainsi qu’une clé privée. Voici comment générer un certificat auto-signé pour un usage de test ou développement :

```bash
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes

```
- Cette commande crée un certificat (`cert.pem`) valable un an et une clé privée (`key.pem`).
- Tu seras invité à remplir des informations comme le pays, la région, l'organisation.
- Le certificat auto-signé provoquera un avertissement navigateur/terminal car il n’est pas émis par une autorité reconnue.

### Pour un usage en production

- Il est conseillé d’obtenir un certificat via une **autorité de certification (CA)** reconnue, par exemple Let's Encrypt.
- Let's Encrypt offre des certificats gratuits et automatisés.
- Le certificat assure que les clients peuvent vérifier l’identité du serveur et éviter les attaques d’usurpation.

### Pourquoi un certificat ?

- Le certificat contient une clé publique qui permet au client de chiffrer les données vers le serveur.
- La clé privée (qui reste sur le serveur) permet de déchiffrer ces données.
- Le protocole SSL/TLS sécurise la négociation initiale (handshake) pour établir un canal sécurisé.

## Déploiement

### Pour la version SSL. 

#### Pour le serveur:
- Compiler avec les bibliothèques OpenSSL (`-lssl -lcrypto`) pour la version SSL (installer la bibliotheque et faite tout simplement make jai deja configurer le Makefile en adequation).  
- Lancer le serveur SSL avec la clé et le certificat préconfigurés dans le dossier de l'executable.  
- Configurer les ports (par défaut 8080 ) selon ton réseau (pour le serveur redefinir la macro listening-port.)
- Quitter proprement en tapant /quit.
  
#### Pour le client:
- Compiler en faisant make.
- Renseigner en arguments le pseudo l'adresse au format 127.0.0.1 (par exemple) puis le port : 8080
  
  ``` bash
  ./Client toto 127.0.0.1 8080
  ```
- Quitter proprement en tapant /quit.
  
### Pour la version TCP.  

#### Pour le serveur:
- Compiler en faisant make.
- Configurer les ports (par défaut 8080 ) selon ton réseau (pour le serveur redefinir la macro listening-port.)

#### Pour le client:
- Compiler en faisant make.
- Renseigner en arguments le pseudo l'adresse au format 127.0.0.1 (par exemple) puis le port : 8080
  
  ``` bash
  ./Client toto 127.0.0.1 8080
  ```

--- 
