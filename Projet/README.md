# Projet : Cadavre Exquis

## 1. But du projet

L'objectif est de faire transiter un mot à travers 4 containers répartis sur 2 VMs Proxmox, en appliquant des transformations cryptographiques et une validation finale.
Schéma de flux

    VM1 - C1 (Sourcing) : Pioche un mot → Envoi TCP.

    VM1 - C2 (Crypting) : César (+5) + XOR ("Nirvana") → Envoi TCP (via Bridge Vmbr1).

    VM3 - C1 (Decrypting) : XOR ("Nirvana") + César (−5) → Envoi TCP.

    VM3 - C2 (Validator) : Vérification dictionnaire → Feedback vers VM1-C1.
