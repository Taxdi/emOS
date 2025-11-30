#include "scheduler.h"

// Volatile flag pour gérer l'interruption (setjmp/longjmp style)
volatile static bool interrupt_requested = false;
volatile static int interrupt_target_pid = -1;

Scheduler::Scheduler(ProcessManager* pm) : pm(pm), current_pid(-1), slice_start_time(0) {}

int Scheduler::getCurrentPid(void) {
    return current_pid;
}

void Scheduler::switchContext(void) {
    if (!pm) return;
    current_pid = pm->getNextRunnable();
}

bool Scheduler::tick(unsigned long timeslice_ms, void (*print_fn)(const char*)) {
    if (!pm || pm->countRunning() == 0) {
        return false; // aucun processus exécutable
    }

    // Obtenir le prochain processus
    switchContext();
    if (current_pid == -1) return false;

    Process* p = pm->getProcess(current_pid);
    if (!p || p->func == nullptr) return false;

    slice_start_time = millis();

    // Exécuter le processus pour au max timeslice_ms
    // En pratique sur Arduino/STM32, on n'a pas de vrai preemption
    // Donc on lance le programme et il doit coopérer (yield/delay)
    p->func(print_fn);

    unsigned long elapsed = millis() - slice_start_time;
    p->total_runtime_ms += elapsed;
    p->last_scheduled_time = millis();

    // Nettoyer les zombies
    pm->cleanupZombies();

    return true;
}

void Scheduler::run(void (*print_fn)(const char*)) {
    // Boucle principale du scheduler
    while (pm->countRunning() > 0) {
        tick(100, print_fn); // timeslice de 100ms (approx)
    }
    print_fn("[Kernel] Tous les processus sont terminés.\r\n");
}
