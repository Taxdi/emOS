# Projet : Cadavre Exquis

## 1. But du projet

L'objectif est de faire transiter un mot à travers 4 containers répartis sur 2 VMs Proxmox, en appliquant des transformations cryptographiques et une validation finale.
Schéma de flux

    VM1 - C1 (Sourcing) : Pioche un mot → Envoi TCP.

    VM1 - C2 (Crypting) : César (+5) + XOR ("Nirvana") → Envoi TCP (via Bridge Vmbr1).

    VM3 - C1 (Decrypting) : XOR ("Nirvana") + César (−5) → Envoi TCP.

    VM3 - C2 (Validator) : Vérification dictionnaire → Feedback vers VM1-C1.

## 2. Détails Techniques des Containers

| Étape | Localisation | Container | Action Principale | Transformation / Protocole | Destination |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **1. Source** | **VM1** | `c1_initiator` | Sélection du mot | Lecture `dict.txt` (Texte clair) | `c2_caesar` (VM1) |
| **2. Chiffrement Cesar** | **VM1** | `c2_caesar` | Chiffrement | César (+5) | `c3_xor` (VM2) |
| **3. Envoie** | **Bridge** | `vmbr1` | Routage réseau | TCP (Paquets binaires) | VM2 |
| **4. Chiffrement XOR**| **VM2** | `c3_xor` | Chiffrement | XOR ("nirvana") | `c4_decipher` (VM2) |
| **5. Vérification**| **VM2** | `c4_decipher` | Déchiffrement | XOR("nirvana") -> César(-5) | `c1_initiator` (VM1) |

-----------------------------------------------------------------

| Service | Port Écouté | Protocole | Type de Donnée |
| :--- | :--- | :--- | :--- |
| **Relais Chiffré** | `5000` | TCP | Binaire (Bytes) |
| **Relais Déchiffré** | `6000` | TCP | Chaîne (String) |
| **Retour Validation** | `7000` | TCP | Statut (ACK/NACK) |

-----------------------------------------------------------------

## 3. Logique de Communication (Protocole TCP)

Bien que l'UDP soit possible, le TCP est recommandé ici car le chiffrement XOR sur des flux binaires supporte mal la perte de paquets ou le désordre.
Étapes de transformation (Exemple)

Soit le mot "HELLO" :

    César +5 : HELLO → MJQQT

    XOR "Nirvana" : MJQQT ⊕ NIRVA... → [Données Binaires]

    Transmission : Envoi des octets bruts à travers le bridge Vmbr1.

-----------------------------------------------------------------

## 4. Configuration Réseau Proxmox

Pour que la communication fonctionne entre les deux VMs, l'infrastructure doit être configurée ainsi :

    Interface réseau : Chaque VM doit posséder une interface réseau liée au bridge vmbr1.

    Adressage IP :

        VM1 : 192.168.1.10

        VM3 : 192.168.1.20

    Docker-Compose : Les ports des containers doivent être mappés sur les hôtes (ex: 8080:8080) pour permettre le passage entre les VMs.
