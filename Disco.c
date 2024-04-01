// disk_monitor.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s directorio\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *directory = argv[1];
    char buffer[BUFFER_SIZE];

    // Obtener estadísticas de espacio en disco
    snprintf(buffer, BUFFER_SIZE, "df -h %s | awk 'NR==2 {print $4,$5}'", directory);
    FILE *pipe = popen(buffer, "r");
    if (!pipe) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    fgets(buffer, BUFFER_SIZE, pipe);
    pclose(pipe);

    // Enviar estadísticas de espacio en disco al proceso padre a través del stdout
    printf("%s", buffer);

    return 0;
}
