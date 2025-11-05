#pragma once
#include <Arduino.h>

// Variable globale indiquant si une interruption a été déclenchée
extern volatile bool interrupted;

// Initialise la gestion d’interruption
void initInterruptHandler();

// Vérifie périodiquement si un Ctrl+C a été reçu
void checkInterrupt();
