#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <Arduino.h>

// Pointeur sur fonction utilisateur (signature standard)
typedef void (*user_program_fn)(void (*print_fn)(const char*));

// États possibles d'un processus
enum ProcessState {
    PS_EMPTY,       // slot vide
    PS_RUNNING,     // en cours d'exécution
    PS_SUSPENDED,   // suspendu (pause)
    PS_ZOMBIE       // terminé, en attente de nettoyage
};

// Structure pour gérer un processus
struct Process {
    int pid;                           // process ID
    const char* name;                  // nom du programme
    user_program_fn func;              // pointeur sur la fonction du programme
    ProcessState state;                // état actuel
    unsigned long created_time;        // timestamp de création
    unsigned long last_scheduled_time; // dernière fois qu'il a eu du CPU
    unsigned long total_runtime_ms;    // temps total d'exécution (ms)
};

// Classe de gestion des processus
class ProcessManager {
public:
    static const int MAX_PROCESSES = 8;

    ProcessManager();

    // Créer et lancer un nouveau processus
    int createProcess(const char* name, user_program_fn func);

    // Récupérer un processus par PID
    Process* getProcess(int pid);

    // Lister tous les processus actifs
    void listProcesses(void (*print_fn)(const char*));

    // Suspend un processus (pause)
    bool suspendProcess(int pid);

    // Reprendre un processus en pause
    bool resumeProcess(int pid);

    // Terminer un processus
    bool killProcess(int pid);

    // Obtenir le prochain processus à exécuter (Round-Robin)
    int getNextRunnable(void);

    // Nombre de processus actifs (RUNNING ou SUSPENDED)
    int countActive(void);

    // Nombre de processus RUNNING
    int countRunning(void);

    // Nettoyer les processus zombies
    void cleanupZombies(void);

private:
    Process processes[MAX_PROCESSES];
    int current_pid_counter;
    int last_scheduled_index; // pour Round-Robin
    
    int findFreeSlot(void);
};

#endif
