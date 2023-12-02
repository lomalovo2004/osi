#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void search_in_file(const char* filename, const char* search) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file\n");
        return;
    }

    char* line = NULL;
    size_t line_length = 0;
    int line_number = 0;
    while (getline(&line, &line_length, file) != -1) {
        line_number++;
        if (strstr(line, search) != NULL) {
            printf("Line was found in file %s, line %d: %s\n", filename, line_number, line);
            fclose(file);
            free(line);
            exit(0);
        }
    }

    fclose(file);
    free(line);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Use: %s <file with list of names>\n", argv[0]);
        return 1;
    }

    char* filename = (char*)malloc((strlen(argv[1]) + 1) * sizeof(char));
    if (filename == NULL) {
        perror("Memmory does not allocate for (filename)");
        return 1;
    }
    strcpy(filename, argv[1]);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file with list of names: %s\n", filename);
        free(filename);
        return 1;
    }

    char* search = (char*)malloc(sizeof(char));
    if (search == NULL) {
        perror("Memmory does not allocate for (ssearch)");
        free(filename);
        fclose(file);
        return 1;
    }
    printf("Enter line for search: ");
    size_t search_length = 0;
    getline(&search, &search_length, stdin);
    search[strcspn(search, "\n")] = '\0';

    pid_t pid;
    char* line = NULL;
    size_t line_length = 0;
    while (getline(&line, &line_length, file) != -1) {
        line[strcspn(line, "\n")] = '\0';

        pid = fork();
        if (pid == -1) {
            perror("Error creating pid");
            free(filename);
            free(search);
            free(line);
            fclose(file);
            return 1;
        } 
        else if (pid == 0) {
            search_in_file(line, search);
        }
    }

    fclose(file);
    free(filename);
    free(search);
    free(line);

    int status;
    pid_t child_pid;
    while ((child_pid = wait(&status)) != -1) {
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 1) {
                printf("Line was not found in file %s\n", line);
            }
        } 
        else {
            printf("Pid %d was finished incorect\n", child_pid);
        }
    }

    return 0;
}
