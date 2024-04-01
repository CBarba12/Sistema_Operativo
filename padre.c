// proceso_padre.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s [cpu|memoria|disco] PID\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *tipo = argv[1];
    int pid = atoi(argv[2]);
    int status;

    // Creamos un pipe para comunicarnos con el proceso hijo
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Creamos un proceso hijo
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Este es el proceso hijo
        close(pipefd[0]);  // Cerramos la lectura del pipe

        // Redireccionamos la salida estándar al pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        // Ejecutamos el proceso hijo correspondiente según el tipo solicitado
        if (strcmp(tipo, "cpu") == 0) {
            execl("/usr/local/bin/cpu_monitor", "cpu_monitor", argv[2], NULL);
        } else if (strcmp(tipo, "memoria") == 0) {
            execl("/usr/local/bin/memory_monitor", "memory_monitor", argv[2], NULL);
        } else if (strcmp(tipo, "disco") == 0) {
            execl("/usr/local/bin/disk_monitor", "disk_monitor", "/path/to/directory", NULL);
        } else {
            fprintf(stderr, "Tipo de monitor no válido\n");
            exit(EXIT_FAILURE);
        }
    } else {
        // Este es el proceso padre
        close(pipefd[1]);  // Cerramos la escritura del pipe

        // Leemos la salida del pipe
        char buffer[1024];
        ssize_t nbytes = read(pipefd[0], buffer, sizeof(buffer));
        if (nbytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Imprimimos la salida del monitor
        printf("Estadísticas de %s para el proceso %d:\n%s\n", tipo, pid, buffer);

        // Esperamos a que el proceso hijo termine
        waitpid(child_pid, &status, 0);
    }

    return 0;
}
