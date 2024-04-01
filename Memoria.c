// memory_monitor.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s PID\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pid = atoi(argv[1]);
    char buffer[BUFFER_SIZE];

    // Leer estadísticas de memoria del proceso
    snprintf(buffer, BUFFER_SIZE, "ps -p %d -o %%mem= | awk '{print $1}'", pid);
    FILE *pipe = popen(buffer, "r");
    if (!pipe) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    fgets(buffer, BUFFER_SIZE, pipe);
    pclose(pipe);

    // Enviar estadísticas de memoria al proceso padre a través del stdout
    printf("%s", buffer);

    return 0;
}
